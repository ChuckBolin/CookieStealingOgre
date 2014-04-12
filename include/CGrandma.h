#ifndef GRANDMA_H
#define GRANDMA_H

#include "CObject.h"

class CGrandma : public CObject{
public:
  CGrandma();
  ~CGrandma();
  //void initialize();
  void Update(double timeDifference);
  //CLandscape& CLandscape::operator =(const CLandscape& rhs);
  void UpdatePlayerPosition(float x, float y);
};


#endif