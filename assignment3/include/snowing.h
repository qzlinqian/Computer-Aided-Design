#include <cstdlib>
#include <vector>


#ifndef ASSINMENT3_SNOWING_H
#define ASSINMENT3_SNOWING_H

class Snowing{
public:
  std::vector<glm::vec3> Positions;
  std::vector<GLfloat> Scales;

  Snowing(int size=1000, GLfloat speed=0.1f):_size(size),_speed(speed),times(0){
//    long temp = random();
    for (int i=0;i<size/50;i++){
      addFlake(1);
    }
  }

  void addFlake(bool init){
    glm::vec3 newPos;
    newPos.x = (random() * 2.0f) / RAND_MAX - 1;
    newPos.y = init? (random() * 2.0f) /RAND_MAX - 1 : 1.1f;
    newPos.z = 0.0f;
    Positions.push_back(newPos);
    Scales.push_back((random()*1.0f)/RAND_MAX);
  }

  void positionUpdate(GLfloat deltaTime){
    for (int i=0;i<getFlakeSize();i++){
      Positions[i].y -= deltaTime * _speed;
      if (Positions[i].y < -1.2f){
        Positions[i].y += 2.4f;
        Scales[i] = (random()*1.0f)/RAND_MAX;
      }
    }
    times++;
    times %= 300;
    if (getFlakeSize() < _size && (times%300 == 0))
      addFlake(0);
  }

  unsigned long getFlakeSize(){
    return Positions.size();
  }

private:
  int _size;
  GLfloat _speed;
  int times;
};

#endif //ASSINMENT3_SNOWING_H
