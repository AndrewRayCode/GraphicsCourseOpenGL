//
//  Window.hpp
//  ModernOpenGLClass
//
//  Created by Andrew Ray on 5/14/20.
//  Copyright © 2020 Andrew Ray. All rights reserved.
//

#ifndef Window_hpp
#define Window_hpp

#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
public:
    Window();
    Window(GLint width, GLint height);

    int initialize();
    GLint getBufferWidth() {
        return bufferWidth;
    }
    GLint getBufferHeight() {
        return bufferHeight;
    }

    bool getShouldClose() {
        return glfwWindowShouldClose(window);
    }

    // In opengl there are two scenes going on at
    // once. One that can't be seen that you're
    // drawing to, and then you swap them so it
    // can be scene. Now that we drew to a buffer,
    // swap to it so we can see it
    void swapBuffers() { glfwSwapBuffers(window); }
    bool* getKeys();
    
    GLfloat getXChange();
    GLfloat getYChange();

    ~Window();

private:
    GLFWwindow *window;
    GLint width, height;
    GLint bufferWidth, bufferHeight;
    GLfloat lastX, lastY, xChange, yChange;
    bool mouseFirstMoved = true;
    
    // range of ascii characters
    bool keys[1024];
    
    void createCallbacks();
    static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
    static void handleMouse(GLFWwindow* window, double x, double y);
};


#endif /* Window_hpp */
