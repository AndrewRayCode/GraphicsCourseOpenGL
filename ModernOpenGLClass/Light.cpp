//
//  Light.cpp
//  ModernOpenGLClass
//
//  Created by Andrew Ray on 6/30/20.
//  Copyright © 2020 Andrew Ray. All rights reserved.
//

#include "Light.hpp"

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat ai, GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat di) {
    diffuseIntensity = di;
    color = glm::vec3(red, green, blue);
    direction = glm::vec3(xDir, yDir, zDir);
    ambientIntensity = ai;
}

void Light::useLight(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation, GLfloat diffuseIntensityLocation, GLfloat directionLocation) {
    glUniform3f(ambientColorLocation, color.x, color.y, color.z);
    glUniform1f(ambientIntensityLocation, ambientIntensity);
    
    glUniform3f(directionLocation, direction.x, direction.y, direction.z);
    glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}

void Light::setDirection(GLfloat xDir, GLfloat yDir, GLfloat zDir) {
    direction = glm::vec3(xDir, yDir, zDir);
}

Light::~Light() {}
