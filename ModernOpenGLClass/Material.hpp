//
//  Material.hpp
//  ModernOpenGLClass
//
//  Created by Andrew Ray on 7/26/20.
//  Copyright © 2020 Andrew Ray. All rights reserved.
//

#ifndef Material_hpp
#define Material_hpp

#include <stdio.h>
#include <GL/glew.h>


class Material {
public:
    Material(GLfloat intensity, GLfloat shine);
    
    void useMaterial(GLuint specularIntensityLocation, GLuint shininessLocation);

    ~Material();

private:
    GLfloat specularIntensity;
    GLfloat shininess; // specular power. How smooth object is percieved
    
};

#endif /* Material_hpp */
