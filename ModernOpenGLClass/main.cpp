#include <iostream>
#include <stdio.h>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Window dimensions
const GLint WIDTH = 800;
const GLint HEIGHT = 600;

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
        return 1;
    }
    
    glViewport(0, 0, screenWidth, screenHeight);
    
    // "While the window is open, continue this game loop"
    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear( GL_COLOR_BUFFER_BIT ); // Clears screen
        
        glfwSwapBuffers(window);
    }
    
    glfwTerminate();
    
    return 0;
}
