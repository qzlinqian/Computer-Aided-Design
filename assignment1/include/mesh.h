//
// Created by Qian Lin on 2018/10/29.
//
#pragma once

// Std. Includes
#include <vector>

// GL Includes
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#ifndef TEST_MESH_H
#define TEST_MESH_H

enum Obj_Movement{
    // rotate
    UPWARD,
    DOWNWARD,
    CLOCKWISE,
    ANTICLOCKWISE,
    LEFTWARD,
    RIGHTWARD,
    // translate
    LIFT,
    SINK,
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class MeshRotate{
public:
    GLfloat Yaw;
    GLfloat Pitch;
    GLfloat Roll;
    glm::vec3 Position;

    MeshRotate(glm::vec3 position=glm::vec3(0.0f,0.0f,0.0f), GLfloat yaw=0, GLfloat pitch=0, GLfloat roll=0){
        this->Pitch = pitch;
        this->Yaw = yaw;
        this->Roll = roll;
    }

    void ProcessKeyboard(Obj_Movement direction, GLfloat deltaTime){
        GLfloat deltaAngle = ANGLE_SPEED*deltaTime;
        GLfloat deltaLength = SPEED*deltaTime;
        if (direction == UPWARD)
            Pitch += deltaAngle;
        if (direction == DOWNWARD)
            Pitch -= deltaAngle;
        if (direction == CLOCKWISE)
            Roll -= deltaAngle;
        if (direction == ANTICLOCKWISE)
            Roll += deltaAngle;
        if (direction == LEFTWARD)
            Yaw += deltaAngle;
        if (direction == RIGHTWARD)
            Yaw -= deltaAngle;
        if (direction == LIFT)
            Position.z += deltaLength;
        if (direction == SINK)
            Position.z -= deltaLength;
        if (direction == FORWARD)
            Position.x -= deltaLength;
        if (direction == BACKWARD)
            Position.x += deltaLength;
        if (direction == LEFT)
            Position.y -= deltaLength;
        if (direction == RIGHT)
            Position.y += deltaLength;
    }


private:
    // default value
    const GLfloat ANGLE_SPEED = 10.0f;
    const GLfloat SPEED       = 3.0f;
};


#endif //TEST_MESH_H
