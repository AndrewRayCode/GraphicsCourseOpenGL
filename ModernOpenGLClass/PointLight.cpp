//
//  PointLight.cpp
//  ModernOpenGLClass
//
//  Created by Andrew Ray on 6/30/20.
//  Copyright © 2020 Andrew Ray. All rights reserved.
//

#include "PointLight.hpp"

PointLight::PointLight() {}

PointLight::PointLight(
    GLfloat red, GLfloat green, GLfloat blue, GLfloat ai, GLfloat di,
    GLfloat x, GLfloat y, GLfloat z,
    GLfloat c, GLfloat l, GLfloat e
) : Light(red, green, blue, ai, di) {
    position = glm::vec3(x, y, z);
    constant = c;
    linear = l;
    exponent = e;
}

void PointLight::useLight(
    GLfloat ambientIntensityLocation, GLfloat ambientColorLocation, GLfloat diffuseIntensityLocation,
    GLfloat positionLocation, GLfloat constantLocation, GLfloat linearLocation, GLfloat exponentLocation
) {
    glUniform3f(ambientColorLocation, color.x, color.y, color.z);
    glUniform1f(ambientIntensityLocation, ambientIntensity);
    glUniform1f(diffuseIntensityLocation, diffuseIntensity);

    glUniform3f(positionLocation, position.x, position.y, position.z);
    glUniform1f(constantLocation, constant);
    glUniform1f(linearLocation, linear);
    glUniform1f(exponentLocation, exponent);
}

PointLight::~PointLight() {}
 
