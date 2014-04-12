#include "..\include\CCharacter.h"  

CCharacter::CCharacter(){

}

CCharacter::~CCharacter(){


}
/*
void CCharacter::initialize(){

}*/

void CCharacter::Update(double timeDifference){
  go.maxFrames = 6;// frameCount = 20;
  return;

  if(go.maxFrames > 1){
    go.totalTime += timeDifference;
    if(go.totalTime > go.updateInterval){
      go.totalTime = 0;
      go.frameCount++;
      if(go.frameCount > go.maxFrames - 1)
        go.frameCount = 0;
    }
  }
}
/*
CCharacter& CCharacter::operator =(const CCharacter& rhs){
  if(this == &rhs)
    return *this;

  go = rhs.go;

  return *this;
}
*/