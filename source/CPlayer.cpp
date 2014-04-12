#include "..\include\CPlayer.h"  

CPlayer::CPlayer(){
  m_state = PLAYER_STATE_STAND;
  
  //manage sprites associated with player
  m_spriteList.resize(MAX_PLAYER_STATES);
  GRAPHIC_IMAGE gi;
  
  m_spriteList[0].spriteID = 52;
  m_spriteList[1].spriteID = 53;
  m_spriteList[2].spriteID = 54;
  m_spriteList[3].spriteID = 55;
  m_spriteList[4].spriteID = 56;
  m_spriteList[5].spriteID = 57;

  gi = m_spriteList[PLAYER_STATE_STAND];
  go.spriteID = gi.spriteID;
  go.maxFrames = gi.maxFrames;
  go.updateInterval = 0;
  go.totalTime = 0;

  m_kindness = 1000;
  m_health = 1000;
  m_score = 0;
  m_time = 300;
  m_speed = 150;

}

CPlayer::~CPlayer(){


}

//used to setup graphic dependent data for CPlayer
void CPlayer::SetupPlayer(CGraphics &console){
  GRAPHIC_IMAGE gi;  
  SetState(PLAYER_STATE_WALK);
  CLog *pLog = CLog::Instance(); 

  for(int i = 0; i < m_spriteList.size(); ++i){
    gi = console.GetSprite(m_spriteList[i].spriteID);
    m_spriteList[i].updateInterval = gi.updateInterval;
    m_spriteList[i].maxFrames = gi.maxFrames;
    //pLog->Log("maxframes",i,gi.maxFrames);
  }

  gi = GetSpriteInfo(GetCurrentSprite());
  go.updateInterval = gi.updateInterval;
  go.frameCount = 0;
  go.maxFrames = gi.maxFrames;
  go.totalTime = 0;
  go.width = gi.width;
  go.height = gi.height;
  go.spriteID = gi.spriteID;
  go.angle = 0;

}

void CPlayer::SetStateSprite(int state, int spriteID){
  if(state < 0 || state > MAX_PLAYER_STATES)
    return;

  m_spriteList[state].spriteID = spriteID;
  //go.spriteID = spriteID;
}

float CPlayer::GetPlayerDir(){
  return go.angle;
}

void CPlayer::MoveLeft(double timeDifference){
  go.angle = 4.71;
  go.pos.x -= m_speed * timeDifference;
  m_state = PLAYER_STATE_WALK;
}
void CPlayer::MoveRight(double timeDifference){
  go.angle = 1.57;
  go.pos.x += m_speed * timeDifference;
  m_state = PLAYER_STATE_WALK;
}
void CPlayer::MoveUp(double timeDifference){
  go.angle = 0;
  //data.m_playerY -= 100.0f * timeDiff;
  go.pos.y -= m_speed * timeDifference;
  m_state = PLAYER_STATE_WALK;
}
void CPlayer::MoveDown(double timeDifference){
  go.angle = 3.14;
  go.pos.y += m_speed * timeDifference;
  m_state = PLAYER_STATE_WALK;
}


void CPlayer::SetPosition (int x, int y){
  go.pos.x = x;
  go.pos.y = y;
}

/*
void CPlayer::initialize(){

}*/

void CPlayer::Update(double timeDifference){
  CLog *pLog = CLog::Instance(); 
  //look to see if sprite has changed due to change in player state
  GRAPHIC_IMAGE gi = GetSpriteInfo(go.spriteID);//GetCurrentSprite());

  if(GetCurrentSprite() != go.spriteID){
    //m_state = PLAYER_STATE_WALK; 
    //m_sprite = go.spriteID;//GetCurrentSprite();

    
    go.updateInterval = gi.updateInterval;
    go.frameCount = 0;
    go.maxFrames = gi.maxFrames;
    go.totalTime = 0;
    go.spriteID = GetCurrentSprite();
   
  }  

  //pLog->Log("sprite",go.spriteID, go.maxFrames);
  //go.spriteID = 53;

  //update frame animation
  if(go.maxFrames > 1){

    go.totalTime += timeDifference;
    if(go.totalTime > go.updateInterval){
      go.totalTime = 0;
      go.frameCount++;
      if(go.frameCount > go.maxFrames - 1)
        go.frameCount = 0;
    }
  }
}

GAME_OBJECT CPlayer::GetGameObject(){
  CLog *pLog = CLog::Instance(); 
  //pLog->Log("go spriteID", go.spriteID);
  //pLog->Log("go frameCount", go.frameCount);
  //pLog->Log("go maxframes", go.maxFrames);
  //go.pos.x = m_playerX;
  //go.pos.y = m_playerY;
  return go;
}

int CPlayer::GetCurrentSprite(){
  return m_spriteList[m_state].spriteID;
}

void CPlayer::PrintStateSprite(){
  CLog *pLog = CLog::Instance(); 

  for(int i = 0; i < m_spriteList.size(); ++i){
    pLog->Log("State", i, m_spriteList[i].spriteID);
  }

}

GRAPHIC_IMAGE CPlayer::GetSpriteInfo(int spriteID){
  GRAPHIC_IMAGE gi;
  gi.spriteID = 21;//default dummy sprite
  gi.maxFrames = 1;
  gi.updateInterval = 0;

  for(int i = 0; i < m_spriteList.size(); ++i){
    if(spriteID == m_spriteList[i].spriteID){
      gi.spriteID = spriteID;
      gi.maxFrames = m_spriteList[i].maxFrames;
      gi.updateInterval = m_spriteList[i].updateInterval;
      return gi;
    }
  }

  return gi;
}




/*
CPlayer& CPlayer::operator =(const CPlayer& rhs){
  if(this == &rhs)
    return *this;

  go = rhs.go;

  return *this;
}
*/

void CPlayer::UpdatePlayerPosition(float x, float y){
  
}