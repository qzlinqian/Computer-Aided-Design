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
  RIGHT,
  R_CHANGE,
  G_CHANGE,
  B_CHANGE
};

class MeshRotate{
public:
  GLfloat Yaw;
  GLfloat Pitch;
  GLfloat Roll;
  glm::vec3 Position;
  glm::vec3 DisplayColor=glm::vec3(0.0f,0.0f,0.0f);

  MeshRotate(glm::vec3 position=glm::vec3(0.0f,0.0f,0.0f), GLfloat yaw=0, GLfloat pitch=0, GLfloat roll=0){
    this->Pitch = pitch;
    this->Yaw = yaw;
    this->Roll = roll;
  }

  void ProcessKeyboard(Obj_Movement direction, GLfloat deltaTime){
    GLfloat deltaAngle = ANGLE_SPEED*deltaTime;
    GLfloat deltaLength = SPEED*deltaTime;
    GLfloat deltaColor = COLOR*deltaTime;
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
      Position.y += deltaLength;
    if (direction == SINK)
      Position.y -= deltaLength;
    if (direction == FORWARD)
      Position.z -= deltaLength;
    if (direction == BACKWARD)
      Position.z += deltaLength;
    if (direction == LEFT)
      Position.x += deltaLength;
    if (direction == RIGHT)
      Position.x -= deltaLength;
    if (direction == R_CHANGE)
      DisplayColor.x += (DisplayColor.x+deltaColor > 1.0f)? deltaAngle-1.0f:deltaAngle;
    if (direction == G_CHANGE)
      DisplayColor.y += (DisplayColor.y+deltaColor > 1.0f)? deltaAngle-1.0f:deltaAngle;
    if (direction == B_CHANGE)
      DisplayColor.z += (DisplayColor.z+deltaColor > 1.0f)? deltaAngle-1.0f:deltaAngle;
  }


private:
  // default value
  const GLfloat ANGLE_SPEED = 10.0f;
  const GLfloat SPEED       = 3.0f;
  const GLfloat COLOR       = 0.000001f;
};


#endif //TEST_MESH_H
