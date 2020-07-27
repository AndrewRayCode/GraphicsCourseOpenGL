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
#include "Light.hpp"
#include "Material.hpp"

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

void calcNormals(unsigned int * indices, unsigned int indicesCount, GLfloat * vertices, unsigned int vertexCount, unsigned int columns /* vLength */, unsigned int normalOffset) {

    // Loop over each triangle
    for(size_t i = 0; i < indicesCount; i += 3) {
        // Each "row" in the vertice array has vertexLength size
        unsigned int in0 = indices[i] * columns;
        unsigned int in1 = indices[i + 1] * columns;
        unsigned int in2 = indices[i + 2] * columns;
        
        // We're going to draw two lines, of the triangle, then get the cross product of those
        // two lines, which is a line perpindicular to both, so it will give us the normal
        glm::vec3 v1(
             vertices[in1] - vertices[in0],
             vertices[in1 + 1] -  vertices[in0 + 1],
             vertices[in1 + 2] -  vertices[in0 + 2]
        );
        glm::vec3 v2(
            vertices[in2] - vertices[in0],
            vertices[in2 + 1] -  vertices[in0 + 1],
            vertices[in2 + 2] -  vertices[in0 + 2]
        );
        
        // Given the two line of our triange: /\ the normal is coming right out towards us
        glm::vec3 normal = glm::cross(v1, v2);
        normal = glm::normalize(normal);
        
        unsigned int n0 = in0 + normalOffset;
        unsigned int n1 = in1 + normalOffset;
        unsigned int n2 = in2 + normalOffset;
        vertices[n0] = normal.x;
        vertices[n0 + 1] = normal.x;
        vertices[n0 + 2] = normal.x;
        vertices[n1] = normal.y;
        vertices[n1 + 1] = normal.y;
        vertices[n1 + 2] = normal.y;
        vertices[n2] = normal.z;
        vertices[n2 + 1] = normal.z;
        vertices[n2 + 2] = normal.z;
    }
    
    // Go through each vertices, and normalize their normal values...why?
    for(size_t i = 0; i < vertexCount / columns; i++) {
        unsigned int nOffset = i * columns + normalOffset;
        glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
        vec = glm::normalize(vec);
        vertices[nOffset] = vec.x;
        vertices[nOffset] = vec.y;
        vertices[nOffset] = vec.z;
    }
}

void calcAverageNormals(unsigned int * indices, unsigned int indiceCount, GLfloat * vertices, unsigned int verticeCount,
                        unsigned int vLength, unsigned int normalOffset)
{
    for (size_t i = 0; i < indiceCount; i += 3)
    {
        unsigned int in0 = indices[i] * vLength;
        unsigned int in1 = indices[i + 1] * vLength;
        unsigned int in2 = indices[i + 2] * vLength;
        glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
        glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
        glm::vec3 normal = glm::cross(v1, v2);
        normal = glm::normalize(normal);

        in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
        vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
        vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
        vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
    }

    for (size_t i = 0; i < verticeCount / vLength; i++)
    {
        unsigned int nOffset = i * vLength + normalOffset;
        glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
        vec = glm::normalize(vec);
        vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
    }
}


// Create objects in our scene
void createMeshes() {

    // by default in opengl, middle of screen is 0,0, and y is up/down,
    // x axis is left/right, and z is depth
    GLfloat vertices[] = {
        // x,  y,     z       u,    v        nx,   ny,   nz
        -1.0f, -1.0f, -0.6f,  0.0f, 0.0f,    0.0f, 0.0f, 0.0f,
        0.0f,  -1.0f, 1.0f,   0.5f, 0.0f,    0.0f, 0.0f, 0.0f,
        1.0,   -1.0f, -0.6f,  1.0f, 0.0f,    0.0f, 0.0f, 0.0f,
        0.0f,   1.0f, 0.0f,   0.5f, 1.0f,    0.0f, 0.0f, 0.0f
    };
    
    // The triangles defining the faces of the pyramid
    unsigned int indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };

//    calcNormals(indices, 12, vertices, 32, 8, 5);
    calcAverageNormals(indices, 12, vertices, 32, 8, 5);

    GLfloat colors[] = {
      1.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 1.0f,
      1.0f, 1.0f, 1.0f
    };
    
    Mesh *obj = new Mesh();
    obj->createMesh(vertices, indices, 32, 12, colors);
    meshList.push_back(obj);
    
    Mesh *obj2 = new Mesh();
    obj2->createMesh(vertices, indices, 32, 12, colors);
    meshList.push_back(obj2);
}

void createShaders() {
    Shader *shader1 = new Shader();
    shader1->createFromFiles(vShader, fShader);
    shaderList.push_back(shader1);
}

int main() {
    Window window = Window(1366, 768);
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
    GLuint uniformEyePosition = 0;
    GLuint uniformSpecularIntensity = 0;
    GLuint uniformShininess = 0;
    
    Camera *camera = new Camera(
       glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f,
                                0.0f, 5.0f, 0.4f
    );
    
    brickTexture = Texture(brick);
    brickTexture.loadTexture();

    dirtTexture = Texture(dirt);
    dirtTexture.loadTexture();
    
    lastTime = glfwGetTime();
    
    Light directionalLight = Light(1.0f, 1.0f, 1.0f, 0.3f,
                                   2.0f, -1.0f, -2.0f, 0.3f);
    
    Material shinyMaterial = Material(1.0f, 32);
    Material dullMaterial = Material(1.0f, 4);
    
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
        glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
        
        // Each pixel has more than just a color,
        // it has things like depth. We need to specify
        // which thing we want to clear, in this case,
        // color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clears screen
        
        // Goes to graphics card and says to use the id with this ID. For multiple shaders
        // what you normally do is draw everything with the same shader, draw, then reassign shader

        shaderList[0]->useShader();
        directionalLight.useLight(
                           shaderList[0]->getAmbientIntensityLocation(),
                           shaderList[0]->getAmbientColorLocation(),
                           shaderList[0]->getDiffuseIntensityLocation(),
                           shaderList[0]->getDirectionLocation()
                           );
                           
        float angl = curAngle * 0.000;
//        directionalLight.setDirection(2.0 * glm::cos(angl), 2.0 * glm::sin(angl), -2.0f);
        uniformProjectionMatrix = shaderList[0]->getProjectionLocation();
        uniformModelMatrix = shaderList[0]->getModelLocation();
        uniformViewMatrix = shaderList[0]->getViewLocation();
        uniformImage = shaderList[0]->getImageLocation();

        uniformEyePosition = shaderList[0]->getUniformEyePositionLocation();
        uniformSpecularIntensity = shaderList[0]->getUniformSpecularIntensityLocation();
        uniformShininess = shaderList[0]->getUniformShininessLocation();

        glUniformMatrix4fv(uniformProjectionMatrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        glUniformMatrix4fv(uniformViewMatrix, 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));
        glUniform3f(uniformEyePosition, camera->getPosition().x, camera->getPosition().y, camera->getPosition().z);

        curAngle += 1.1f;
        
        // Order of transforms matters! Matricies are applied
        // in reverse order, so you want to rotate last so that
        // it gets applied first
//        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
//        model = glm::rotate(model, toRadians * curAngle, glm::vec3(0.0, 1.0, 0.0));
//        model = glm::scale(model, glm::vec3(triOffset, triOffset, 1.0));

        // - MESH RENDER 1 ----
        // Create an identity matrix
        glm::mat4 model = glm::mat4(1.0f);
        // Now set the uniform to the shader at the location (after attaching the shader)
        glUniformMatrix4fv(uniformModelMatrix, 1, GL_FALSE, glm::value_ptr(model));
        shinyMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
        brickTexture.useTexture();
        meshList[0]->renderMesh();
        
        // - MESH RENDER 2 ----
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -4.0f));
        glUniformMatrix4fv(uniformModelMatrix, 1, GL_FALSE, glm::value_ptr(model));
        dullMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
        dirtTexture.useTexture();
        meshList[1]->renderMesh();

        // Have to unbind after glUseProgram
        glUseProgram(0);
        
        window.swapBuffers();
    }
    
    return 0;
}
