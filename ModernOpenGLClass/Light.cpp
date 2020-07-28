//
//  Light.cpp
//  ModernOpenGLClass
//
//  Created by Andrew Ray on 6/30/20.
//  Copyright © 2020 Andrew Ray. All rights reserved.
//

#include "Light.hpp"

Light::Light() {}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat ai, GLfloat di) {
    color = glm::vec3(red, green, blue);
    ambientIntensity = ai;
    diffuseIntensity = di;
}

Light::~Light() {}
