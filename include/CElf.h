#ifndef ELF_H
#define ELF_H

#include "CObject.h"

class CElf : public CObject{
public:
  CElf();
  ~CElf();
  //void initialize();
  void Update(double timeDifference);
  //CLandscape& CLandscape::operator =(const CLandscape& rhs);
  void UpdatePlayerPosition(float x, float y);
};


#endif