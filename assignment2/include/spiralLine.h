#include <iostream>
#include <vector>
#include <math.h>

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include <SOIL.h>

// Other includes
#include "shader.h"

#ifndef TEST_SPIRALLINE_H
#define TEST_SPIRALLINE_H

#define pi 3.1415926536


class SpiralLine{
private:
  GLfloat angleStep = static_cast<GLfloat>(pi / 8); // distance (angle) between two neighbour
  GLfloat Range; // compute & store the points in the range only
  std::vector<double> LengthLog; // r, in polar coordinate
  GLfloat InitAngle; // the first angle of the serial
  GLint StartAngleIndex; // the first point should be drawn in which direction

public:
  std::vector<glm::vec3> Positions; // points position
  glm::vec3 colorType[16]; // define the color of the points in every direction
  std::vector<int> Colors; // record the group the point belong to

  SpiralLine(GLfloat r=2.0f, GLfloat InitA= static_cast<GLfloat>(-2 * pi), GLint StartAI=0): Range(r), InitAngle(InitA), StartAngleIndex(StartAI){
    // color definition
    for (int i=0;i<5;i++){
      colorType[i].x = 1.0f-0.2f*i;
      colorType[i].y = 0.2f*i;
      colorType[i].z = 0.0f;
    }
    for (int i=1;i<6;i++){
      colorType[i+4].x = 0.0f;
      colorType[i+4].y = 1.0f-0.2f*i;
      colorType[i+4].z = 0.2f*i;
    }
    for (int i=1;i<6;i++){
      colorType[i+9].z = 1.0f-0.2f*i;
      colorType[i+9].x = 0.2f*i;
      colorType[i+9].y = 0.1f;
    }
    colorType[15].x = 1.0f;
    colorType[15].y = 0.1f;
    colorType[15].z = 0.1f;
  }

  void logarithmUpd(GLfloat deltaTime){
    GLfloat temp = 0;
    StartAngleIndex = -static_cast<GLint>(std::ceil(InitAngle / angleStep)); // find the first point of the spiral
    GLfloat angle = InitAngle + StartAngleIndex * angleStep; // angle of the first point in (0, angleStep)
    // when calculate the position, the index of the first angle is StartAngleIndex
    while (temp<Range){ // generate points in polar coordinate
      temp = std::exp(angle)/1000;
      LengthLog.push_back(temp);
      angle += angleStep;
    }
    InitAngle += deltaTime * 0.2;
    if (InitAngle > 0) // InitAngle in (-2pi, 2pi)
      InitAngle -= 2*pi;
    Positions.resize(LengthLog.size());
    Colors.resize(LengthLog.size());
  }

  void PositionUpdate(GLfloat deltaTime){
    glm::vec3 tempPos(0.0f,0.0f,0.0f);
    logarithmUpd(deltaTime);
    for (int i=0;i<LengthLog.size();i++){
      Positions[i].x = static_cast<float>(LengthLog[i] * std::cos(angleStep * (i+StartAngleIndex)));
      Positions[i].y = static_cast<float>(LengthLog[i] * std::sin(angleStep * (i+StartAngleIndex)));
      Positions[i].z = 0.0f;

      Colors[i] = (i+StartAngleIndex+16) % 16;
    }
    LengthLog.clear();
  }

};

#endif //TEST_SPIRALLINE_H
