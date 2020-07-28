//
//  PointLight.hpp
//  ModernOpenGLClass
//
//  Created by Andrew Ray on 6/30/20.
//  Copyright © 2020 Andrew Ray. All rights reserved.
//

#ifndef PointLight_hpp
#define PointLight_hpp

#include <stdio.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Light.hpp"

class PointLight : public Light {
public:
    PointLight();

    PointLight(
               GLfloat red, GLfloat green, GLfloat blue, GLfloat ai, GLfloat di,
               GLfloat x, GLfloat y, GLfloat z,
               GLfloat constant, GLfloat linear, GLfloat exponent);
    
    void useLight(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation, GLfloat diffuseIntensityLocation,
    GLfloat positionLocation, GLfloat constantLocation, GLfloat linearLocation, GLfloat exponentLocation);

    ~PointLight();

private:
    glm::vec3 position;
    GLfloat constant, linear, exponent;
};

#endif /* PointLight */
