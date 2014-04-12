#include "..\include\CGameObject.h"

CGameObject::CGameObject(){
  m_bAlive = true;
  m_collisionDiameter = 1.0f;
  m_meshID = -1;
  m_meshBadID = -1;
  m_pos.x = 0.0f;  m_pos.y = 0.0f; m_pos.z = 0.0f;
  m_acc.x = 0.0f;  m_acc.y = 0.0f; m_acc.z = 0.0f;
  m_vel.x = 0.0f;  m_vel.y = 0.0f; m_vel.z = 0.0f;
  m_rotate.x = 0.0f; m_rotate.y = 0.0f; m_rotate.z = 0.0f;

}

CGameObject::~CGameObject(){

}

void CGameObject::Update(float timeDifference){

}

void CGameObject::SetPosition(float x, float y, float z){

}

void CGameObject::SetVelocity(float x, float y, float z){

}

void CGameObject::SetAcceleration (float x, float y, float z){

}

bool CGameObject::IsAlive(){
  return m_bAlive;
}

void CGameObject::SetMeshID(int id){

}

void CGameObject::SetCollisionDiameter(float diameter){


}

GAME_OBJECT CGameObject::GetData(){
  GAME_OBJECT temp;

  temp.meshID =  m_meshID;
  temp.collisionDiameter = m_collisionDiameter;
  temp.alive = m_bAlive;
  temp.x = m_pos.x;
  temp.y = m_pos.y;
  temp.z = m_pos.z;
  temp.rx = m_rotate.x;
  temp.ry = m_rotate.y;
  temp.rz = m_rotate.z;
  return temp;
}

  
