#include <iostream>
#include <stdlib.h>
#include <stdio.h>  // printf
#include <string.h> // strlen
#include <cmath> // math absolute
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include "Window.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Texture.hpp"

glm::mat4 model = glm::mat4(1.0f);

static const char* vShader = "shaders/shader.vertex";
static const char* fShader = "shaders/shader.fragment";

char const *brick = "textures/brick.png";
char const *dirt = "textures/dirt.png";

std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;

Texture brickTexture;
Texture dirtTexture;

float curAngle = 0.0f;

// Convert degrees to radians. Multiply this by
// degrees to get radians
const float toRadians = 3.14159265f / 180.0f;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 1.0f;
float triIncrement = 0.005f;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

// Create objects in our scene
void createMeshes() {

    // by default in opengl, middle of screen is 0,0, and y is up/down,
    // x axis is left/right, and z is depth
    GLfloat vertices[] = {
        // x,  y,     z     u,    v
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f,   0.5f, 0.0f, // go into third dimension, new point added
        1.0, -1.0f, 0.0f,   1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,   0.5f, 1.0f
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
    obj->createMesh(vertices, indices, 20, 12, colors);
    meshList.push_back(obj);
    
    Mesh *obj2 = new Mesh();
    obj2->createMesh(vertices, indices, 20, 12, colors);
    meshList.push_back(obj2);
}

void createShaders() {
    Shader *shader1 = new Shader();
    shader1->createFromFiles(vShader, fShader);
    shaderList.push_back(shader1);
}

int main() {
    Window window = Window(800, 600);
    window.initialize();

    // first view is up and down view angle
    glm::mat4 projectionMatrix = glm::perspective(45.0f, (GLfloat)window.getBufferWidth() / (GLfloat)window.getBufferHeight(), 0.1f, 100.0f);
    
    createMeshes();
    // See https://stackoverflow.com/questions/54181078/opengl-3-3-mac-error-validating-program-validation-failed-no-vertex-array-ob
    // It looks like when validate hapens, it validates against existing opengl context, and since we unbound the vao, it wont' be in the context lol
//    glBindVertexArray(VAO);
    createShaders();
    
    GLuint uniformProjectionMatrix = 0;
    GLuint uniformModelMatrix = 0;
    GLuint uniformViewMatrix = 0;
    GLuint uniformImage = 0;
    
    Camera *camera = new Camera(
       glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f,
                                0.0f, 5.0f, 0.4f
    );
    
    brickTexture = Texture(brick);
    brickTexture.loadTexture();

    dirtTexture = Texture(dirt);
    dirtTexture.loadTexture();
    
    lastTime = glfwGetTime();
    
    // "While the window is open, continue this game
    // loop". it knows based on a value hidden inside
    // glfw
    while(!window.getShouldClose()) {
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

        // Get and handle user input events. Not
        // strictly an opengl thing, a glfw thing
        glfwPollEvents();
        
        camera->keyControl(window.getKeys(), deltaTime);
        camera->mouseControl(window.getXChange(), window.getYChange());
        
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
        uniformViewMatrix = shaderList[0]->getViewLocation();
        uniformImage = shaderList[0]->getImageLocation();
        
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
        
        glUniformMatrix4fv(uniformViewMatrix, 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));

        brickTexture.useTexture();
        meshList[0]->renderMesh();
        
        
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -4.0f));
        glUniformMatrix4fv(uniformModelMatrix, 1, GL_FALSE, glm::value_ptr(model));
        dirtTexture.useTexture();
        meshList[1]->renderMesh();

        // Have to unbind after glUseProgram
        glUseProgram(0);
        
        window.swapBuffers();
    }
    
    return 0;
}
