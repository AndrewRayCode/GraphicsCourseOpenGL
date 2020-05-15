//
//  Shader.hpp
//  ModernOpenGLClass
//
//  Created by Andrew Ray on 5/14/20.
//  Copyright © 2020 Andrew Ray. All rights reserved.
//

#ifndef Shader_hpp
#define Shader_hpp

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL/glew.h>

class Shader
{
public:
    Shader();
    
    void createFromString(const char* vertexCode, const char* fragmentCode);
    void useShader();
    void clearShader();

    GLuint getProjectionLocation();
    GLuint getModelLocation();
    
    ~Shader();
private:
    GLuint shaderID, uniformProjectionMatrix, uniformModelMatrix;
    
    void compileShader();
    void addShader(GLuint program, const char* code, GLenum shaderType);
};

#endif /* Shader_hpp */
