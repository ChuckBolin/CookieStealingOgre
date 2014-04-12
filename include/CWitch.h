#ifndef WITCH_H
#define WITCH_H

#include "CObject.h"

class CWitch : public CObject{
public:
  CWitch();
  ~CWitch();
  //void initialize();
  void Update(double timeDifference);
  //CLandscape& CLandscape::operator =(const CLandscape& rhs);
  void UpdatePlayerPosition(float x, float y);

};


#endif