#include "..\include\CGeneric.h"  

CGeneric::CGeneric(){

}

CGeneric::~CGeneric(){


}
/*
void CGeneric::initialize(){

}*/

void CGeneric::Update(double timeDifference){

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
}
/*
CGeneric& CGeneric::operator =(const CGeneric& rhs){
  if(this == &rhs)
    return *this;

  go = rhs.go;

  return *this;
}
*/
void CGeneric::UpdatePlayerPosition(float x, float y){
  
}