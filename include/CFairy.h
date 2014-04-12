#ifndef FAIRY_H
#define FAIRY_H

#include "CObject.h"

class CFairy : public CObject{
public:
  CFairy();
  ~CFairy();
  //void initialize();
  void Update(double timeDifference);
  //CLandscape& CLandscape::operator =(const CLandscape& rhs);
  void UpdatePlayerPosition(float x, float y);

};


#endif