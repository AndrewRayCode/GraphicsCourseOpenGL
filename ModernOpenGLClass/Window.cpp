//
//  Window.cpp
//  ModernOpenGLClass
//
//  Created by Andrew Ray on 5/14/20.
//  Copyright © 2020 Andrew Ray. All rights reserved.
//

#include "Window.hpp"

Window::Window() {
    width = 800;
    height = 600;
    
    for(size_t i = 0; i < 1024; i++) {
        keys[i] = 0;
    }
};

Window::Window(GLint _width, GLint _height) {
    width = _width;
    height = _height;

    xChange = 0;
    yChange = 0;
    
    for(size_t i = 0; i < 1024; i++) {
        keys[i] = 0;
    }
}

int Window::initialize() {
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
    window = glfwCreateWindow(width, height, "OpenGL Class", nullptr, nullptr);
    if(!window) {
        printf("GLFW window creation failed");
        glfwTerminate();
        return 1;
    }
    
    // Get buffer size information. We want to get
    // dimensions of area in the middle of the window,
    // that holds all the opengl data. the viewport, the
    // part that's getting drawn to
    glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight); // basically required for mac, on mac with retina display, we'll get actual window size for our particular mac
    
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
    glViewport(0, 0, bufferWidth, bufferHeight);
    
    glfwSetWindowUserPointer(window, this);
    createCallbacks();
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    return 0;
}

void Window::createCallbacks() {
    glfwSetKeyCallback(window, handleKeys);
    glfwSetCursorPosCallback(window, handleMouse);
}

bool* Window::getKeys() {
    return keys;
}

GLfloat Window::getXChange() {
    GLfloat x = xChange;
    xChange = 0.0f;
    return x;
}

GLfloat Window::getYChange() {
    GLfloat y = yChange;
    yChange = 0.0f;
    return y;
}

void Window::handleMouse(GLFWwindow* window, double x, double y) {
    Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
    
    if(self->mouseFirstMoved) {
        self->lastX = x;
        self->lastY = y;
        self->mouseFirstMoved = false;
    }

    self->xChange = x - self->lastX;
    self->yChange = self->lastY - y;

    self->lastX = x;
    self->lastY = y;
}

void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode) {
    Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
    
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    
    if(key >= 0 && key < 1014) {
        if(action == GLFW_PRESS) {
            self->keys[key] = true;
        }
        if(action == GLFW_RELEASE) {
            self->keys[key] = false;
        }
    }
}

Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
}
