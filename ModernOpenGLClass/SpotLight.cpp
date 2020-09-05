#include "SpotLight.hpp"

SpotLight::SpotLight() {}

SpotLight::SpotLight(
              GLfloat red, GLfloat green, GLfloat blue, GLfloat ai, GLfloat di,
              GLfloat x, GLfloat y, GLfloat z,
              GLfloat xDir, GLfloat yDir, GLfloat zDir,
              GLfloat constant, GLfloat linear, GLfloat exponent,
             GLfloat edg) : PointLight(red, green, blue, ai, di, x, y, z, constant, linear, exponent) {
    position = glm::normalize(glm::vec3(x, y, z));
    direction = glm::vec3(xDir, yDir, zDir);
    edge = edg;
    processedEdge = cosf(glm::radians(edge));
}

void SpotLight::useLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation, GLuint diffuseIntensityLocation,
GLuint positionLocation, GLuint directionLocation, GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation, GLuint edgeLocation) {
    glUniform3f(ambientColorLocation, color.x, color.y, color.z);
    glUniform1f(ambientIntensityLocation, ambientIntensity);
    glUniform1f(diffuseIntensityLocation, diffuseIntensity);

    glUniform3f(positionLocation, position.x, position.y, position.z);
    glUniform1f(constantLocation, constant);
    glUniform1f(linearLocation, linear);
    glUniform1f(exponentLocation, exponent);
    
    // A spot light is a point light with a direction
    glUniform3f(directionLocation, direction.x, direction.y, direction.z);
    glUniform1f(edgeLocation, processedEdge);
}

void SpotLight::setFlash(glm::vec3 pos, glm::vec3 dir) {
    position = pos;
    direction = dir;
}

SpotLight::~SpotLight() {}
