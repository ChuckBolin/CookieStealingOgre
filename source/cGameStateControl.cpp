#include "..\include\cGameStateControl.h"
 
cGameStateControl::cGameStateControl(){
}

cGameStateControl::~cGameStateControl()
{
	m_TE.clear();
}

void cGameStateControl::initialize(){
  //m_viewChoice = 0;
  ::ShowCursor(true);
  //m_currentSprite = 20;
  m_transitionState = 0;
}

void cGameStateControl::activate(CGameData &data){

}
//***************************************************************
//                         U P D A T E
//***************************************************************
cGameStateObject* cGameStateControl::update(double timeDifference, Camera &cam, CGameData &data, CGraphics &con){
  m_event = EVENT_GO_NO_WHERE;
  //static bool bStop = false;
  //D3DXVECTOR3 position;
  GRAPHIC_IMAGE gi;
  //GAME_OBJECT go;

  CLog *pLog = CLog::Instance(); 
  //pLog->Log("Control", m_transitionState);

  //play audio file at start of game state
  CAudioManager *pAudio = CAudioManager::Instance();
  //if(pAudio->IsPlaying(0) == false)    
  //  pAudio->PlaySoundClip(0, 1);

  //configure mouse
  m_mouse.SetHandle(data.m_hWnd);

  //set aircraft camera to 1000 feet
  static bool bFirstTime = true;
  if(bFirstTime == true){
    m_transitionState = 0;
    bFirstTime = false;
    /*
    position.x = 0.0f; position.y = 3.0f; position.z = 0.0f;
    cam.setPosition(&position); 
    cam.roll(0.0f);
    
    //test of badge
    m_badgeAngleX = 0.0f;
    m_badgeAngleZ = 0.0f;

    bFirstTime = false;
    m_viewChoice = 4;
    */
  }

  //escape state by pressing ESC key
  if(::GetActiveWindow() == data.m_hWnd && keyDown(VK_ESCAPE))
    m_event = EVENT_GO_MAIN;
  //if(::GetActiveWindow() == data.m_hWnd && keyDown('1'))
  //  m_event = EVENT_GO_PLAY1;

  //cam.getPosition(&position);
  static double timeDiff = 0;
  timeDiff = timeDifference;
  static double timeSpaceKey = 0;
  timeSpaceKey += timeDifference;
  static bool bOneShot = false; //use to execute one time only transition code once
  long score = 0;

  switch(m_transitionState){
    case 0://enter from main
      
      //space bar to acknowledge welcome to game
      if(::GetActiveWindow() == data.m_hWnd && keyDown(VK_SPACE) && timeSpaceKey > 0.5){
        m_transitionState++;
        timeSpaceKey = 0;
        bOneShot = false;
        
      }
      break;

    // LEVEL 1
    //***************************
    case 1://start playing level 1
      if(bOneShot == false){
        data.m_currentLevel = 1;
        data.m_player.m_health = 1000;
        data.m_player.m_kindness = 1000;
        data.m_levelComplete = false;
        data.m_gameOver = false;
        data.m_playerQuit = false;
        data.m_healthOut = false;
        data.m_kindnessOut = false;
        data.m_pObject.clear();
        data.m_grandma.clear();
        data.m_projectiles.clear();
        data.m_player.m_score = 0;
        data.m_player.m_time = 0;
        data.m_timeOut = false;
        data.m_weapons[1].rounds = 0;   
        data.m_weapons[1].exists = false; //pistol
        data.m_weapons[2].rounds = 0;
        data.m_weapons[2].exists = false; //shotgun
        data.m_weapons[3].rounds = 0;
        data.m_weapons[3].exists = false; //machine gun
        data.m_weapons[4].rounds = 0;
        data.m_weapons[4].exists = false; //rocket launcher
        data.m_weapons[5].rounds = 0;
        data.m_weapons[5].exists = false; //flamethrower
        bOneShot = true;
        m_levelScore = 0;
      }

      //space bar to advance to game play level 1
      if(::GetActiveWindow() == data.m_hWnd && keyDown(VK_SPACE) && timeSpaceKey > 0.5){
        m_transitionState++;
        timeSpaceKey = 0;
        bOneShot = false;
        m_event = EVENT_GO_PLAY1;

        if(pAudio->IsPlaying(14) == false)    
            pAudio->PlaySoundClip(14, 0);
      }

      break;

    case 2://finished playing level 1

      //space bar to acknowledge information from level 1
      //implied that the level is complete
      if(::GetActiveWindow() == data.m_hWnd && keyDown(VK_SPACE) && timeSpaceKey > 0.5){
        m_transitionState++;
        timeSpaceKey = 0;
        bOneShot = false;
      }
      if(pAudio->IsPlaying(15) == false)    
          pAudio->PlaySoundClip(15, 0);

      if(data.m_playerQuit == true || data.m_timeOut == true || data.m_healthOut == true || data.m_kindnessOut == true)
        m_transitionState = 8;

      m_levelScore = (data.m_currentLevel * 10 * data.m_player.m_time) + (data.m_player.m_health * data.m_currentLevel) + (data.m_player.m_kindness * data.m_currentLevel);
      score = data.m_player.m_score + m_levelScore;

      break;
    
    // LEVEL 2
    //***************************
    case 3://start playing level 2
      if(bOneShot == false){
        m_levelScore = (data.m_currentLevel * 10 * data.m_player.m_time) + (data.m_player.m_health * data.m_currentLevel) + (data.m_player.m_kindness * data.m_currentLevel);
        data.m_player.m_score + m_levelScore;

        data.m_currentLevel = 2;
        data.m_player.m_health = 1000;
        data.m_player.m_kindness = 1000;
        data.m_levelComplete = false;
        data.m_gameOver = false;
        //data.m_player.m_score = 0;
        data.m_playerQuit = false;
        data.m_healthOut = false;
        data.m_kindnessOut = false;
        data.m_pObject.clear();
        data.m_grandma.clear();
        data.m_projectiles.clear();
        data.m_player.m_time = 0;
        data.m_timeOut = false;
        if(data.m_weapons[1].rounds > 30)
          data.m_weapons[1].rounds = 30;   
        if(data.m_weapons[2].rounds > 20)
          data.m_weapons[2].rounds = 20;
        data.m_weapons[3].rounds = 0;      
        data.m_weapons[4].rounds = 0;      
        data.m_weapons[5].rounds = 0;
        bOneShot = true;
        m_levelScore = 0;

      }

      //space bar to advance to game play level 2
      if(::GetActiveWindow() == data.m_hWnd && keyDown(VK_SPACE) && timeSpaceKey > 0.5){
        m_transitionState++;
        timeSpaceKey = 0;
        bOneShot = false;
        m_event = EVENT_GO_PLAY2;
        if(pAudio->IsPlaying(14) == false)    
            pAudio->PlaySoundClip(14, 0);

      }

      break;
    case 4://finished playing level 2

      //space bar to acknowledge information from level 2
      if(::GetActiveWindow() == data.m_hWnd && keyDown(VK_SPACE) && timeSpaceKey > 0.5){
        m_transitionState++;
        timeSpaceKey = 0;
        bOneShot = false;
      }
      if(pAudio->IsPlaying(15) == false)    
          pAudio->PlaySoundClip(15, 0);

      if(data.m_playerQuit == true || data.m_timeOut == true || data.m_healthOut == true || data.m_kindnessOut == true)
        m_transitionState = 8;
      
      m_levelScore = (data.m_currentLevel * 10 * data.m_player.m_time) + (data.m_player.m_health * data.m_currentLevel) + (data.m_player.m_kindness * data.m_currentLevel);
      score = data.m_player.m_score + m_levelScore;
      break;

    // LEVEL 3
    //***************************
    case 5://start playing level 3
      if(bOneShot == false){
        m_levelScore = (data.m_currentLevel * 10 * data.m_player.m_time) + (data.m_player.m_health * data.m_currentLevel) + (data.m_player.m_kindness * data.m_currentLevel);
        data.m_player.m_score + m_levelScore;

        data.m_currentLevel = 3;
        data.m_player.m_health = 1000;
        data.m_player.m_kindness = 1000;
        data.m_levelComplete = false;
        data.m_gameOver = false;     
        data.m_playerQuit = false;
        data.m_healthOut = false;
        data.m_kindnessOut = false;
        data.m_pObject.clear();
        data.m_grandma.clear();
        data.m_projectiles.clear();
        data.m_timeOut = false;
        data.m_player.m_time = 0;
        if(data.m_weapons[1].rounds > 60)
          data.m_weapons[1].rounds = 60;   
        if(data.m_weapons[2].rounds > 40)
          data.m_weapons[2].rounds = 40;
        if(data.m_weapons[3].rounds > 50)
          data.m_weapons[3].rounds = 50;     
        if(data.m_weapons[4].rounds > 10)
          data.m_weapons[4].rounds = 10;      
        data.m_weapons[5].rounds = 0;
        bOneShot = true;
        m_levelScore = 0;

      }

      //space bar to advance to game play level 3
      if(::GetActiveWindow() == data.m_hWnd && keyDown(VK_SPACE) && timeSpaceKey > 0.5){
        m_transitionState++;
        timeSpaceKey = 0;
        bOneShot = false;
        m_event = EVENT_GO_PLAY3;
        if(pAudio->IsPlaying(14) == false)    
            pAudio->PlaySoundClip(14, 0);
      }

      break;

    case 6://finished playing level 3

      //space bar to acknowledge information from level 2
      if(::GetActiveWindow() == data.m_hWnd && keyDown(VK_SPACE) && timeSpaceKey > 0.5){
        m_transitionState = 9;
        timeSpaceKey = 0;
        bOneShot = false;
      }
      if(pAudio->IsPlaying(15) == false)    
          pAudio->PlaySoundClip(15, 0);

      if(data.m_playerQuit == true || data.m_timeOut == true || data.m_healthOut == true || data.m_kindnessOut == true)
        m_transitionState = 8;

      if(data.m_levelComplete == true)
        m_transitionState = 7;
      
      m_levelScore = (data.m_currentLevel * 10 * data.m_player.m_time) + (data.m_player.m_health * data.m_currentLevel) + (data.m_player.m_kindness * data.m_currentLevel);
      score = data.m_player.m_score + m_levelScore;
      break;

    // YOU WIN GAME!!!
    //***************************
    case 7://you win
      
      //space bar to acknowledge you win
      if(::GetActiveWindow() == data.m_hWnd && keyDown(VK_SPACE) && timeSpaceKey > 0.5){
        //m_transitionState++;
        m_event = EVENT_GO_MAIN;
        bFirstTime = false;
        timeSpaceKey = 0;
        bOneShot = false;
        m_transitionState = 0;
        
      }

       if(pAudio->IsPlaying(11) == false)    
         pAudio->PlaySoundClip(11, 0);

      break;

    // YOU LOSE GAME!!!
    //***************************
    case 8://you lose

      //space bar to acknowledge you lose
      if(::GetActiveWindow() == data.m_hWnd && keyDown(VK_SPACE) && timeSpaceKey > 0.5){
        //m_transitionState++;
        timeSpaceKey = 0;
        bOneShot = false;
        m_event = EVENT_GO_MAIN;
        bFirstTime = false;
        m_transitionState = 0;
      }

      break;

    // GAME OVER
    //***************************
    case 9://game over

      //space bar to acknowledge game is over
      if(::GetActiveWindow() == data.m_hWnd && keyDown(VK_SPACE) && timeSpaceKey > 0.5){
        //m_transitionState++;
        timeSpaceKey = 0;
        bOneShot = false;
        m_event = EVENT_GO_MAIN;
        bFirstTime = false;
      }
      break;
  }


/*
  for(int i = 0; i < data.m_pObject.size(); ++i){
    data.m_pObject[i]->Update(timeDiff);
  }*/

  //slows down key press for PageUp and PageDown
  //static double timeSprite = 0;
  //timeSprite += timeDiff;

  //only process keys if this window has focus
  /*
  if(::GetActiveWindow() == data.m_hWnd){// && gTimerKey.getTimer(0.03) == true){
    cam.yaw(0.0f);

    //rudder control
    if(keyDown('Z') == true){
      cam.yaw(-2.0f * timeDiff);
    }
    if(keyDown('X') == true){
      cam.yaw(2.0f * timeDiff);
    }
    
    //walk
    if(keyDown('W') == true){
      cam.walk(1.0f * timeDiff);
      data.m_playerY -= 200.0f * timeDiff;
      if(data.m_playerY < data.m_world_top)
        data.m_playerY = data.m_world_top;
    }
    if(keyDown('S') == true){
      cam.walk(-1.0f * timeDiff);
      data.m_playerY += 200.0f * timeDiff;
      if(data.m_playerY > data.m_world_bottom)
        data.m_playerY = data.m_world_bottom;
    }

    //strafe
    if(keyDown('A') == true){
      cam.strafe(-1.0f * timeDiff);
      data.m_playerX -= 200.0f * timeDiff;
      if(data.m_playerX < data.m_world_left)
        data.m_playerX = data.m_world_left;
    }
    if(keyDown('D') == true){
      cam.strafe(1.0f * timeDiff);
      data.m_playerX += 200.0f * timeDiff;
      if(data.m_playerX > data.m_world_right)
        data.m_playerX = data.m_world_right;
    }

    //walk
    if(keyDown(VK_UP) == true){
      cam.walk(1.0f * timeDiff);
      data.m_playerY -= 1000.0f * timeDiff;
    }
    if(keyDown(VK_DOWN) == true){
      cam.walk(-1.0f * timeDiff);
      data.m_playerY += 1000.0f * timeDiff;
    }

    //strafe
    if(keyDown(VK_LEFT) == true){
      cam.strafe(-1.0f * timeDiff);
      data.m_playerX -= 1000.0f * timeDiff;
    }
    if(keyDown(VK_RIGHT) == true){
      cam.strafe(1.0f * timeDiff);
      data.m_playerX += 1000.0f * timeDiff;
    }

    //editor
    if(keyDown(VK_NEXT) == true  && timeSprite > 0.2){
      m_currentSprite++;
      if(m_currentSprite > con.GetSpriteCount() - 1)
        m_currentSprite = 0;
      gi = con.GetSprite2(m_currentSprite);
   //   if(gi.layer >= 2 && gi.layer <= 6)
        timeSprite = 0.0;
    }
    if(keyDown(VK_PRIOR) == true && timeSprite > 0.2){
      m_currentSprite--;
      if(m_currentSprite < 0)
        m_currentSprite = con.GetSpriteCount() - 1;
      gi = con.GetSprite2(m_currentSprite);
  //    if(gi.layer >= 2 && gi.layer <= 6)
        timeSprite = 0.0;
    }
*/
    

    /*
    //vertical height
    if(keyDown(VK_DOWN) == true){
      position.y -= 5.0f * timeDiff;
      if(position.y < 2.0f)
        position.y = 2.0f;
      cam.setPosition(&position);
    }
    if(keyDown(VK_UP) == true){
      position.y += 5.0f * timeDiff;
      if(position.y > 20.0f)
        position.y = 20.0f;
      cam.setPosition(&position);
    }*/

    //badge test
  /*
    m_badgeAngleX += (1.0f * timeDiff);
    m_badgeAngleZ += (2.0f * timeDiff);

  }*/
/*
  //editor
  static double addTime = 0.0;
  addTime += timeDiff;
  gi = con.GetSprite2(m_currentSprite);
  if(addTime > 0.25 && data.m_bLeftMouseDown == true  ){
    //if(m_mouse.MouseX() >= data.m_world_left && m_mouse.MouseX() <= data.m_world_right && m_mouse.MouseY() >= data.m_world_top && m_mouse.MouseY() <= data.m_world_bottom){
      if(gi.width == gi.height)
        data.AddGameObject(gi, m_mouse.MouseX() - gi.width/2, m_mouse.MouseY() - gi.height/2);//square
      else
        data.AddGameObject(gi, m_mouse.MouseX(), m_mouse.MouseY()); //none square
      addTime = 0.0;
    //}
  }
*/

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
void cGameStateControl::render(CGraphics &con, Camera &cam, CGameData &data){
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
  con.m_d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 140, 0), 1.0f, 0);
  con.m_d3ddev->BeginScene();    // begins the 3D scene

  //display content based on transition state
   switch(m_transitionState){
    case 0://enter from main
      con.Print("Cindy and the Cookie Stealing Ogre", F_V20, 380, 10, 255, 255, 255, 255);      

      con.Print("Cindy is a Rainbow Scout. She has sold 99 boxes of Rainbow Scout Cookies.  She needed to sell 100 boxes", F_V20, 10, 50, 0, 255, 0);
      con.Print("in order to receive her Rainbow Scout \"Little Miss\" badge.  Cindy was standing in front of the local", F_V20, 10, 70, 0, 255, 0);
      con.Print("grocery store selling cookies. SUDDENLY a mean Ogre came running past her stealing her very last box of", F_V20, 10, 90, 0, 255, 0);
      con.Print("cookies.  Cindy began yelling obscenities and ran in pursuit of the Ogre. She chased him into the Enchanted", F_V20, 10, 110, 0, 255, 0);
      con.Print("forest.", F_V20, 10, 130, 0, 255, 0);
      con.Print("You are Cindy. Your goal is to get past the flesh eating elves,      fairies,       and witches       .  ", F_V20, 10, 160, 0, 255, 255);
      gi = con.GetSprite(90);
      con.RenderGraphicModulate(gi, 542, 155, 255, 255, 255);
      gi = con.GetSprite(91);
      con.RenderGraphicModulate(gi, 642, 155, 255, 255, 255);
      gi = con.GetSprite(92);
      con.RenderGraphicModulate(gi, 782, 155, 255, 255, 255);

      con.Print("You must work through 3 levels. In levels 1 and 2 you must find the gate         out of the level.", F_V20, 10, 200, 0, 255, 255);
      con.Print("In level 3 you must get very close to the Ogre            and he will return the box of cookies.", F_V20, 10, 240, 0, 255, 255);
      
      con.Print("You must be very careful. Watch the status of the Time remaining, Health, and Kindness.  Health is reduced", F_V20, 10, 280, 0, 255, 255);
      con.Print("when the elves, fairies, and witches bite Cindy. Kindness is reduced each time Cindy uses a weapon.", F_V20, 10, 300, 0, 255, 255);
      con.Print("You can increase your health by collecting first aid.      You can increase your kindness by visiting.", F_V20, 10, 320, 0, 255, 255);
      con.Print("Grandma       .  Kindness increases as long as you stand close to here. The white circle will flash.  You can", F_V20, 10, 340, 0, 255, 255);
      con.Print("still shoot all weapons while visiting Grandma. Grandma cannot be harmed by your weapons.", F_V20, 10, 360, 0, 255, 255);

      gi = con.GetSprite(111);
      con.RenderGraphicModulate(gi, 450, 315, 255, 255, 255);//health
      gi = con.GetSprite(112);
      con.RenderGraphicModulate(gi, 89, 330, 255, 255, 255);//grandma
      gi = con.GetSprite(113);
      con.RenderGraphicModulate(gi, 417, 220, 255, 255, 255);//ogre
      gi = con.GetSprite(83);
      con.RenderGraphicModulate(gi, 662, 190, 255, 255, 255);//door


      con.Print("Use WASD keys to move around. Cindy is only 10 years-old so she doesn't run very fast.  Use the Arrow keys", F_V20, 10, 400, 0, 255, 255);
      con.Print("to fire weapons.  Spacebar may also be used to fire weapons.  Keys 1, 2, 3, 4, and 5 may be used to select", F_V20, 10, 420, 0, 255, 255);
      con.Print("the weapons: 9mm pistol,     shotgun,     machine gun,     rocket launcher,     and flamethrower     .", F_V20, 10, 440, 0, 255, 255);

      gi = con.GetSprite(75);
      con.RenderGraphicModulate(gi, 242, 435, 255, 255, 255);
      gi = con.GetSprite(76);
      con.RenderGraphicModulate(gi, 342, 435, 255, 255, 255);
      gi = con.GetSprite(77);
      con.RenderGraphicModulate(gi, 487, 435, 255, 255, 255);
      gi = con.GetSprite(78);
      con.RenderGraphicModulate(gi, 662, 435, 255, 255, 255);
      gi = con.GetSprite(79);
      con.RenderGraphicModulate(gi, 862, 435, 255, 255, 255);

      con.Print("Ammo for pistol         shotgun       and machine gun.", F_V20, 10, 480, 0, 255, 255);
      gi = con.GetSprite(80);
      con.RenderGraphicModulate(gi, 145, 470, 255, 255, 255);
      gi = con.GetSprite(81);
      con.RenderGraphicModulate(gi, 270, 470, 255, 255, 255);
      gi = con.GetSprite(82);
      con.RenderGraphicModulate(gi, 470, 470, 255, 255, 255);
      
      con.Print("Press Spacebar to play...", F_V20, 410, 540, 255, 255, 255, 255);
/*
      con.Print("", F_V20, 10, 50, 0, 255, 255);
      con.Print("", F_V20, 10, 50, 0, 255, 255);
      con.Print("", F_V20, 10, 50, 0, 255, 255);
      con.Print("", F_V20, 10, 50, 0, 255, 255);
      con.Print("", F_V20, 10, 50, 0, 255, 255);
*/

      break;

    // LEVEL 1
    //***************************
    case 1://start playing level 1
      con.Print("Level 1", F_V20, 450, 10, 255, 255, 255, 255);
      con.Print("Kill elves. Practice with all 5 weapons. Watch your health and kindness. Remember to visit Grandma. =)", F_V20, 40, 200, 255, 255, 255);
      con.Print("Press spacebar to continue...", F_V20, 410, 220, 255, 255, 255, 255);

      break;
    case 2://finished playing level 1
      con.Print("Level 1 Complete!", F_V20, 410, 200, 255, 255, 255, 255);
      con.Print("Press Spacebar to continue...", F_V20, 360, 220, 255, 255, 255, 255);
    
      break;
    
    // LEVEL 2
    //***************************
    case 3://start playing level 2
      con.Print("Level 2", F_V20, 450, 10, 255, 255, 255, 255);
      con.Print("Kill elves and fairies. The fairies fly quickly and love to bite. Watch your health. =) Your kindess will decrease", F_V20, 40, 200, 255, 255, 255);
      con.Print("so make sure to visit grandma. You may still fire weapons while visiting. She likes to watch. =)", F_V20, 40, 220, 255, 255, 255);
      con.Print("Press spacebar to continue...", F_V20, 410, 240, 255, 255, 255, 255);

      break;

    case 4://finished playing level 2
      
      con.Print("Level 2 Complete!", F_V20, 410, 200, 255, 255, 255, 255);
      con.Print("Press Spacebar to continue...", F_V20, 360, 220, 255, 255, 255, 255);

      break;

    // LEVEL 3
    //***************************
    case 5://start playing level 3
      con.Print("Level 3", F_V20, 450, 10, 255, 255, 255, 255);
      con.Print("Kill elves, fairies, and witches. The witches fly in spiral patterns.  If you get mauled use the flamethrower. =)", F_V20, 40, 200, 255, 255, 255);
      con.Print("Pick up first aid as often as possible. You'll get bitten a lot. You'll need rockets and flamethrowers for this one.", F_V20, 40, 220, 255, 255, 255);
      con.Print("Press spacebar to continue...", F_V20, 410, 240, 255, 255, 255, 255);

      break;

    case 6://finished playing level 3
      con.Print("Level 3 Complete!", F_V20, 410, 200, 255, 255, 255, 255);
      con.Print("Press Spacebar to continue...", F_V20, 360, 220, 255, 255, 255, 255);

      break;

    // YOU WIN GAME!!!
    //***************************
    case 7://you win
      con.Print("You WIN! You are awarded 'Little Miss' Rainbow Scout. Congratulations!", F_V20, 410, 200, 255, 0, 0, 255);
      gi = con.GetSprite(114);
      con.RenderGraphicModulate(gi, 160, 285, 255, 255, 255);//Little Miss
      
      con.Print("Press Spacebar to continue...", F_V20, 410, 620, 255, 0, 0, 255);

      break;

    // YOU LOSE GAME!!!
    //***************************
    case 8://you lose
      con.Print("You LOSE!!!", F_V20, 450, 110, 255, 0, 0, 255);

      gi = con.GetSprite(115);
      con.RenderGraphicModulate(gi, 200, 140, 255, 255, 255);//big ogre

      con.Print("Press Spacebar to continue...", F_V20, 360, 700, 255, 0, 0, 255);

      if(data.m_playerQuit == true){
        con.Print("Player is a quitter!", F_V20, 410, 250, 255, 0, 0, 255);         
      }
      else if(data.m_timeOut == true){
        con.Print("Exceeded time!", F_V20, 410, 250, 255, 0, 0, 255);         
      }
      else if(data.m_healthOut == true){
        con.Print("Health problems!", F_V20, 410, 250, 255, 0, 0, 255);         
      }
      else if(data.m_kindnessOut == true){
        con.Print("Mean girl!", F_V20, 410, 250, 255, 0, 0, 255);         
      }

      break;

    // GAME OVER
    //***************************
    case 9://game over
      con.Print("GAME OVER", F_V16, 510, 10, 255, 0, 0, 255);
      con.Print("Press spacebar to continue...", F_V16,510, 30, 255, 0, 0, 255);

      break;
  }
  
  /*  
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
        temp.rx = m_badgeAngleX;
        temp.ry = m_badgeAngleZ;
        con.RenderMesh(temp);
      }
      else
        con.RenderMesh(temp);//.meshID, temp.rx, temp.ry, temp.rz);
    }
  }
  */  
  
  //print text to display
  if(data.m_displayDebugInfo == true){
    //con.Print("CONTROL STATE - Version", data.m_version, F_V16, 10, 10,  255, 255, 255, 255);
    //con.Print("FPS", data.m_FPS , F_V16, 10, 10,  255, 255, 0, 255);
  }
  //con.Print("Press ESC to quit", F_V16, data.m_screenWidth/2 - 100, data.m_screenHeight/2 + 200, 255, 0, 0, 255);

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

void cGameStateControl::processEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
}

void cGameStateControl::deactivate(){
  CAudioManager *pAudio = CAudioManager::Instance();
  pAudio->KillAll();
//  Delete<psys::PSystem*>( Gun );
  
}
void cGameStateControl::resume(){
}
void cGameStateControl::pause(){
}
void cGameStateControl::save(){
}

void cGameStateControl::addTransitionEvent(int event, cGameStateObject* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}
