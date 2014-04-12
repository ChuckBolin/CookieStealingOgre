#include "..\include\CFairy.h"  

CFairy::CFairy(){

}

CFairy::~CFairy(){


}
/*
void CFairy::initialize(){

}*/

void CFairy::Update(double timeDifference){
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

        //go.alive = false;//<<<<<<<<<<<<<
      }
    }
  }

  if(go.dying == true)
    return;

  //srand((unsigned)time(NULL));
  //static float offset = (float)(rand() % 10);
  float speed = go.speed;// 45 + offset;
  float dx = playerPos.x - go.pos.x - 5;
  float dy = playerPos.y - go.pos.y + 10;
  float range = sqrt(dy * dy + dx * dx);

  if(range > 500){
    speed = 0;
    return;
  }

  //update position
  if(dx >= 0)
    go.pos.x += timeDifference * speed;
  else
    go.pos.x -= timeDifference * speed;

  if(dy >= 0)
    go.pos.y += timeDifference * speed;
  else
    go.pos.y -= timeDifference * speed;

  //determine angle
  float lastAngle = go.angle;

  if(dx != 0){
    if(dx >= 0 && dy < 0)
      go.angle = 4.71 + atan(dy/dx);
    else if(dx >= 0 && dy >= 0)
      go.angle = 4.71 + atan(dy/dx); //
    else if(dx < 0 && dy > 0)
      go.angle = 1.57 + atan(dy/dx);
    else if(dx < 0 && dy < 0)
      go.angle = 1.57 + atan(dy/dx);
  }
  else{
    if(dy >= 0)
      go.angle = 3.14;
    else
      go.angle = 0;
  }

}
/*
CFairy& CFairy::operator =(const CFairy& rhs){
  if(this == &rhs)
    return *this;

  go = rhs.go;

  return *this;
}
*/
void CFairy::UpdatePlayerPosition(float x, float y){
  playerPos.x = x;
  playerPos.y = y;
}