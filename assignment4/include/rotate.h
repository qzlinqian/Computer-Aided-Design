#pragma once

// Std. Includes
#include <vector>

// GL Includes
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//
// Created by Qian Lin on 2018/11/19.
// Did not realize self rotate

#ifndef ASSIGNMENT4_ROTATE_H
#define ASSIGNMENT4_ROTATE_H

class Rotation{
public:
  std::vector<glm::vec3> Positions;
  std::vector<GLfloat> Radius;
//  GLfloat selfRotateSpeed[10];
  GLfloat orbitRotateSpeed[10];
//  GLfloat selfAngle[10];
  GLfloat orbitAngle[10];

  Rotation(){
    // Position init, no need to change afterwards, just update the angle
    Positions.emplace_back(glm::vec3(0.0f, 0.0f, 0.0f));  // Sun
    Positions.emplace_back(glm::vec3(0.2f, 0.0f, 0.0f));  // Mercury
    Positions.emplace_back(glm::vec3(0.4f, 0.0f, 0.0f));  // Venus
    Positions.emplace_back(glm::vec3(0.6f, 0.0f, 0.0f));  // Earth
    Positions.emplace_back(glm::vec3(0.04f, 0.0f, 0.0f));  // Moon! the deviation from earth!
    // Attention here! Moon should translate by the deviation of the Earth first,
    // and then rotate by its orbit angle, translate by the Earth's distance with sun, rotate by Earth's orbit angle
    Positions.emplace_back(glm::vec3(0.8f, 0.0f, 0.0f));  // Mars
    Positions.emplace_back(glm::vec3(1.0f, 0.0f, 0.0f));  // Jupiter
    Positions.emplace_back(glm::vec3(1.2f, 0.0f, 0.0f));  // Saturn
    Positions.emplace_back(glm::vec3(1.4f, 0.0f, 0.0f));  // Uranus
    Positions.emplace_back(glm::vec3(1.6f, 0.0f, 0.0f));  // Neptune

    // Radius init, should not be modified afterwards
    Radius.emplace_back(0.2f);
    Radius.emplace_back(0.01f);
    Radius.emplace_back(0.02f);
    Radius.emplace_back(0.025f);
    Radius.emplace_back(0.005f);
    Radius.emplace_back(0.02f);
    Radius.emplace_back(0.04f);
    Radius.emplace_back(0.03f);
    Radius.emplace_back(0.02f);
    Radius.emplace_back(0.01f);

    /*selfRotateSpeed[0] = 1.0f;
    selfRotateSpeed[1] = 4.0f;
    selfRotateSpeed[2] = 3.0f;
    selfRotateSpeed[3] = 2.0f;
    selfRotateSpeed[4] = 0.5f;  // the two speed of moon equals
    selfRotateSpeed[5] = 4.0f;
    selfRotateSpeed[6] = 4.5f;
    selfRotateSpeed[7] = 2.5f;
    selfRotateSpeed[8] = 3.2f;
    selfRotateSpeed[9] = 1.0f;*/

    orbitRotateSpeed[0] = 0.0f;  // sun does not move
    orbitRotateSpeed[1] = 4.0f;
    orbitRotateSpeed[2] = 3.5f;
    orbitRotateSpeed[3] = 3.0f;
    orbitRotateSpeed[4] = 20.0f;
    orbitRotateSpeed[5] = 4.0f;
    orbitRotateSpeed[6] = 1.0f;
    orbitRotateSpeed[7] = 2.0f;
    orbitRotateSpeed[8] = 1.5f;
    orbitRotateSpeed[9] = 2.3f;

    for (int i=0; i<10; i++){
//      selfAngle[i] = 0;
      orbitAngle[i] = 0.0f;
    }
  }

  void PositionUpdate(GLfloat deltaTime){
    for (int i=0; i<10; i++){
//      selfAngle[i] += selfRotateSpeed[i] * deltaTime;
      orbitAngle[i] += orbitRotateSpeed[i] * deltaTime;

//      if (selfAngle[i] > 360.0f)
//        selfAngle[i] -= 360.0f;
      if (orbitAngle[i] > 360.0f)
        orbitAngle[i] -= 360.0f;
    }
  }
};

#endif //ASSIGNMENT4_ROTATE_H
