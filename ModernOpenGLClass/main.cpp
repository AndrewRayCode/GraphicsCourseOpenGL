#include <iostream>
#include <stdio.h>  // printf
#include <string.h> // strlen
#include <cmath> // math absolute
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

glm::mat4 model = glm::mat4(1.0f);

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include "Mesh.hpp"
#include "Shader.hpp"

std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;

// Window dimensions
const GLint WIDTH = 800;
const GLint HEIGHT = 600;

float curAngle = 0.0f;

// Convert degrees to radians. Multiply this by
// degrees to get radians
const float toRadians = 3.14159265f / 180.0f;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 1.0f;
float triIncrement = 0.005f;

// Vertex Shader lets us modify vertices and pass the result onto the fragment shader
static const char* vShader = "\n\
// Version of GLSL                  \n\
# version 330                       \n\
// Values from code below are passed into 'zero' in the shader. You can also make it search by name and not need layout. layout() is only supported since version 3.2 \n\
layout(location = 0) in vec3 pos;    \n\
layout(location = 1) in vec3 color;  \n\
// he uses 'v' to mean 'vertex'      \n\
out vec3 vColor;                     \n\
uniform mat4 modelMatrix;            \n\
uniform mat4 projectionMatrix;       \n\
void main() {                        \n\
vColor = color;                      \n\
// Example of swimming through 3d    \n\
//vColor = vec3(modelMatrix * vec4(pos, 1.0)); \n\
// Built in value ot the shader itself. Imagine there's an out vec4. It's built in, we can't see it. The values are calculated as the graphics pipeline goes, and this value is sent to the fragment shader. These are the final positions of the vertices on the screen. Z is how far away this thing is from us \n\
gl_Position = projectionMatrix * modelMatrix * vec4(pos, 1.0); \n\
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

// Create vertex array object - holds multiple VBOs
// Create vertex buffer object
// Usually have multiple vbos per vao
void createTriangle() {

    // by default in opengl, middle of screen is 0,0, and y is up/down,
    // x axis is left/right, and z is depth
    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,
        0.0f, 0.0f, 1.0f, // go into third dimension, new point added
        1.0, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };
    
    // The triangles defining the faces of the pyramid
    unsigned int indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };

    GLfloat colors[] = {
      1.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 1.0f,
      1.0f, 1.0f, 1.0f
    };
    
    Mesh *obj = new Mesh();
    obj->createMesh(vertices, indices, 12, 12, colors);
    meshList.push_back(obj);

    Mesh *obj2 = new Mesh();
    obj2->createMesh(vertices, indices, 12, 12, colors);
    meshList.push_back(obj2);
}

void createShaders() {
    Shader *shader1 = new Shader();
    shader1->createFromString(vShader, fShader);
    shaderList.push_back(shader1);
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
    
    // GL has a bunch of features turned off by default
    glEnable(GL_DEPTH_TEST);
    
    // set where top left coords are, and width
    // and height, which is the section in the middle,
    // defined by width we got out of buffer size.
    // This sets up our *viewport*
    glViewport(0, 0, screenWidth, screenHeight);
    
    // first view is up and down view angle
    glm::mat4 projectionMatrix = glm::perspective(45.0f, (GLfloat)(screenWidth / screenHeight), 0.1f, 100.0f);
    
    createTriangle();
    // See https://stackoverflow.com/questions/54181078/opengl-3-3-mac-error-validating-program-validation-failed-no-vertex-array-ob
    // It looks like when validate hapens, it validates against existing opengl context, and since we unbound the vao, it wont' be in the context lol
//    glBindVertexArray(VAO);
    createShaders();
    
    GLuint uniformProjectionMatrix = 0;
    GLuint uniformModelMatrix = 0;
    
    // "While the window is open, continue this game
    // loop". it knows based on a value hidden inside
    // glfw
    while(!glfwWindowShouldClose(window)) {
        // Get and handle user input events. Not
        // strictly an opengl thing, a glfw thing
        glfwPollEvents();
        
        triOffset = triOffset + (direction ? 1 : -1) * triIncrement * 2.0;
        if(abs(triOffset) >= triMaxOffset) {
            direction = !direction;
        }
        
        // gl, not glfw!
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        
        // Each pixel has more than just a color,
        // it has things like depth. We need to specify
        // which thing we want to clear, in this case,
        // color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clears screen
        
        // Goes to graphics card and says to use the id with this ID. For multiple shaders
        // what you normally do is draw everything with the same shader, draw, then reassign shader

        shaderList[0]->useShader();
        uniformProjectionMatrix = shaderList[0]->getProjectionLocation();
        uniformModelMatrix = shaderList[0]->getModelLocation();
        
        // Create an identity matrix
        glm::mat4 model = glm::mat4(1.0f);
        
        curAngle += 1.1f;
        
        // Order of transforms matters! Matricies are applied
        // in reverse order, so you want to rotate last so that
        // it gets applied first
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
        model = glm::rotate(model, toRadians * curAngle, glm::vec3(0.0, 1.0, 0.0));
//        model = glm::scale(model, glm::vec3(triOffset, triOffset, 1.0));

        // Now set the uniform to the shader at the location (after attaching the shader)
        glUniformMatrix4fv(uniformModelMatrix, 1, GL_FALSE, glm::value_ptr(model));
        
        glUniformMatrix4fv(uniformProjectionMatrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
               
        meshList[0]->renderMesh();
        
        
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -4.0f));
        glUniformMatrix4fv(uniformModelMatrix, 1, GL_FALSE, glm::value_ptr(model));
        meshList[1]->renderMesh();

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
