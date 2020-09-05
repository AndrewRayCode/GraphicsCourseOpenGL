//
//  Camera.cpp
//  ModernOpenGLClass
//
//  Created by Andrew Ray on 5/31/20.
//  Copyright © 2020 Andrew Ray. All rights reserved.
//

#include "Camera.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch, GLfloat movementSpeed, GLfloat turnSpeed) {
    this->position = position;
    this->worldUp = up;
    this->yaw = yaw;
    this->pitch = pitch;

    this->front = glm::vec3(0.0f, 0.0f, -1.0f);
    
    this->movementSpeed = movementSpeed;
    this->turnSpeed = turnSpeed;
    
    update();
}

void Camera::keyControl(bool *keys, GLfloat deltaTime) {
    // We want to use the front because we want to
    // move in the direction we're looking
    if(keys[GLFW_KEY_W]) {
        position += front * movementSpeed * deltaTime;
    }
    if(keys[GLFW_KEY_A]) {
        position -= right * movementSpeed * deltaTime;
    }
    if(keys[GLFW_KEY_S]) {
        position -= front * movementSpeed * deltaTime;
    }
    if(keys[GLFW_KEY_D]) {
        position += right * movementSpeed * deltaTime;
    }
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange) {
    xChange *= turnSpeed;
    yChange *= turnSpeed;

    yaw += xChange;
    pitch += yChange;
    pitch = glm::clamp(pitch, -89.0f, 89.0f);
    
    // If we've changed our pitch, front changes. If we've
    // looked up and down, our up has changed, so update
    update();
}

glm::mat4 Camera::calculateViewMatrix() {
    return glm::lookAt(position, position + front, up);
}

void Camera::update() {
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);
    
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

glm::vec3 Camera::getDirection() {
    return glm::normalize(front);
}

glm::vec3 Camera::getPosition() {
    return position;
}
