#ifndef OGRE_H
#define OGRE_H

#include "CObject.h"

class COgre : public CObject{
public:
  COgre();
  ~COgre();
  //void initialize();
  void Update(double timeDifference);
  //CLandscape& CLandscape::operator =(const CLandscape& rhs);
  void UpdatePlayerPosition(float x, float y);
};


#endif