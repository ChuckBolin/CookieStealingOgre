#include "..\include\CElf.h"  

CElf::CElf(){

}

CElf::~CElf(){


}
/*
void CElf::initialize(){

}*/

void CElf::Update(double timeDifference){

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
      }
    }
  }
  if(go.dying == true)
    return;

  //static float speed = 25;// + ( rand() % 5);
  float speed = go.speed;
  float dx = playerPos.x - go.pos.x;
  float dy = playerPos.y - go.pos.y;
  float range = sqrt(dy * dy + dx * dx);

  if(range > 500){
    speed = 0;
    return;
  }

  //update position
  if(dx > 0)
    go.pos.x += timeDifference * speed;
  else if(dx < 0)
    go.pos.x -= timeDifference * speed;

  if(dy > 0)
    go.pos.y += timeDifference * speed;
  else if(dy < 0)
    go.pos.y -= timeDifference * speed;


  //determine angle
  //float lastAngle = go.angle;

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
CElf& CElf::operator =(const CElf& rhs){
  if(this == &rhs)
    return *this;

  go = rhs.go;

  return *this;
}
*/

void CElf::UpdatePlayerPosition(float x, float y){
  playerPos.x = x;
  playerPos.y = y;
}