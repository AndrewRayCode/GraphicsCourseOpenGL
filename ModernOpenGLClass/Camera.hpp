#include <stdio.h>
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

class Camera {
public:
    Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch, GLfloat movementSpeed, GLfloat turnSpeed);
    
    void keyControl(bool* keys, GLfloat deltaTime);
    void mouseControl(GLfloat xChange, GLfloat yChange);
    
    glm::mat4 calculateViewMatrix();
    glm::vec3 getPosition();
    
    ~Camera();
private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    // Where the sky is
    glm::vec3 worldUp;
    
    // Roll not needed for FPS style camera
    GLfloat yaw; // makes you look left and right, rotating around neck (y axis)
    GLfloat pitch; // looking up and down (pitching your head)
    
    GLfloat movementSpeed;
    GLfloat turnSpeed;
    
    void update();
};
