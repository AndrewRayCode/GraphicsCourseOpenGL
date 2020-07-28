//
//  DirectionalLight.cpp
//  ModernOpenGLClass
//
//  Created by Andrew Ray on 6/30/20.
//  Copyright © 2020 Andrew Ray. All rights reserved.
//

#include "DirectionalLight.hpp"

DirectionalLight::DirectionalLight(
    GLfloat red, GLfloat green, GLfloat blue,
    GLfloat ambIntens, GLfloat intens,
    GLfloat xDir, GLfloat yDir, GLfloat zDir
) : Light(red, green, blue, ambIntens, intens) {
    direction = glm::vec3(xDir, yDir, zDir);
}

void DirectionalLight::useLight(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation, GLfloat diffuseIntensityLocation, GLfloat directionLocation) {
    glUniform3f(ambientColorLocation, color.x, color.y, color.z);
    glUniform1f(ambientIntensityLocation, ambientIntensity);
    
    glUniform3f(directionLocation, direction.x, direction.y, direction.z);
    glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}

void DirectionalLight::setDirection(GLfloat x, GLfloat y, GLfloat z) {
    direction = glm::vec3(x, y, z);
}

DirectionalLight::~DirectionalLight() {}
