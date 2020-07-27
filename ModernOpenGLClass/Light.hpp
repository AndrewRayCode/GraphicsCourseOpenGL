//
//  Light.hpp
//  ModernOpenGLClass
//
//  Created by Andrew Ray on 6/30/20.
//  Copyright © 2020 Andrew Ray. All rights reserved.
//

#ifndef Light_hpp
#define Light_hpp

#include <stdio.h>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Light {
public:
    Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambientIntensity, GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat intensity);
    
    void useLight(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation, GLfloat diffuseIntensityLocation, GLfloat directionLocation);
    void setDirection(GLfloat xDir, GLfloat yDir, GLfloat zDir);
    
    ~Light();

private:
    glm::vec3 color, direction;
    GLfloat ambientIntensity, diffuseIntensity;
};

#endif /* Light_hpp */
