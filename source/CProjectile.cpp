#include "..\include\CProjectile.h"

CProjectile::CProjectile(){
  m_WO.alive = true;
  m_WO.damage = 1;
  m_WO.pos.x = 0; m_WO.pos.y = 0;
  m_WO.vel.x = 0; m_WO.vel.y = 0;
  m_WO.radius = 1;
  m_WO.distance = 0;
  m_WO.range = 10;
  m_WO.spriteID = 0;
  m_WO.weaponType = 0;
  m_WO.dir = 0;  
}

CProjectile::~CProjectile(){


}

void CProjectile::Update(double timeDifference){
  CLog *pLog = CLog::Instance();

  //static double accTime = 0;
  //accTime += timeDifference;

  //determine vel
  if(go.angle < 0.1){//== 0){//up
    m_WO.vel.x = 0;
    m_WO.vel.y = -1;
  }
  else if(go.angle < 1.6){//== 1.57){ //right
    m_WO.vel.x = 1;
    m_WO.vel.y = 0;
  }
  else if(go.angle < 3.2){//down
    m_WO.vel.x = 0;
    m_WO.vel.y = 1;
  }
  else if(go.angle < 4.8){ //left
    m_WO.vel.x = -1;
    m_WO.vel.y = 0;
  }
  
  //update weapon movement
  //if(accTime > 0.01){//update interval
   
    if(m_WO.spriteID == 85){//sg
      m_WO.vel.x *= 200 * timeDifference;
      m_WO.vel.y *= 200 * timeDifference;
      m_WO.distance += (200  * timeDifference);
      if(m_WO.distance > 300){
        m_WO.distance = 0;
        go.alive = false;
      }
    }
    else if(m_WO.spriteID == 86){//mg
      m_WO.vel.x *= 400 * timeDifference;
      m_WO.vel.y *= 400 * timeDifference;
      m_WO.distance += (400  * timeDifference);
      if(m_WO.distance > 500){
        m_WO.distance = 0;
        go.alive = false;
      }
    }
    else if(m_WO.spriteID == 88){//rocket
      m_WO.vel.x *= 700 * timeDifference;
      m_WO.vel.y *= 700 * timeDifference;
      m_WO.distance += (700  * timeDifference);
      if(m_WO.distance > 600){
        m_WO.distance = 0;
        go.alive = false;
      }
    }
    else if(m_WO.spriteID == 84){//pistol
      m_WO.vel.x *= 300 * timeDifference;
      m_WO.vel.y *= 300 * timeDifference;
      m_WO.distance += (300  * timeDifference);
      if(m_WO.distance > 400){
        m_WO.distance = 0;
        go.alive = false;
      }
    }

    //accTime = 0;
    if(m_WO.spriteID != 89  && m_WO.spriteID != 87)//flamethrower and bat
      m_WO.pos += m_WO.vel;
    go.pos.x = m_WO.pos.x;
    go.pos.y = m_WO.pos.y;

  //update frame animation
  if(go.maxFrames > 1){
    go.totalTime += timeDifference;
    if(go.totalTime > go.updateInterval){
      go.totalTime = 0;
      go.frameCount++;
      if(go.frameCount > go.maxFrames - 1){
        go.frameCount = 0;
        if(go.spriteID == 89)//flamethrower
          go.alive = false;

      }
    }
  }

  if(m_WO.spriteID == 89){//flamethrower position

    if(go.angle < 0.1){//== 0){//up
      go.pos.x = m_WO.pos.x;
      go.pos.y = m_WO.pos.y - go.height;
    }
    else if(go.angle < 1.6){//right
      go.pos.x = m_WO.pos.x + go.width + 50;
      go.pos.y = m_WO.pos.y - go.height/2 + 16;
    }
    else if(go.angle < 3.2){//down
      go.pos.x = m_WO.pos.x;// + go.width/2;
      go.pos.y = m_WO.pos.y + 34;// - go.height/2;
    }
    else if(go.angle < 4.8){ //left
      go.pos.x = m_WO.pos.x - go.width - 48;
      go.pos.y = m_WO.pos.y - go.height/2 + 16;
    }
  }
  else if(m_WO.spriteID == 87){//bat swinging
    if(go.angle < 0.1){//up
      go.pos.x = m_WO.pos.x;
      go.pos.y = m_WO.pos.y - go.height + 16;
    }
    else if(go.angle < 1.6){//right
      go.pos.x = m_WO.pos.x + go.width - 16;
      go.pos.y = m_WO.pos.y - go.height/2 + 16;
    }
    else if(go.angle < 3.2){//down
      go.pos.x = m_WO.pos.x;// + go.width/2;
      go.pos.y = m_WO.pos.y + 18;// - go.height/2;
    }
    else if(go.angle < 4.8){ //left
      go.pos.x = m_WO.pos.x - go.width + 16;
      go.pos.y = m_WO.pos.y - go.height/2 + 16;
    }
  }

}

void CProjectile::UpdatePlayerPosition(float x, float y){
  
}