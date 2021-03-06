//
//  Shader.cpp
//  ModernOpenGLClass
//
//  Created by Andrew Ray on 5/14/20.
//  Copyright © 2020 Andrew Ray. All rights reserved.
//

#include "Shader.hpp"

Shader::Shader() {
    shaderID = 0;
    uniformModelMatrix = 0;
    uniformProjectionMatrix = 0;
    uniformViewMatrix = 0;
    uniformImage = 0;
    pointLightCount = 0;
    spotLightCount = 0;
}

void Shader::useShader() {
    glUseProgram(shaderID);
}

void Shader::clearShader() {
    if(shaderID != 0) {
        glDeleteProgram(shaderID);
        shaderID = 0;
    }
    
    uniformModelMatrix = 0;
    uniformProjectionMatrix = 0;
}

void Shader::addShader(GLuint program, const char* code, GLenum shaderType) {
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

void Shader::createFromFiles(const char* vertexLocation, const char* fragmentLocation) {
    std::string v = readFile(vertexLocation);
    std::string f = readFile(fragmentLocation);
    const char* vertexCode = v.c_str();
    const char* fragmentCode = f.c_str();
    
    createFromString(vertexCode, fragmentCode);
}

std::string Shader::readFile(const char* location) {
    std::string content;
    std::ifstream fileStream(location, std::ios::in);

    if(!fileStream.is_open()) {
        printf("Failed to load %s\n", location);
        return "";
    }
    
    std::string line = "";
    while(!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }
    
    fileStream.close();

    return content;
}

void Shader::createFromString(const char* vertexCode, const char* fragmentCode) {
    
    // stores shader program location in graphics card memory as ID
    shaderID = glCreateProgram();
    if(!shaderID) {
        printf("Error creating shader program");
        return;
    }

    addShader(shaderID, vertexCode, GL_VERTEX_SHADER);
    addShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

    // Get error codes from shaders, because we now need to validate and link programs
    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    // Create executables on graphics card and link them together. This is after we're finished
    // creating the shader. we have to then check it linked properly and validated
    glLinkProgram(shaderID);

    // Get link status, if it's false, get the info log
    glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
    if(!result) {
        // specify size so it doesn't overflow
        glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
        printf("error linking program: %s", eLog);
    }

    // Check if the shader is valid in the current context
    //    glValidateProgram(shader);
    //    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
    //    if(!result) {
    //         specify size so it doesn't overflow
    //        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
    //        printf("error validating program: %s", eLog);
    //    }

    // Get ID of uniform variable in compiled shader (by name)
    uniformModelMatrix = glGetUniformLocation(shaderID, "modelMatrix");
    uniformProjectionMatrix = glGetUniformLocation(shaderID, "projectionMatrix");
    uniformViewMatrix = glGetUniformLocation(shaderID, "viewMatrix");
    uniformImage = glGetUniformLocation(shaderID, "image");
    
    uniformDirectaionlLight.uniformColor = glGetUniformLocation(shaderID, "directionalLight.base.color");
    uniformDirectaionlLight.uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.base.ambientIntensity");
    uniformDirectaionlLight.uniformDirection = glGetUniformLocation(shaderID, "directionalLight.direction");
    uniformDirectaionlLight.uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.base.diffuseIntensity");

    uniformEyePosition = glGetUniformLocation(shaderID, "eyePosition");
    uniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
    uniformShininess = glGetUniformLocation(shaderID, "material.shininess");
    
    // Point lights

    uniformPointLightCount = glGetUniformLocation(shaderID, "pointLightCount");
    
    for(size_t i = 0; i < MAX_POINT_LIGHTS; i++) {
        char locBuff[100] = {'\0'};
        int idx = (int)i;

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.color", idx);
        uniformPointLight[i].uniformColor = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.ambientIntensity", idx);
        uniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);
        
        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.diffuseIntensity", idx);
        uniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].position", idx);
        uniformPointLight[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].constant", idx);
        uniformPointLight[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].linear", idx);
        uniformPointLight[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].exponent", idx);
        uniformPointLight[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);
    }
    
    // Spot Lights

    uniformSpotLightCount = glGetUniformLocation(shaderID, "spotLightCount");
    
    for(size_t i = 0; i < MAX_SPOT_LIGHTS; i++) {
        char locBuff[100] = {'\0'};
        int idx = (int)i;

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.color", idx);
        uniformSpotLight[i].uniformColor = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.ambientIntensity", idx);
        uniformSpotLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);
        
        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.diffuseIntensity", idx);
        uniformSpotLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.position", idx);
        uniformSpotLight[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.constant", idx);
        uniformSpotLight[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.linear", idx);
        uniformSpotLight[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.exponent", idx);
        uniformSpotLight[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].direction", idx);
        uniformSpotLight[i].uniformDirection = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].edge", idx);
        uniformSpotLight[i].uniformEdge = glGetUniformLocation(shaderID, locBuff);
    }
}

void Shader::setDirectionalLight(DirectionalLight * dLight) {
    dLight->useLight(
                     uniformDirectaionlLight.uniformAmbientIntensity,
                     uniformDirectaionlLight.uniformColor,
                     uniformDirectaionlLight.uniformDiffuseIntensity,
                     uniformDirectaionlLight.uniformDirection);
}

void Shader::setPointLights(PointLight * pLight, unsigned int lightCount) {
    glUniform1i(uniformPointLightCount, lightCount);
    for(size_t i = 0; i < lightCount; i++) {
        pLight[i].useLight(
           uniformPointLight[i].uniformAmbientIntensity,
           uniformPointLight[i].uniformColor,
           uniformPointLight[i].uniformDiffuseIntensity,
           uniformPointLight[i].uniformPosition,
           uniformPointLight[i].uniformConstant,
           uniformPointLight[i].uniformLinear,
           uniformPointLight[i].uniformExponent
       );
    }
}

void Shader::setSpotLights(SpotLight * sLight, unsigned int lightCount) {
    glUniform1i(uniformSpotLightCount, lightCount);
    for(size_t i = 0; i < lightCount; i++) {
        sLight[i].useLight(
           uniformSpotLight[i].uniformAmbientIntensity,
           uniformSpotLight[i].uniformColor,
           uniformSpotLight[i].uniformDiffuseIntensity,
           uniformSpotLight[i].uniformPosition,
           uniformSpotLight[i].uniformDirection,
           uniformSpotLight[i].uniformConstant,
           uniformSpotLight[i].uniformLinear,
           uniformSpotLight[i].uniformExponent,
           uniformSpotLight[i].uniformEdge
       );
    }
}

GLuint Shader::getProjectionLocation() {
    return uniformProjectionMatrix;
}
GLuint Shader::getModelLocation() {
    return uniformModelMatrix;
}
GLuint Shader::getViewLocation() {
    return uniformViewMatrix;
}
GLuint Shader::getImageLocation() {
    return uniformImage;
}
GLuint Shader::getAmbientIntensityLocation() {
    return uniformDirectaionlLight.uniformAmbientIntensity;
}
GLuint Shader::getAmbientColorLocation() {
    return uniformDirectaionlLight.uniformColor;
}
GLuint Shader::getDiffuseIntensityLocation() {
    return uniformDirectaionlLight.uniformDiffuseIntensity;
}
GLuint Shader::getDirectionLocation() {
    return uniformDirectaionlLight.uniformDirection;
}
GLuint Shader::getUniformEyePositionLocation() {
    return uniformEyePosition;
}
GLuint Shader::getUniformSpecularIntensityLocation() {
    return uniformSpecularIntensity;
}
GLuint Shader::getUniformShininessLocation() {
    return uniformShininess;
}

Shader::~Shader() {
    clearShader();
}
