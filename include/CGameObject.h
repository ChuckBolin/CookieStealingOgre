/*
  CGameObject.h -
  Written by Chuck Bolin, June,2008

*/

#ifndef CGAMEOBJECT_H
#define CGAMEOBJECT_H

#include <d3d9.h>
#include <d3dx9.h>
#include <vector>
#include "..\include\CObject.h"  

/*
struct GAME_OBJECT{
  int asset; //0 = mesh, 1 = splatter
  int meshID;   //unique ID
  int meshBadID; //ID of mesh used when damaged
  float x;      //position
  float y;
  float z;
  float rx;     //rotational angles - orientation
  float ry;
  float rz;
  float collisionDiameter; 
  bool alive;
  int spriteID;
  int spriteBadID;
  int alpha;
  float angle;
  int width;
  int height;
  int left;
  int top;
  int frameCount;
  double totalTime;
  int numberFrames;
  double updateInterval;
};
*/

class CGameObject{
public:
  CGameObject();
   ~CGameObject();
   void Update(float timeDifference);
   void SetPosition(float x, float y, float z);
   void SetVelocity(float x, float y, float z);
   void SetAcceleration (float x, float y, float z);
   bool IsAlive();
   void SetMeshID(int id);
   void SetCollisionDiameter(float diameter);
  GAME_OBJECT GetData();

private:
  int m_meshID;
  int m_meshBadID;
  float m_collisionDiameter;
  bool m_bAlive;
  D3DXVECTOR3 m_pos;
  D3DXVECTOR3 m_acc;
  D3DXVECTOR3 m_vel;
  D3DXVECTOR3 m_rotate;
  
};

#endif