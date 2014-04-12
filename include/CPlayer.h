#ifndef PLAYER_H
#define PLAYER_H

#include "CObject.h"
#include "CGraphics.h"
#include <vector>
#include "CLog.h"

const int MAX_PLAYER_STATES = 6;
const int PLAYER_STATE_STAND = 0;
const int PLAYER_STATE_WALK = 1;
const int PLAYER_STATE_DIE = 2;
const int PLAYER_STATE_PISTOL = 3;
const int PLAYER_STATE_BAT = 4;
const int PLAYER_STATE_SHOTGUN = 5;

class CPlayer : public CObject{
public:
  CPlayer();
  ~CPlayer();
  //void initialize();
  void Update(double timeDifference);
  //CLandscape& CLandscape::operator =(const CLandscape& rhs);
  void SetupPlayer(CGraphics &console); //used to setup graphic dependent data for CPlayer  
  GRAPHIC_IMAGE GetSpriteInfo(int spriteID);
  GAME_OBJECT GetGameObject();
  void SetStateSprite(int state, int spriteID);
  void SetPosition (int x, int y);  
  void SetState(int state){m_state = state;}
  int GetCurrentSprite();  //returns sprite number based upon game state
  void PrintStateSprite();
  void MoveLeft(double timeDifference);
  void MoveRight(double timeDifference);
  void MoveUp(double timeDifference);
  void MoveDown(double timeDifference);
  void Stand(){m_state = PLAYER_STATE_STAND;}
  float GetPlayerDir();
  int m_kindness;
  bool m_rechargeKindness;
  int m_health;
  int m_score;
  int m_time;
  void UpdatePlayerPosition(float x, float y);

private:
  int m_state;
  std::vector<GRAPHIC_IMAGE> m_spriteList;
  float m_speed;
};


#endif