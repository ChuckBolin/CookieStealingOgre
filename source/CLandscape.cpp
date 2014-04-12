#include "..\include\CLandscape.h"  

CLandscape::CLandscape(){

}

CLandscape::~CLandscape(){


}
/*
void CLandscape::initialize(){

}*/

void CLandscape::Update(double timeDifference){
  static double timeDiff = 0;
  timeDiff += timeDifference;
  if(timeDiff > 0.1){
    go.left += 1;
    timeDiff = 0;
  }
  return;

  go.maxFrames = 8;// frameCount = 20;
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
CLandscape& CLandscape::operator =(const CLandscape& rhs){
  if(this == &rhs)
    return *this;

  go = rhs.go;

  return *this;
}
*/