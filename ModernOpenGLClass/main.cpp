#include <iostream>
#include <stdio.h>  // printf
#include <string.h> // strlen
#include <cmath> // math absolute

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

glm::mat4 model = glm::mat4(1.0f);

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Window dimensions
const GLint WIDTH = 800;
const GLint HEIGHT = 600;

GLuint VAO, VBO, VBO1, shader, uniformModelMatrix;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.005f;

// Vertex Shader lets us modify vertices and pass the result onto the fragment shader
static const char* vShader = "\n\
// Version of GLSL                  \n\
# version 330                       \n\
// Values from code below are passed into 'zero' in the shader. You can also make it search by name and not need layout. layout() is only supported since version 3.2 \n\
layout(location = 0) in vec3 pos;    \n\
layout(location = 1) in vec3 color;  \n\
out vec3 vColor;                     \n\
uniform mat4 modelMatrix;            \n\
void main() {                        \n\
vColor = color;                      \n\
// Built in value ot the shader itself. Imagine there's an out vec4. It's built in, we can't see it. The values are calculated as the graphics pipeline goes, and this value is sent to the fragment shader. These are the final positions of the vertices on the screen. Z is how far away this thing is from us \n\
gl_Position = modelMatrix * vec4((pos * vec3(0.4, 0.4, 1.0)), 1.0); \n\
}                                    \n\
";

static const char* fShader = "\n\
// Version of GLSL                   \n\
# version 330                        \n\
// Fragment shader only has one output usually, so we don't need to define this as special or default \n\
out vec4 color;                      \n\
in vec3 vColor;                      \n\
void main() {                        \n\
  color = vec4(vColor, 1.0);  \n\
} \n\
";

void addShader(GLuint program, const char* code, GLenum shaderType) {
    // Create an empty shader of a certain type and get the ID back
    GLuint shdr = glCreateShader(shaderType);
    
    // create an array and pas in a pointer to the first element of the array. This is where
    // we store the code itself
    const GLchar* theCode[1];
    theCode[0] = code;
    
    // Holds the length of the code, this is why we include string.h at the top
    GLint codeLength[1];
    codeLength[0] = (GLint)strlen(code);
    
    // Set up shader source code.
    // - shader id itself
    // - count is because we can have multiple strings, multiple chunks of code, but we've only got one
    // - pass in code itself. Is this why we need a count above?
    // - and length
    glShaderSource(shdr, 1, theCode, codeLength);
    
    // Compiles ther shader code in memory on the graphics card, turning strings into a ... program?
    glCompileShader(shdr);
    
    // Get errors
    GLint result = 0;
    GLchar eLog[1024] = { 0 };
    
    // Get the compile status of the shader, make sure it's valid for the opengl context
    glGetShaderiv(shdr, GL_COMPILE_STATUS, &result);
    if(!result) {
        glGetShaderInfoLog(shdr, sizeof(eLog), NULL, eLog);
        printf("error compiling the %d shader: %s", shaderType, eLog);
    }
    
    glAttachShader(program, shdr);
}

void compileShaders() {
    // stores shader program location in graphics card memory as ID
    shader = glCreateProgram();
    if(!shader) {
        printf("Error creating shader program");
        return;
    }
    
    addShader(shader, vShader, GL_VERTEX_SHADER);
    addShader(shader, fShader, GL_FRAGMENT_SHADER);
    
    // Get error codes from shaders, because we now need to validate and link programs
    GLint result = 0;
    GLchar eLog[1024] = { 0 };
    
    // Create executables on graphics card and link them together. This is after we're finished
    // creating the shader. we have to then check it linked properly and validated
    glLinkProgram(shader);
    
    // Get link status, if it's false, get the info log
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if(!result) {
        // specify size so it doesn't overflow
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("error linking program: %s", eLog);
    }
    
    // Check if the shader is valid in the current context
    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
    if(!result) {
        // specify size so it doesn't overflow
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("error validating program: %s", eLog);
    }
    
    // Get ID of uniform variable in compiled shader (by name)
    uniformModelMatrix = glGetUniformLocation(shader, "modelMatrix");
}

// Create vertex array object - holds multiple VBOs
// Create vertex buffer object
// Usually have multiple vbos per vao
void createTriangle() {
    // by default in opengl, middle of screen is 0,0, and y is up/down,
    // x axis is left/right, and z is depth
    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,
        1.0, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };
    
    // ammount of arrays we want to create, and where we want to store
    // the ID of the array. This defines space on the graphics card memory!
    // for one vertex array, and gives you that Id back
    glGenVertexArrays(1, &VAO); // I changed this from 1 to 2 - is 2 right for 2 VBOs?
    
    // Then we "bind" it...? Now any opengl functions we use that interact with VBOs
    // will interact with this VAO ID. Got it
    glBindVertexArray(VAO);
    
    // Create buffers (how many buffers, and ID we want to use). We've created a buffer
    // object inside the VAO
//    GLuint mahBufs[2];
    glGenBuffers(1, &VBO);
    
    // Which buffer we want to bind to. A VBO has multiple targets it can bind to
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    // Now we want to connect the vertices to the VBO
    // which target to bind to, the size of the data we're passing in (length of buffer)
    // then the data, then GL_STATIC_DRAW or GL_DYNAMIC_DRAW, static meaning we wont change
    // the values in the array. Dynamic lets you change vertex positions while the program is
    // running but it's more complex
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
                 
    // Wont' make as much sense until we get to shaders
    // - location of attribute we create shader for. We want to point to attribute 0 in shader
    // - size of each value getting passed in
    // - type of these values
    // - if you want to normalize the values (we aren't, we want them to go as is)
    // - with VBOs you can splice data together, you can splice colors and vertices together
    //   in the array we pass in. we can tell it to stride across the data, skip 3, get the next
    //   value, etc. We don't want to stride, it's tightly packed data, so 0 skip
    // - Offset, where data starts, in our case there's no reason to use offset. If you only want
    //   to draw part of the object you can start at a different place
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    // In the shader, we want to enable the usage of the location we mentioned above
    glEnableVertexAttribArray(0);
    
    // bind vbo to nothing (unbinding). Some people like to indent the bind from above to here
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    
    
    
    GLfloat colors[] = {
      1.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 1.0f
    };

    glGenBuffers(1, &VBO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
    // unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    
    
    
    
    
    // and unbind vao
    glBindVertexArray(0);
}

int main() {
    // Initialize glfw. Function returns false if fail
    if(!glfwInit()) {
        printf("GLFW initializeation failed");
        glfwTerminate();
        return 1;
    }

    // Sets properties of window we're going to create
    // Takes enums as first argument
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    
    // Set the profile to "core". How openGL treats code it's passed. Core is not backwards compatible
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Allow forard compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    // Create the window itself, and a pointer to the window
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Class", nullptr, nullptr);
    if(!window) {
        printf("GLFW window creation failed");
        glfwTerminate();
        return 1;
    }
    
    // Get buffer size information. We want to get
    // dimensions of area in the middle of the window,
    // that holds all the opengl data. the viewport, the
    // part that's getting drawn to
    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight); // basically required for mac, on mac with retina display, we'll get actual window size for our particular mac
    
    // Set context for glew to use
    glfwMakeContextCurrent(window);
    
    // Enables us to access extensions, even though
    // this course wont' use them
    glewExperimental = GL_TRUE;
    
    // create the viewport
    if(GLEW_OK != glewInit()) {
        printf("Failed to initialize GLEW");
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }
    
    // set where top left coords are, and width
    // and height, which is the section in the middle,
    // defined by width we got out of buffer size.
    // This sets up our *viewport*
    glViewport(0, 0, screenWidth, screenHeight);
    
    createTriangle();
    // See https://stackoverflow.com/questions/54181078/opengl-3-3-mac-error-validating-program-validation-failed-no-vertex-array-ob
    // It looks like when validate hapens, it validates against existing opengl context, and since we unbound the vao, it wont' be in the context lol
    glBindVertexArray(VAO);
    compileShaders();
    
    // "While the window is open, continue this game
    // loop". it knows based on a value hidden inside
    // glfw
    while(!glfwWindowShouldClose(window)) {
        // Get and handle user input events. Not
        // strictly an opengl thing, a glfw thing
        glfwPollEvents();
        
        triOffset = triOffset + (direction ? 1 : -1) * triIncrement;
        if(abs(triOffset) >= triMaxOffset) {
            direction = !direction;
        }
        
        // gl, not glfw!
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        
        // Each pixel has more than just a color,
        // it has things like depth. We need to specify
        // which thing we want to clear, in this case,
        // color
        glClear(GL_COLOR_BUFFER_BIT); // Clears screen
        
        // Goes to graphics card and says to use the id with this ID. For multiple shaders
        // what you normally do is draw everything with the same shader, draw, then reassign shader
        glUseProgram(shader);
        
        // Create an identity matrix
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(triOffset, 0.0f, 0.0f));
        
        // Now set the uniform to the shader at the location (after attaching the shader)
        glUniformMatrix4fv(uniformModelMatrix, 1, GL_FALSE, glm::value_ptr(model));
        
        // We're working with this VAO now
        glBindVertexArray(VAO);
        
        // Can have triangles, lines (not color in middle), tris are filled in as middle, and
        // then where in the array should we start, and then how many points to draw
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        // Unbind veretx array
        glBindVertexArray(0);
        // Have to unbind after glUseProgram
        glUseProgram(0);
        
        // In opengl there are two scenes going on at
        // once. One that can't be seen that you're
        // drawing to, and then you swap them so it
        // can be scene. Now that we drew to a buffer,
        // swap to it so we can see it
        glfwSwapBuffers(window);
    }
    
    glfwTerminate();
    
    return 0;
}
