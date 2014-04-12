#include "..\include\cGameStatePlay1.h"
 
cGameStatePlay1::cGameStatePlay1(){

}

cGameStatePlay1::~cGameStatePlay1()
{
	m_TE.clear();
}

void cGameStatePlay1::initialize(){
  m_viewChoice = 0;
  ::ShowCursor(true);
  m_currentSprite = 20;
  m_secondTimer.initialize();
}

void cGameStatePlay1::activate(CGameData &data){

}
//***************************************************************
//                         U P D A T E
//***************************************************************
cGameStateObject* cGameStatePlay1::update(double timeDifference, Camera &cam, CGameData &data, CGraphics &con){
  m_event = EVENT_GO_NO_WHERE;
  static bool bStop = false;

  D3DXVECTOR3 position;
  GRAPHIC_IMAGE gi;
  GAME_OBJECT go;

  CLog *pLog = CLog::Instance(); 

  //play audio file at start of game state
  CAudioManager *pAudio = CAudioManager::Instance();
  //if(pAudio->IsPlaying(0) == false)    
  //  pAudio->PlaySoundClip(0, 1);

  //configure mouse
  m_mouse.SetHandle(data.m_hWnd);

  //first time code
  //**********************************************************
  static bool bFirstTime = true;
  if(bFirstTime == true){
    position.x = 0.0f; position.y = 3.0f; position.z = 0.0f;
    cam.setPosition(&position); 
    cam.roll(0.0f);
    
    m_inventoryX = con.GetScreenWidth() - 163; //293;
    m_inventoryY = 10;
    m_statusX = 150;   //10;
    m_statusY = con.GetScreenHeight() - 95;

    bFirstTime = false;
    m_viewChoice = 4;
    data.m_player.SetupPlayer(con); //updates player data
    data.LoadGameLevel("data//level1.dat", con);
    //data.m_timeOut = false;
    //data.m_player.m_time = 100;
    //data.m_gameOver = false;
    m_bInventory = true;
    m_bPause = false;
    m_bHelp = false;
    //data.m_score = 0;
    //data.m_levelComplete = false;
  }

  //this is the code to allow for pause
  //***********************************
  static double pauseTime = 0;
  pauseTime += timeDifference;  
  if(::GetActiveWindow() == data.m_hWnd && keyDown('P') == true && pauseTime > 0.25){
    m_bPause = !m_bPause;
    pauseTime = 0;
  }
  if(m_bPause == true)
    return 0;

  //manages help
  //**********************************
  static double helpTime = 0;
  helpTime += timeDifference;
  if(::GetActiveWindow() == data.m_hWnd && keyDown('H') == true && helpTime > 0.25){
    m_bHelp = !m_bHelp;
    helpTime = 0;
  }

  //manages kindness accumulation next to grandma
  //*********************************************
  static double kindnessTime = 0;
  kindnessTime += timeDifference;

  //game over if not enough time
  //*********************************
  if(m_secondTimer.getTimer(1) == true){
    data.m_player.m_time--;
    if(data.m_player.m_time <= 0){
      data.m_gameOver = true;
      data.m_timeOut = true;
    }
  }

  //level is complete
  //***************************************
  if(data.m_levelComplete == true){
    m_event = EVENT_GO_CONTROL;
    //return 0;
  }

  //game over
  if(data.m_gameOver == true){
    m_event = EVENT_GO_CONTROL;
    //return 0;
  }

  //update projectiles
  for(int i = 0; i < data.m_projectiles.size(); ++i){
    data.m_projectiles[i].Update(timeDifference);
  }

  //escape state by pressing ESC key
  if(::GetActiveWindow() == data.m_hWnd && keyDown(VK_ESCAPE)){
    data.m_playerQuit = true;
    m_event = EVENT_GO_CONTROL;
  }
  //if(::GetActiveWindow() == data.m_hWnd && keyDown('2'))
  //  m_event = EVENT_GO_PLAY2;

  cam.getPosition(&position);
  static double timeDiff = 0;
  static double timeFire = 0;
  static double timeKey = 0;
  timeDiff = timeDifference;
  timeFire += timeDifference;
  timeKey += timeDifference;

  //update all objects
  for(int i = 0; i < data.m_pObject.size(); ++i){
    data.m_pObject[i]->Update(timeDiff);
    data.m_pObject[i]->UpdatePlayerPosition(data.m_player.go.pos.x, data.m_player.go.pos.y);
  }

  bool bRet = false;

  //delete obsolete projectiles
  for(int i = data.m_projectiles.size() - 1; i > -1; --i){
    if(data.m_projectiles[i].go.alive == false)
      data.m_projectiles.erase(data.m_projectiles.begin() + i, data.m_projectiles.begin() + i + 1);
  }

  //delete objects
  for(int i = data.m_pObject.size() - 1; i > -1; --i){
    if(data.m_pObject[i]->go.alive == false)
      data.m_pObject.erase(data.m_pObject.begin() + i, data.m_pObject.begin() + i + 1);
  }

  //slows down key press for PageUp and PageDown
  static double timeSprite = 0;
  timeSprite += timeDiff;

  //**********************************************
  // Move player
  //**********************************************
  //only process keys if this window has focus
  if(::GetActiveWindow() == data.m_hWnd){// && gTimerKey.getTimer(0.03) == true){
    cam.yaw(0.0f);

    //walk
    if(keyDown('W') == true){
      //cam.walk(0.5f * timeDiff);
      data.m_player.MoveUp(timeDiff);
      if(data.m_player.go.pos.y < data.m_world_top)
        data.m_player.go.pos.y = data.m_world_top;
    }
    else if(keyDown('S') == true){
      //cam.walk(-0.5f * timeDiff);
      data.m_player.MoveDown(timeDiff);
      if(data.m_player.go.pos.y > data.m_world_bottom - 32)
        data.m_player.go.pos.y = data.m_world_bottom - 32;
    }

    //strafe
    else if(keyDown('A') == true){
      //cam.strafe(-0.5f * timeDiff);
      data.m_player.MoveLeft(timeDiff);
      if(data.m_player.go.pos.x < data.m_world_left)
        data.m_player.go.pos.x = data.m_world_left;
    }
    else if(keyDown('D') == true){
      //cam.strafe(0.5f * timeDiff);
      data.m_player.MoveRight(timeDiff);
      if(data.m_player.go.pos.x > data.m_world_right - 32)
        data.m_player.go.pos.x = data.m_world_right - 32;
    }
    else
      data.m_player.Stand();
  }
   
  //select weapon type from keyboard
  //****************************************
  if(::GetActiveWindow() == data.m_hWnd && timeKey > 0.25){
    if(keyDown('1'))
      data.m_weaponType = 1;//pistol
    else if(keyDown('2'))
      data.m_weaponType = 2;//shotgun
    else if(keyDown('3'))
      data.m_weaponType = 3;//machine gun
    else if(keyDown('4'))
      data.m_weaponType = 4;//rockets
    else if(keyDown('5'))
      data.m_weaponType = 5;//flame thrower

    //toggle inventory
    if(keyDown('I') && m_bInventory == true)
      m_bInventory = false;
    else if(keyDown('I') && m_bInventory == false)
      m_bInventory = true;

    timeKey = 0;
  }

  //fire weapon with spacebar
  if(::GetActiveWindow() == data.m_hWnd && timeFire > 0.25){

    //walk
    if((keyDown(VK_SPACE) == true || data.m_bLeftMouseDown == true) && data.m_weapons[data.m_weaponType].exists == true && data.m_weapons[data.m_weaponType].rounds > 0){
      data.AddProjectile(data.m_player.go.pos.x, data.m_player.go.pos.y, 0, 0, data.m_weaponType, data.m_player.GetPlayerDir());
      data.m_weapons[data.m_weaponType].rounds--;
  /*    if(data.m_weaponType == WEAPON_PISTOL)// && pAudio->IsPlaying(5) == false)
        pAudio->PlaySoundClip(5, 0);
      if(data.m_weaponType == WEAPON_SHOTGUN)// && pAudio->IsPlaying(4) == false)
        pAudio->PlaySoundClip(4, 0);
      if(data.m_weaponType == WEAPON_MACHINEGUN && pAudio->IsPlaying(2) == false)
        pAudio->PlaySoundClip(2, 0);
      if(data.m_weaponType == WEAPON_ROCKET)// && pAudio->IsPlaying(6) == false)
        pAudio->PlaySoundClip(6, 0);
      if(data.m_weaponType == WEAPON_FLAMETHROWER)// && pAudio->IsPlaying(3) == false)
        pAudio->PlaySoundClip(3, 0);
*/
      //pLog->Log("Projectile added", data.m_player.go.pos.x, data.m_player.go.pos.y);
      timeFire = 0;
    }
  }
  
  //combined direction and shooting
  //*******************************************************************************************************************
  if(::GetActiveWindow() == data.m_hWnd  && timeFire > 0.25){// && gTimerKey.getTimer(0.03) == true){
    cam.yaw(0.0f);

    //walk
    if(keyDown(VK_UP) == true  && data.m_weapons[data.m_weaponType].exists == true && data.m_weapons[data.m_weaponType].rounds > 0){
      //cam.walk(0.5f * timeDiff);
      data.m_player.MoveUp(timeDiff);
      if(data.m_player.go.pos.y < data.m_world_top)
        data.m_player.go.pos.y = data.m_world_top;
      data.AddProjectile(data.m_player.go.pos.x, data.m_player.go.pos.y, 0, 0, data.m_weaponType, data.m_player.GetPlayerDir());
      data.m_weapons[data.m_weaponType].rounds--;

    }
    else if(keyDown(VK_DOWN) == true   && data.m_weapons[data.m_weaponType].exists == true && data.m_weapons[data.m_weaponType].rounds > 0){
      //cam.walk(-0.5f * timeDiff);
      data.m_player.MoveDown(timeDiff);
      if(data.m_player.go.pos.y > data.m_world_bottom - 32)
        data.m_player.go.pos.y = data.m_world_bottom - 32;
      data.AddProjectile(data.m_player.go.pos.x, data.m_player.go.pos.y, 0, 0, data.m_weaponType, data.m_player.GetPlayerDir());
      data.m_weapons[data.m_weaponType].rounds--;
    }
    //strafe
    else if(keyDown(VK_LEFT) == true   && data.m_weapons[data.m_weaponType].exists == true && data.m_weapons[data.m_weaponType].rounds > 0){
      //cam.strafe(-0.5f * timeDiff);
      data.m_player.MoveLeft(timeDiff);
      if(data.m_player.go.pos.x < data.m_world_left)
        data.m_player.go.pos.x = data.m_world_left;
      data.AddProjectile(data.m_player.go.pos.x, data.m_player.go.pos.y, 0, 0, data.m_weaponType, data.m_player.GetPlayerDir());
      data.m_weapons[data.m_weaponType].rounds--;

    }
    else if(keyDown(VK_RIGHT) == true   && data.m_weapons[data.m_weaponType].exists == true && data.m_weapons[data.m_weaponType].rounds > 0){
      //cam.strafe(0.5f * timeDiff);
      data.m_player.MoveRight(timeDiff);
      if(data.m_player.go.pos.x > data.m_world_right - 32)
        data.m_player.go.pos.x = data.m_world_right - 32;
      data.AddProjectile(data.m_player.go.pos.x, data.m_player.go.pos.y, 0, 0, data.m_weaponType, data.m_player.GetPlayerDir());
      data.m_weapons[data.m_weaponType].rounds--;

    }
    else
      data.m_player.Stand();

    timeFire = 0;
  }

  //select weapon from inventory
  if(::GetActiveWindow() == data.m_hWnd && data.m_bLeftMouseDown == true){
    if(m_mouse.MouseX() > m_inventoryX && m_mouse.MouseX() < m_inventoryX + 140 &&
       m_mouse.MouseY() > m_inventoryY && m_mouse.MouseY() < m_inventoryY + 730){
      
        if(m_mouse.MouseY() > m_inventoryY + 68 && m_mouse.MouseY() < m_inventoryY + 196)
          data.m_weaponType = 1;
        else if(m_mouse.MouseY() > m_inventoryY + 196 && m_mouse.MouseY() < m_inventoryY + 328)
          data.m_weaponType = 2;
        else if(m_mouse.MouseY() > m_inventoryY + 328 && m_mouse.MouseY() < m_inventoryY + 460)
          data.m_weaponType = 3;
        else if(m_mouse.MouseY() > m_inventoryY + 460 && m_mouse.MouseY() < m_inventoryY + 590)
          data.m_weaponType = 4;
        else if(m_mouse.MouseY() > m_inventoryY + 590 && m_mouse.MouseY() < m_inventoryY + 710)
          data.m_weaponType = 5;
        if(pAudio->IsPlaying(1) == false)    
          pAudio->PlaySoundClip(1, 0);
      
    }
  }

  //update player information
  data.m_player.Update(timeDiff);

  //update grandma information
  data.m_player.m_rechargeKindness = false;
  if(kindnessTime > 0.1){
    for(int i = 0; i < data.m_grandma.size(); ++i){
      data.m_grandma[i].Update(timeDiff);
      if(sqrt((data.m_player.go.pos.x - data.m_grandma[i].go.pos.x) * (data.m_player.go.pos.x - data.m_grandma[i].go.pos.x) +
        (data.m_player.go.pos.y - data.m_grandma[i].go.pos.y) * (data.m_player.go.pos.y - data.m_grandma[i].go.pos.y)) < 100){
          data.m_player.m_kindness += 3;
          data.m_player.m_rechargeKindness = true;
          if(pAudio->IsPlaying(16) == false)    
            pAudio->PlaySoundClip(16, 0);
      }    
    }
    kindnessTime = 0;
  }

  // C O L L I S I O N    D E T E C T I O N player
  for(int i = 0; i < data.m_pObject.size(); ++i){

    //collision with block
    if(data.m_pObject[i]->go.layer == 6 && data.m_pObject[i]->go.objectID != 27){
      bRet = data.IsPlayerCollision(i);        
    }

    //collision with elf...oops!!!
    else if(data.m_pObject[i]->go.layer == 8 && data.m_pObject[i]->go.objectID == 27){
      if(data.IsPlayerCollision(i) == true){
        data.m_player.m_health -= 1;
        if(pAudio->IsPlaying(8) == false)    
          pAudio->PlaySoundClip(8, 0);
        break;
      }
    }
    //collision with fairy
    else if(data.m_pObject[i]->go.layer == 8 && data.m_pObject[i]->go.objectID == 28){
      if(data.IsPlayerCollision(i) == true){
        data.m_player.m_health -= 1;
        if(pAudio->IsPlaying(7) == false)    
          pAudio->PlaySoundClip(7, 0);
        break;
      }
    }
    //collision with witch
    else if(data.m_pObject[i]->go.layer == 8 && data.m_pObject[i]->go.objectID == 29){
      if(data.IsPlayerCollision(i) == true){
        data.m_player.m_health -= 1;
        if(pAudio->IsPlaying(9) == false)    
          pAudio->PlaySoundClip(9, 0);
        break;
      }
    }

    //weapon pickup
    else if((data.m_pObject[i]->go.layer == 7 && data.m_pObject[i]->go.radius == -1) || data.m_pObject[i]->go.objectID == 30){
      if(data.IsPlayerCollision(i) == true){
        data.m_pObject[i]->go.alive = false;

        //pistol
        if(data.m_pObject[i]->go.objectID == 12){ // && data.m_weapons[1].exists == false){
          data.m_weapons[1].exists = true;
          data.m_weapons[1].rounds += data.m_weapons[1].maxRounds;
          if(pAudio->IsPlaying(12) == false)    
            pAudio->PlaySoundClip(12, 0);

          data.m_pObject.erase(data.m_pObject.begin() + i, data.m_pObject.begin() + i + 1);
        }

        //shotgun
        else if(data.m_pObject[i]->go.objectID == 13){// && data.m_weapons[2].exists == false){
          data.m_weapons[2].exists = true;
          data.m_weapons[2].rounds += data.m_weapons[2].maxRounds;
          if(pAudio->IsPlaying(12) == false)    
            pAudio->PlaySoundClip(12, 0);

          data.m_pObject.erase(data.m_pObject.begin() + i, data.m_pObject.begin() + i + 1);
        }

        //machine gun
        else if(data.m_pObject[i]->go.objectID == 14){// && data.m_weapons[3].exists == false){
          data.m_weapons[3].exists = true;
          data.m_weapons[3].rounds += data.m_weapons[3].maxRounds;
          if(pAudio->IsPlaying(12) == false)    
            pAudio->PlaySoundClip(12, 0);

          data.m_pObject.erase(data.m_pObject.begin() + i, data.m_pObject.begin() + i + 1);
        }

        //rocket launcher
        else if(data.m_pObject[i]->go.objectID == 15){// && data.m_weapons[4].exists == false){
          data.m_weapons[4].exists = true;
          data.m_weapons[4].rounds += data.m_weapons[4].maxRounds;
          if(pAudio->IsPlaying(12) == false)    
            pAudio->PlaySoundClip(12, 0);

          data.m_pObject.erase(data.m_pObject.begin() + i, data.m_pObject.begin() + i + 1);
        }

        //flame thrower
        else if(data.m_pObject[i]->go.objectID == 16){// && data.m_weapons[5].exists == false){
          data.m_weapons[5].exists = true;
          data.m_weapons[5].rounds += data.m_weapons[5].maxRounds;
          if(pAudio->IsPlaying(12) == false)    
            pAudio->PlaySoundClip(12, 0);

          data.m_pObject.erase(data.m_pObject.begin() + i, data.m_pObject.begin() + i + 1);
        }

        //ammo_pistol
        else if(data.m_pObject[i]->go.objectID == 17){// && data.m_weapons[1].exists == true){
          data.m_weapons[1].rounds += data.m_weapons[1].maxRounds;
          if(pAudio->IsPlaying(12) == false)    
            pAudio->PlaySoundClip(12, 0);
          data.m_pObject.erase(data.m_pObject.begin() + i, data.m_pObject.begin() + i + 1);
        }

        //ammo shotgun
        else if(data.m_pObject[i]->go.objectID == 18){// && data.m_weapons[2].exists == true){
          data.m_weapons[2].rounds += data.m_weapons[2].maxRounds;
          if(pAudio->IsPlaying(12) == false)    
            pAudio->PlaySoundClip(12, 0);
          data.m_pObject.erase(data.m_pObject.begin() + i, data.m_pObject.begin() + i + 1);
        }

        //ammo machine gun
        else if(data.m_pObject[i]->go.objectID == 19 ){//&& data.m_weapons[3].exists == true){
          data.m_weapons[3].rounds += data.m_weapons[3].maxRounds;
          if(pAudio->IsPlaying(12) == false)    
            pAudio->PlaySoundClip(12, 0);
          data.m_pObject.erase(data.m_pObject.begin() + i, data.m_pObject.begin() + i + 1);
        }

        //pickup first aid health
        else if(data.m_pObject[i]->go.objectID == 9 ){//&& data.m_weapons[3].exists == true){
          data.m_pObject[i]->go.alive = false;
          data.m_player.m_health += 200;
          if(data.m_player.m_health > 1000)
              data.m_player.m_health = 1000;
          if(pAudio->IsPlaying(10) == false)    
            pAudio->PlaySoundClip(10, 0);

          data.m_pObject.erase(data.m_pObject.begin() + i, data.m_pObject.begin() + i + 1);
        }

        //exit through the door
        else if(data.m_pObject[i]->go.objectID == 20 || data.m_pObject[i]->go.objectID == 30 ){//&& data.m_weapons[3].exists == true){
          data.m_pObject[i]->go.alive = false;
          data.m_levelComplete = true;
          
        }
      }
    }
  }
 
  /*
  //ensure all elves don't collide
  //*****************************************************
  for(int i = 0; i < data.m_pObject.size(); ++i){

    //only process for elves
    if(data.m_pObject[i]->go.objectID == 27){
      
      //iterate through all objects except witches, and fairies
      for(int j = 0; j < data.m_pObject.size(); ++j){

        //check for collision
        if(data.m_pObject[j]->go.layer == 6){
          if(data.m_pObject[i]->go.pos.x > data.m_pObject[j]->go.pos.x - 50 && data.m_pObject[i]->go.pos.x < data.m_pObject[j]->go.pos.x + 50){
            if(data.m_pObject[i]->go.pos.y > data.m_pObject[j]->go.pos.y - 50 && data.m_pObject[i]->go.pos.y < data.m_pObject[j]->go.pos.y + 50){
              bRet = data.IsElfCollision(i, j);
            }
          }
        }

      }
    }
  }
*/
  //check game over stuff
  if(data.m_player.m_health < 1){
    data.m_gameOver = true;
    data.m_healthOut = true;
    data.m_player.m_health = 0;
  }
  if(data.m_player.m_kindness < 1){
    data.m_gameOver = true;
    data.m_player.m_kindness = 0;
    data.m_kindnessOut = true;
  }

  //process projectile collision with elf, fairy, and witch
  //*******************************************************
  data.UpdateProjectiles(timeDifference);

  //data.m_pObject.erase(data.m_pObject.begin() + nDelete, data.m_pObject.begin() + nDelete + 1); 
  //events manager
  for(unsigned int i=0; i< m_TE.size(); i++){
    if (m_TE[i].event == m_event){
      return m_TE[i].p_gso;
    }
  }
  return 0;
}

//***************************************************************
//                         R E N D E R 
//***************************************************************
void cGameStatePlay1::render(CGraphics &con, Camera &cam, CGameData &data){
  CLog *pLog = CLog::Instance();

  //variables
  static bLostDevice = false;
  static int lostCount = 0;
  GRAPHIC_IMAGE gi;
  GAME_OBJECT go;
  HRESULT hr;
  GAME_OBJECT temp;

  ::ShowCursor(false);
 
  //first time code
  static bool bFirstTime = true;
  if(bFirstTime == true){

    //look for animated data...and load GI stuff into GO
    for(int i = 0; i < data.LevelSize(); ++i){
      go = data.GetLevelData(i);
      if(go.asset == C_SPRITE){
        for(int j = 2; j < 7; ++j){
          gi = con.GetSprite(go.spriteID);
          go.maxFrames = gi.maxFrames;
          go.updateInterval = gi.updateInterval;
          data.UpdateLevelData(i, go);          
        }
      }
    }
    bFirstTime = false;
  }

  if(FAILED(hr = con.m_d3ddev->TestCooperativeLevel())){
    pLog->Log("Failed test");
    if(hr == D3DERR_DEVICELOST)
      return;
    pLog->Log("No lost");
    if( hr == D3DERR_DEVICENOTRESET){// && bLostDevice == true  ){
      lostCount++;
      pLog->Log("Time to reset");
      con.DeleteLost();
      pLog->Log("Deleted everything");
      data.LoadConfigFile("data//config.txt");
      data.LoadGameLevel("data//level1.dat", con);

      hr = con.m_d3ddev->Reset(&con.GetDPP());
      pLog->Log("Dev reset");
      con.ReloadLost();
      pLog->Log("ReloadLost");
      bLostDevice = false;
      return;
    }
  }

  D3DXMATRIX V;
  D3DXVECTOR3 pos;
  cam.getPosition(&pos);  
  cam.getViewMatrix(&V, m_viewChoice);
  con.m_d3ddev->SetTransform(D3DTS_VIEW, &V);

  // clear the window to a deep blue
  //con.m_d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
  con.m_d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
  con.m_d3ddev->BeginScene();    // begins the 3D scene
  
  //render layer 0 
  for(int i = 0; i < data.LevelSize(); ++i){
    temp = data.GetLevelData(i);
    if(temp.asset == C_SPLATTER)
      con.RenderSplatter(temp.meshID, temp.x, temp.y, temp.z, temp.rx, temp.ry, temp.rz);//, temp.ry);
  }

  //render layer 1
  for(int i = 0; i < data.LevelSize(); ++i){
    temp = data.GetLevelData(i);
    if(temp.asset == C_MESH){
      if(temp.meshID == 1){//BADGE test
        temp.rx = 0;//m_badgeAngleX;
        temp.ry = 0;//m_badgeAngleZ;
        con.RenderMesh(temp);
      }
      else
        con.RenderMesh(temp);//.meshID, temp.rx, temp.ry, temp.rz);
    }
  }
    
  //render all objects
  int w = con.GetScreenWidth()/2;
  int h = con.GetScreenHeight()/2;
  int px = data.m_player.go.pos.x;
  int py = data.m_player.go.pos.y;

  go = data.m_player.GetGameObject();

  for(int j = 3; j <= 6; ++j){//3..4
    for(int i = 0; i < data.m_pObject.size(); ++i){
      if(data.m_pObject[i]->go.layer == j && data.m_pObject[i]->go.alive == true ){
        if(data.m_pObject[i]->go.pos.x > px - w - 128  && data.m_pObject[i]->go.pos.x < px + w + 128 &&
           data.m_pObject[i]->go.pos.y > py - h - 128 && data.m_pObject[i]->go.pos.y < py + h + 128)
          con.RenderGraphicModulate(data.m_pObject[i]->go, con.GetScreenWidth()/2 + (data.m_pObject[i]->go.pos.x - go.pos.x) - 16, con.GetScreenHeight()/2 + (data.m_pObject[i]->go.pos.y - go.pos.y) - 16, 255, 255, 255);
      }
    }
  }

  //draws cindy's face as a graphic place holder
  go.width = 32;
  go.height = 32;
  go.pos.x = con.GetScreenWidth()/2 ;//- gi.width/2;
  go.pos.y = con.GetScreenHeight()/2 ;// - gi.height/2;
  con.RenderGraphicModulate(go, 255, 255, 255);

  //draws cindy's face as a graphic place holder
  //gi = con.GetSprite(21);
  //gi.scale = 0.75f/2.0f;//pos.y;
  //con.RenderGraphicModulate(gi, con.GetScreenWidth()/2 - (gi.scale * gi.width)/2 , con.GetScreenHeight()/2 - (gi.scale * gi.height)/2, 255, 255, 255);


  go = data.m_player.GetGameObject();

  for(int j = 7; j <= 8; ++j){//5..9
    for(int i = 0; i < data.m_pObject.size(); ++i){
      if(data.m_pObject[i]->go.layer == j ){//&& data.m_pObject[i]->go.alive == true){
        if(data.m_pObject[i]->go.pos.x > px - w - 128  && data.m_pObject[i]->go.pos.x < px + w + 128 &&
           data.m_pObject[i]->go.pos.y > py - h - 128 && data.m_pObject[i]->go.pos.y < py + h + 128)
          con.RenderGraphicModulate(data.m_pObject[i]->go, con.GetScreenWidth()/2 + (data.m_pObject[i]->go.pos.x - go.pos.x) - 16, con.GetScreenHeight()/2 + (data.m_pObject[i]->go.pos.y - go.pos.y) - 16, 255, 255, 255);
      }
    }
  }
   
  //render grandmas
  for(int i = 0; i < data.m_grandma.size(); ++i){
   // pLog->Log(data.m_grandma[i].go.objectID, data.m_grandma[i].go.spriteID, data.m_grandma[i].go.pos.x, data.m_grandma[i].go.pos.y);
    con.RenderGraphicModulate(data.m_grandma[i].go, con.GetScreenWidth()/2 + (data.m_grandma[i].go.pos.x - go.pos.x) - 16, con.GetScreenHeight()/2 + (data.m_grandma[i].go.pos.y - go.pos.y) - 16, 255, 255, 255);
  }


  //draw projectiles
  for(int i = 0; i < data.m_projectiles.size(); ++i){
    gi = con.GetSprite(data.m_projectiles[i].m_WO.spriteID);
    if(data.m_projectiles[i].go.alive == true)
      con.RenderGraphicModulate(data.m_projectiles[i].go, con.GetScreenWidth()/2 + (data.m_projectiles[i].go.pos.x - go.pos.x) - 16, con.GetScreenHeight()/2 + (data.m_projectiles[i].go.pos.y - go.pos.y) - 16, 255, 255, 255);
  }

  //draws a circle around Cindy's position
  if(data.m_player.m_rechargeKindness == false)
    con.DrawCircle(data.m_screenWidth/2, data.m_screenHeight/2, 255, 255, 255, 48);
  else
    con.DrawCircle(data.m_screenWidth/2, data.m_screenHeight/2, 0, 0, 255, 48);

  //draw rectangle around screen
  if(m_bInventory == true)
    con.DrawRect(3, 3, data.m_screenWidth - 6, data.m_screenHeight - 6, 255, 255, 255);
  
  //map
  //************************************************
  gi = con.GetSprite(45);
  go = data.m_player.GetGameObject();

  m_mapX = 10 + gi.width + 220;
  m_mapY = 10;
  if(m_bInventory == true)
    con.DrawRect((float)m_mapX, (float)m_mapY, (float)(m_mapX + 80), (float)(m_mapY + 80), 255,0,0);//draw map
  int smallW = con.GetScreenWidth()/64;
  int smallH = con.GetScreenHeight()/85;
  if(m_bInventory == true)
    con.DrawRect((float)m_mapX + data.m_player.go.pos.x/32 - smallW, (float)m_mapY + data.m_player.go.pos.y/32 - smallH, 
                 (float)m_mapX + data.m_player.go.pos.x/32 + smallW, (float)m_mapY + data.m_player.go.pos.y/32 + smallH,255, 255, 255); //draw player

  //inventory
  //*******************************************************************************
  int transParency = 120;
  if(m_bInventory == true && m_bHelp == false){
    gi = con.GetSprite(58);
    gi.alpha = transParency;
    con.RenderGraphicModulate(gi, m_inventoryX, m_inventoryY, 255, 255, 255);
    if(data.m_weapons[1].exists == true && data.m_weapons[1].rounds > 0 )
      gi = con.GetSprite(65);
    else
      gi = con.GetSprite(64);
    gi.alpha = transParency;
    con.RenderGraphicModulate(gi, m_inventoryX + 14, m_inventoryY + 63,  255, 255, 255);
    if(data.m_weapons[2].exists == true && data.m_weapons[2].rounds > 0)
      gi = con.GetSprite(67);
    else
      gi = con.GetSprite(66);
    gi.alpha = transParency;
    con.RenderGraphicModulate(gi, m_inventoryX + 14, m_inventoryY + 192,  255, 255, 255);
    if(data.m_weapons[3].exists == true  && data.m_weapons[3].rounds > 0)
      gi = con.GetSprite(69);
    else
      gi = con.GetSprite(68);
    gi.alpha = transParency;
    con.RenderGraphicModulate(gi, m_inventoryX + 14, m_inventoryY + 321,  255, 255, 255);

    if(data.m_weapons[4].exists == true  && data.m_weapons[4].rounds > 0)
      gi = con.GetSprite(71);
    else
      gi = con.GetSprite(70);
    gi.alpha = transParency;
    con.RenderGraphicModulate(gi, m_inventoryX + 14, m_inventoryY + 450, 255, 255, 255);
    if(data.m_weapons[5].exists == true  && data.m_weapons[5].rounds > 0)
      gi = con.GetSprite(73);
    else
      gi = con.GetSprite(72);
    gi.alpha = transParency;
    con.RenderGraphicModulate(gi, m_inventoryX + 14, m_inventoryY + 579, 255, 255, 255);
  }

  //display round count
  if(m_bInventory == true && m_bHelp == false){
    con.Print(data.m_weapons[1].rounds, F_V20, m_inventoryX + 24, m_inventoryY + 73, 255, 0, 0, 255);
    con.Print(data.m_weapons[2].rounds, F_V20, m_inventoryX + 24, m_inventoryY + 201, 255, 0, 0, 255);
    con.Print(data.m_weapons[3].rounds, F_V20, m_inventoryX + 24, m_inventoryY + 329, 255, 0, 0, 255);
    con.Print(data.m_weapons[4].rounds, F_V20, m_inventoryX + 24, m_inventoryY + 457, 255, 0, 0, 255);
    con.Print(data.m_weapons[5].rounds, F_V20, m_inventoryX + 24, m_inventoryY + 595, 255, 0, 0, 255);
  }

  //draws outline around selected weapon using m_weaponType;
  if(m_bInventory == true && m_bHelp == false){
    if(data.m_weaponType >= 0 && data.m_weaponType <= 5){
      con.DrawRect(m_inventoryX + 15, m_inventoryY + 64 + ((data.m_weaponType - 1) * 129), m_inventoryX + 139, m_inventoryY + 189 + ((data.m_weaponType - 1) * 129), 255, 0, 0);
      con.DrawRect(m_inventoryX + 16, m_inventoryY + 65 + ((data.m_weaponType - 1) * 129), m_inventoryX + 138, m_inventoryY + 188 + ((data.m_weaponType - 1) * 129), 255, 0, 0);
      con.DrawRect(m_inventoryX + 17, m_inventoryY + 66 + ((data.m_weaponType - 1) * 129), m_inventoryX + 137, m_inventoryY + 187 + ((data.m_weaponType - 1) * 129), 255, 0, 0);
    }
  }

  //status bar
  if(m_bHelp == false){
    gi = con.GetSprite(59);
    gi.alpha = transParency;
    con.RenderGraphicModulate(gi, m_statusX, m_statusY, 255, 255, 255);
   
    //health
    gi = con.GetSprite(60);
    gi.alpha = transParency;
    gi.width =  (float)gi.width * (float)data.m_player.m_health * 0.001;
    con.RenderGraphicModulate(gi, m_statusX + 113, m_statusY + 17, 255, 255, 255);
    gi = con.GetSprite(61);
    gi.alpha = transParency;
    gi.width =  (float)gi.width * (float)data.m_player.m_kindness * 0.001;
    con.RenderGraphicModulate(gi, m_statusX + 113, m_statusY + 43, 255, 255, 255);

    //time and score
    con.Print(data.m_player.m_time, F_V20, m_statusX + 580, m_statusY + 20, 255, 255, 255, 255);
    con.Print(data.m_player.m_score, F_V20, m_statusX + 570, m_statusY + 46, 255, 255, 255, 255);
  }
  //draw mouse cursor
  if(m_bInventory == true){
    con.DrawLine(m_mouse.MouseX(), m_mouse.MouseY(), m_mouse.MouseX() + 15, m_mouse.MouseY(),255,255,0);
    con.DrawLine(m_mouse.MouseX(), m_mouse.MouseY(), m_mouse.MouseX() - 15, m_mouse.MouseY(),255,255,0);
    con.DrawLine(m_mouse.MouseX(), m_mouse.MouseY(), m_mouse.MouseX(), m_mouse.MouseY() + 15,255,255,0);
    con.DrawLine(m_mouse.MouseX(), m_mouse.MouseY(), m_mouse.MouseX(), m_mouse.MouseY() - 15,255,255,0);
  }

  //print text to display
  if(data.m_displayDebugInfo == true){
    //con.Print("PLAY1 STATE - Version", data.m_version, F_V16, 10, 10,  255, 255, 255, 255);
    con.Print("FPS", data.m_FPS , F_V16, 510, 10,  255, 255, 0, 255);
    //con.Print("X", m_mouse.MouseX(), F_V16, 10, 30, 0, 255, 255, 255);
    //con.Print("Y", m_mouse.MouseY(), F_V16, 150, 30, 0, 255, 255, 255);
    //con.Print("X", data.m_player.go.pos.x, F_V16, 10, 50, 0, 255, 255, 255);
    //con.Print("Y", data.m_player.go.pos.y, F_V16, 150, 50, 0, 255, 255, 255);
    //con.Print("Projectiles", data.m_projectiles.size(), F_V16, 10, 70, 255, 0, 255);
  }
  
  //overlay help information
  if(m_bHelp == true){
    gi = con.GetSprite(106);
    con.RenderGraphicModulate(gi, 20, 20, 255, 255, 255);//number keys
    gi = con.GetSprite(107);
    con.RenderGraphicModulate(gi, 700, 20, 255, 255, 255);//H, I, P
    gi = con.GetSprite(108);
    con.RenderGraphicModulate(gi, 20, 250, 255, 255, 255);//W, A, S, D
    gi = con.GetSprite(110);
    con.RenderGraphicModulate(gi, 360, 450, 255, 255, 255);
    gi = con.GetSprite(109);
    con.RenderGraphicModulate(gi, 700, 250, 255, 255, 255);
  }


  if(m_bPause == true)
    con.Print("PAUSED!", F_V20, data.m_screenWidth/2 - 50, data.m_screenHeight/2 - 10, 255, 0, 0, 255);

  //mouse coordinates
  con.Print("Press ESC to quit", F_V20, data.m_screenWidth/2 - 100, data.m_screenHeight/2 + 260, 255, 0, 0, 255);

  //finish rendering
  con.m_d3ddev->EndScene();                             // ends the 3D scene
  hr = con.m_d3ddev->Present(NULL, NULL, NULL, NULL);   // displays the created frame on the screen
  
  if(FAILED(hr = con.m_d3ddev->TestCooperativeLevel())){
    if(hr == D3DERR_DEVICELOST)
      bLostDevice = true;
      lostCount++;
  }
  return;  
}

void cGameStatePlay1::processEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
}

void cGameStatePlay1::deactivate(){
  CAudioManager *pAudio = CAudioManager::Instance();
  pAudio->KillAll();
//  Delete<psys::PSystem*>( Gun );
  
}
void cGameStatePlay1::resume(){
}
void cGameStatePlay1::pause(){
}
void cGameStatePlay1::save(){
}

void cGameStatePlay1::addTransitionEvent(int event, cGameStateObject* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}
