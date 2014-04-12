#include "..\include\CWitch.h"  

CWitch::CWitch(){

}

CWitch::~CWitch(){


}
/*
void CWitch::initialize(){

}*/

void CWitch::Update(double timeDifference){
  //update frame animation
  if(go.maxFrames > 1){
    go.totalTime += timeDifference;
    if(go.totalTime > go.updateInterval){
      go.totalTime = 0;
      go.frameCount++;
      if(go.frameCount > go.maxFrames - 1){
        go.frameCount = 0;
        if(go.dying == true)
          go.alive = false;

        //go.alive = false;//<<<<<<<<<<<<<<<<
      }
    }
  }
  
  if(go.dying == true)
    return;

  float speed = go.speed;
  float dx = playerPos.x - go.pos.x;
  float dy = playerPos.y - go.pos.y;

  //determine angle
  float lastAngle = go.angle;
  float angle = 0;
  static float dir = 0;

  if(dx != 0){
    if(dx >= 0 && dy < 0)
      angle = 4.71 + atan(dy/dx);
    else if(dx >= 0 && dy >= 0)
      angle = 4.71 + atan(dy/dx); //
    else if(dx < 0 && dy > 0)
      angle = 1.57 + atan(dy/dx);
    else if(dx < 0 && dy < 0)
      angle = 1.57 + atan(dy/dx);
  }
  else{
    if(dy >= 0)
      angle = 3.14;
    else
      angle = 0;
  }

  //update position
  //go.angle = angle;
  float range = sqrt(dy * dy + dx * dx);
  //dir = angle;// * timeDifference;
 /*
  srand((unsigned)time(NULL));
  static float sign = 0;
  if(sign == 0){
    if(rand() % 100 < 50)
      sign = -1;
    else
      sign = 1;
  }*/
  
  if(range < 100)
    angle += .25;
  else if(range < 300)
    angle += .45;
  else if(range < 500)
    angle += .65;
  else if(range < 700)
    angle += .85;
  else
    angle += 1;

  if(angle > 6.28)
    angle = 0;

  //angle -= .5;// * sign;//sign * range/(float)1000;
  if(range > 500){
    speed = 0;
    return;
  }

  float vx = speed * cos(angle) * timeDifference;
  float vy = speed * sin(angle) * timeDifference;
  /*
  if(vx != 0)
    go.angle = atan(-vy/vx);
  else
    go.angle = atan(-vy/.001);
  */

  go.angle = angle;
  go.pos.x += vx;
  go.pos.y += vy;

  /*
  if(dx >= 0)
    go.pos.x += timeDifference * speed * -tan(go.angle);
  else
    go.pos.x -= timeDifference * speed * -tan(go.angle);

  if(dy >= 0)
    go.pos.y += timeDifference * speed * -tan(go.angle);
  else
    go.pos.y -= timeDifference * speed * -tan(go.angle);
  */

}
/*
CWitch& CWitch::operator =(const CWitch& rhs){
  if(this == &rhs)
    return *this;

  go = rhs.go;

  return *this;
}
*/
void CWitch::UpdatePlayerPosition(float x, float y){
  playerPos.x = x;
  playerPos.y = y; 
}