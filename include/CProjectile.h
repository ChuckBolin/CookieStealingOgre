#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "CObject.h"
#include <d3d9.h>
#include <d3dx9.h>
#include "CLog.h"

struct WEAPON_OBJECT{
  D3DXVECTOR3 pos;
  D3DXVECTOR3 vel;
  float range;       //fixed range of weapons
  float distance;    //actual distance traveled
  int weaponType; //0..5
  int spriteID;
  bool alive;     //true if exists
  float radius;   //effectiveness of weapon
  int damage;     //amount of damage to an object
  float dir;        //0=right, 1=up, 2=left,3=down
};


class CProjectile : public CObject{
public:
  CProjectile();
  ~CProjectile();
  //void initialize();
  void Update(double timeDifference);
  WEAPON_OBJECT m_WO;
  void UpdatePlayerPosition(float x, float y);

private:
};

#endif