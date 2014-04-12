#ifndef CHARACTER_H
#define CHARACTER_H

#include "CObject.h"

class CCharacter : public CObject{
public:
  CCharacter();
  ~CCharacter();
  //void initialize();
  void Update(double timeDifference);
  //CCharacter& CCharacter::operator =(const CCharacter& rhs);
};


#endif