//
//  Material.cpp
//  ModernOpenGLClass
//
//  Created by Andrew Ray on 7/26/20.
//  Copyright © 2020 Andrew Ray. All rights reserved.
//

#include "Material.hpp"

Material::Material(GLfloat intensity, GLfloat shine) {
    specularIntensity = intensity;
    shininess = shine;
};

void Material::useMaterial(GLuint specularIntensityLocation, GLuint shininessLocation) {
    glUniform1f(specularIntensityLocation, specularIntensity);
    glUniform1f(shininessLocation, shininess);
}

Material::~Material() {};
