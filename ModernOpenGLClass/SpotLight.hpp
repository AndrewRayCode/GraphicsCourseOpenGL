#ifndef SpotLight_hpp
#define SpotLight_hpp

#include <stdio.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "PointLight.hpp"

class SpotLight : public PointLight {
public:
    SpotLight();

    SpotLight(
              GLfloat red, GLfloat green, GLfloat blue, GLfloat ai, GLfloat di,
              GLfloat x, GLfloat y, GLfloat z,
              GLfloat xDir, GLfloat yDir, GLfloat zDir,
              GLfloat constant, GLfloat linear, GLfloat exponent,
              GLfloat edge);
    
    void useLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation, GLuint diffuseIntensityLocation,
    GLuint positionLocation, GLuint directionLocation, GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation, GLuint edgeLocation);

    void setFlash(glm::vec3 pos, glm::vec3 dir);

    ~SpotLight();

private:
    glm::vec3 direction; // which way the spotlight is facing
    GLfloat edge; // angle of the edge of the light
    GLfloat processedEdge;
};

#endif
