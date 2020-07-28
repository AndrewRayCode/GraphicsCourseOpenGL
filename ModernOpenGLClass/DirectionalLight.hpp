//
//  DirectionalLight.hpp
//  ModernOpenGLClass
//
//  Created by Andrew Ray on 6/30/20.
//  Copyright © 2020 Andrew Ray. All rights reserved.
//

#ifndef DirectionalLight_hpp
#define DirectionalLight_hpp

#include <stdio.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Light.hpp"

class DirectionalLight : public Light {
public:
    DirectionalLight(
                     GLfloat red, GLfloat green, GLfloat blue,
                     GLfloat ambientIntensity, GLfloat intensity,
                     GLfloat xDir, GLfloat yDir, GLfloat zDir);
    
    void useLight(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation, GLfloat diffuseIntensityLocation, GLfloat directionLocation);
    void setDirection(GLfloat x, GLfloat y, GLfloat z);

    ~DirectionalLight();

private:
    glm::vec3 direction;
};

#endif /* DirectionalLight */
