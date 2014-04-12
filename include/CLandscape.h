#ifndef LANDSCAPE_H
#define LANDSCAPE_H

#include "CObject.h"

class CLandscape : public CObject{
public:
  CLandscape();
  ~CLandscape();
  //void initialize();
  void Update(double timeDifference);
  //CLandscape& CLandscape::operator =(const CLandscape& rhs);

};


#endif