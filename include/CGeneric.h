/*
  This class is used for 'generic' game objects that have no special update requirements. This
  includes items such as water, ground, walls, rock, trees, etc.
*/
#ifndef GENERIC_H
#define GENERIC_H

#include "CObject.h"

class CGeneric : public CObject{
public:
  CGeneric();
  ~CGeneric();
  //void initialize();
  void Update(double timeDifference);
  //CLandscape& CLandscape::operator =(const CLandscape& rhs);
  void UpdatePlayerPosition(float x, float y);
};


#endif