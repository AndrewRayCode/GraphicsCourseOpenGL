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
#include "Constants.h"
#include "DirectionalLight.hpp"
#include "PointLight.hpp"

class Shader
{
public:
    Shader();
    
    void createFromString(const char* vertexCode, const char* fragmentCode);
    void createFromFiles(const char* vertexLocation, const char* fragmentLocation);
    std::string readFile(const char* location);
    void useShader();
    void clearShader();
    
    void setDirectionalLight(DirectionalLight * dLight);
    void setPointLights(PointLight * dLight, unsigned int lightCount);

    GLuint getProjectionLocation();
    GLuint getModelLocation();
    GLuint getViewLocation();
    GLuint getImageLocation();
    GLuint getAmbientIntensityLocation();
    GLuint getAmbientColorLocation();
    GLuint getDirectionLocation();
    GLuint getDiffuseIntensityLocation();
    GLuint getUniformEyePositionLocation();
    GLuint getUniformSpecularIntensityLocation();
    GLuint getUniformShininessLocation();
    
    ~Shader();
private:
    int pointLightCount;

    GLuint shaderID, uniformProjectionMatrix, uniformModelMatrix, uniformViewMatrix, uniformImage, uniformEyePosition, uniformSpecularIntensity, uniformShininess;
    
    struct {
        GLuint uniformColor;
        GLuint uniformAmbientIntensity;
        GLuint uniformDiffuseIntensity;
        GLuint uniformDirection;
    } uniformDirectaionlLight;

    GLuint uniformPointLightCount;

    struct {
        GLuint uniformColor;
        GLuint uniformAmbientIntensity;
        GLuint uniformDiffuseIntensity;
        GLuint uniformPosition;
        GLuint uniformConstant;
        GLuint uniformLinear;
        GLuint uniformExponent;
    } uniformPointLight[MAX_POINT_LIGHTS];
    
    void compileShader();
    void addShader(GLuint program, const char* code, GLenum shaderType);
};

#endif /* Shader_hpp */
