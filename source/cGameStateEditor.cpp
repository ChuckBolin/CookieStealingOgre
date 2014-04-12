#include "..\include\cGameStateEditor.h"
 
cGameStateEditor::cGameStateEditor(){
}

cGameStateEditor::~cGameStateEditor()
{
	m_TE.clear();
}

void cGameStateEditor::initialize(){
  m_viewChoice = 0;
  ::ShowCursor(true);
  m_currentSprite = 20;

  m_workViewX = 8;
  m_workViewY = 50;
  m_gridSpace = 32;
  m_lastGridSpace = 0;
  m_currentObjectID = 2;
  m_gridOn = true;
  m_snapOn = true;
  m_panelX = 0;
  m_panelY = 0;
  m_editorX = 50;
  m_editorY = 30;
  m_angle = 0;//.78;
  m_fileBarX = 50;
  m_fileBarY = 500;

  m_bSave = false;
  m_bClear = false;
  m_bOpen = false;
  m_bUndo = false;
  m_bFillGround = false;
}

void cGameStateEditor::activate(CGameData &data){
  data.m_pObject.clear();
  m_bFillGround = false;
  m_currentObjectID = 3;
  m_gridOn = true;
  m_snapOn = true;
}

//***************************************************************
//                         U P D A T E
//***************************************************************
cGameStateObject* cGameStateEditor::update(double timeDifference, Camera &cam, CGameData &data, CGraphics &con){
  m_event = EVENT_GO_NO_WHERE;
  static bool bStop = false;
  D3DXVECTOR3 position;
  GRAPHIC_IMAGE gi;
  GAME_OBJECT go;
  std::ostringstream oss;

  CLog *pLog = CLog::Instance(); 

  //play audio file at start of game state
  CAudioManager *pAudio = CAudioManager::Instance();
  if(pAudio->IsPlaying(0) == false)    
    pAudio->PlaySoundClip(0, 1);

  //configure mouse
  m_mouse.SetHandle(data.m_hWnd);

  //set aircraft camera to 1000 feet
  static bool bFirstTime = true;
  if(bFirstTime == true){
    position.x = 0.0f; position.y = 3.0f; position.z = 0.0f;
    cam.setPosition(&position); 
    cam.roll(0.0f);

    //define position of editor control panel
    gi = con.GetSprite(42);
    m_panelX = con.GetScreenWidth() - 50 - gi.width;
    m_panelY = 50;
    gi = con.GetSprite(47);
    m_fileBarY = con.GetScreenHeight() - 50 - gi.height;
    bFirstTime = false;
    m_viewChoice = 4;
    m_editLevel = 0;
    //data.LoadGameLevel("data//level1.dat", con);
  }

  //escape state by pressing ESC key
  if(::GetActiveWindow() == data.m_hWnd && keyDown(VK_ESCAPE))
    m_event = EVENT_GO_MAIN;

  cam.getPosition(&position);
  static double timeDiff = 0;
  timeDiff = timeDifference;

  for(int i = 0; i < data.m_pObject.size(); ++i){
    data.m_pObject[i]->Update(timeDiff);
  }

  //slows down key press for PageUp and PageDown
  static double timeSprite = 0;
  timeSprite += timeDiff;

  //only process keys if this window has focus
  if(::GetActiveWindow() == data.m_hWnd){// && gTimerKey.getTimer(0.03) == true){
    cam.yaw(0.0f);

    //rotate sprite
    if(keyDown('X') == true   && timeSprite > 0.05){
      cam.yaw(-2.0f * timeDiff);
      m_angle += 0.05f;
      if(m_angle > 6.28)
        m_angle = 0.0f;
      timeSprite = 0;
    }
    if(keyDown('Z') == true   && timeSprite > 0.05){
      cam.yaw(2.0f * timeDiff);
      m_angle -= 0.05f;
      if(m_angle < 0.0f)
        m_angle = 6.28f;
      timeSprite = 0;
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
      if(data.m_playerY < data.m_world_top)
        data.m_playerY = data.m_world_top;
    }
    if(keyDown(VK_DOWN) == true){
      cam.walk(-1.0f * timeDiff);
      data.m_playerY += 1000.0f * timeDiff;
      if(data.m_playerY > data.m_world_bottom)
        data.m_playerY = data.m_world_bottom;
    }

    //strafe
    if(keyDown(VK_LEFT) == true){
      cam.strafe(-1.0f * timeDiff);
      data.m_playerX -= 1000.0f * timeDiff;
      if(data.m_playerX < data.m_world_left)
        data.m_playerX = data.m_world_left;
    }
    if(keyDown(VK_RIGHT) == true){
      cam.strafe(1.0f * timeDiff);
      data.m_playerX += 1000.0f * timeDiff;
      if(data.m_playerX > data.m_world_right)
        data.m_playerX = data.m_world_right;
    }

    //editor
    if(keyDown(VK_NEXT) == true  && timeSprite > 0.2){//page up
      m_currentObjectID++;
      if(m_currentObjectID > data.m_objectType.size() - 1)
        m_currentObjectID = 0;
        timeSprite = 0.0;
    }
    if(keyDown(VK_PRIOR) == true && timeSprite > 0.2){//page down
      m_currentObjectID--;
      if(m_currentObjectID < 0)
        m_currentObjectID = data.m_objectType.size() - 1;
        timeSprite = 0.0;
    }

    //UNDO
    if(keyDown('U') == true && timeSprite > 0.15){
      if(data.m_pObject.size() > 0)
        data.m_pObject.pop_back();
      timeSprite = 0.0;
    }

    //fill ground with grass
    if(keyDown('G') == true && m_bFillGround == false && timeSprite > 0.15){
      go = data.GetObjectInfo(3);
      gi = con.GetSprite(go.spriteID);
      go.alive = true;
      go.alpha = gi.alpha;
      go.angle = gi.angle;
      go.maxFrames = gi.maxFrames;
      go.updateInterval = gi.updateInterval;
      go.frameCount = 0;
      go.width = gi.width;
      go.height = gi.height;

      for(int i = 0; i < 20; ++i){
        for(int j = 0; j < 20; ++j){
          go.pos.x = i * 128;
          go.pos.y = j * 128;
          data.AddGameObject(go);        
        }
      }
      timeSprite = 0.0;
      m_bFillGround = true;
    }
  }

  //mouse moves the game world
    //strafe
    if(::GetActiveWindow() == data.m_hWnd && m_mouse.MouseX() < 50){
      cam.strafe(-1.0f * timeDiff);
      data.m_playerX -= 1000.0f * timeDiff;
      if(data.m_playerX < data.m_world_left)
        data.m_playerX = data.m_world_left;
    }
    if(::GetActiveWindow() == data.m_hWnd && m_mouse.MouseX() > con.GetScreenWidth() - 50){
      cam.strafe(1.0f * timeDiff);
      data.m_playerX += 1000.0f * timeDiff;
      if(data.m_playerX > data.m_world_right)
        data.m_playerX = data.m_world_right;
    }
    if(::GetActiveWindow() == data.m_hWnd && m_mouse.MouseY() < 50) {
      cam.walk(1.0f * timeDiff);
      data.m_playerY -= 1000.0f * timeDiff;
      if(data.m_playerY < data.m_world_top)
        data.m_playerY = data.m_world_top;
    }
    if(::GetActiveWindow() == data.m_hWnd && m_mouse.MouseY() > con.GetScreenHeight() - 50){
      cam.walk(-1.0f * timeDiff);
      data.m_playerY += 1000.0f * timeDiff;
      if(data.m_playerY > data.m_world_bottom)
        data.m_playerY = data.m_world_bottom;
    }

  //editor
  //mouse affects editor control panel
  gi = con.GetSprite(42);
  bool bPanel = false;

  if(m_mouse.MouseX() > m_panelX && m_mouse.MouseX() < m_panelX + gi.width && m_mouse.MouseY() > m_panelY && m_mouse.MouseY() < m_panelY + gi.height){
    bPanel = true;
  }

  //is mouse on file bar
  gi = con.GetSprite(47);
  bool bFileBar = false;
  if(m_mouse.MouseX() > m_fileBarX && m_mouse.MouseX() < m_fileBarX + gi.width && m_mouse.MouseY() > m_fileBarY && m_mouse.MouseY() < m_fileBarY + gi.height){
    bFileBar = true;
  }

  //mouse click on editor panel
  if(data.m_bLeftMouseDown == true && bPanel == true){
    
    //turn grid on
    if(m_mouse.MouseX() > m_panelX + 99 && m_mouse.MouseX() < m_panelX + 157 &&
       m_mouse.MouseY() > m_panelY + 554 && m_mouse.MouseY()< m_panelY + 592)
      m_gridOn = true;

    //turn grid off
    if(m_mouse.MouseX() > m_panelX + 158 && m_mouse.MouseX() < m_panelX + 224 &&
       m_mouse.MouseY() > m_panelY + 554 && m_mouse.MouseY()< m_panelY + 592)
      m_gridOn = false;

    //turn snap on
    if(m_mouse.MouseX() > m_panelX + 99 && m_mouse.MouseX() < m_panelX + 157 &&
      m_mouse.MouseY() > m_panelY + 593 && m_mouse.MouseY()< m_panelY + 641 && m_snapOn == false){
      m_snapOn = true;
      }

    //turn snap off
    if(m_mouse.MouseX() > m_panelX + 158 && m_mouse.MouseX() < m_panelX + 224 &&
      m_mouse.MouseY() > m_panelY + 593 && m_mouse.MouseY()< m_panelY + 641 && m_snapOn == true){
      m_snapOn = false;
      }

    //32 pixel grid spacing
    if(m_mouse.MouseX() > m_panelX + 28 && m_mouse.MouseX() < m_panelX + 84 &&
      m_mouse.MouseY() > m_panelY + 642 && m_mouse.MouseY()< m_panelY + 686 ){
       m_gridSpace = 32;
      }

    //64 pixel grid spacing
    if(m_mouse.MouseX() > m_panelX + 85 && m_mouse.MouseX() < m_panelX + 145 &&
      m_mouse.MouseY() > m_panelY + 642 && m_mouse.MouseY()< m_panelY + 686){
       m_gridSpace = 64;
      }

    //128 pixel grid spacing
    if(m_mouse.MouseX() > m_panelX + 146 && m_mouse.MouseX() < m_panelX + 217 &&
      m_mouse.MouseY() > m_panelY + 642 && m_mouse.MouseY()< m_panelY + 686){
       m_gridSpace = 128;
      }

    //uses mouse to select new object type
    if(m_mouse.MouseX() > m_panelX + 28 && m_mouse.MouseX() < m_panelX + 242){
      for(int i = 0; i < data.m_objectType.size(); ++i){
        if(m_mouse.MouseY() > m_panelY + 67 + (i * 16) && m_mouse.MouseY() < m_panelY + 81 + (i * 16)){
          m_currentObjectID = i;
        }
      }
    }
  }

  //look for mouse click on file bar
  if(data.m_bLeftMouseDown == true && bFileBar == true && timeSprite > 0.25){
    //clear game world
    if(m_mouse.MouseX() > m_fileBarX + 18 && m_mouse.MouseX() < m_fileBarX + 103 &&
      m_mouse.MouseY() > m_fileBarY + 15 && m_mouse.MouseY() < m_fileBarY + 55 && m_bClear == false ){
      //m_bClear = true;
      data.m_pObject.clear();
      m_bFillGround = false;
    }

    //open level file 1
    if(m_mouse.MouseX() > m_fileBarX + 195 && m_mouse.MouseX() < m_fileBarX + 217 &&
      m_mouse.MouseY() > m_fileBarY + 15 && m_mouse.MouseY() < m_fileBarY + 55 && m_bOpen == false ){
        m_editLevel = 1;
        data.m_pObject.clear();
        data.LoadGameLevel("data//level1.dat", con);
    }

    //open level file 2
    if(m_mouse.MouseX() > m_fileBarX + 223 && m_mouse.MouseX() < m_fileBarX + 245 &&
      m_mouse.MouseY() > m_fileBarY + 21 && m_mouse.MouseY() < m_fileBarY + 51 && m_bOpen == false ){
        m_editLevel = 2;
        data.m_pObject.clear();
        data.LoadGameLevel("data//level2.dat", con);
    }

    //open level file 3
    if(m_mouse.MouseX() > m_fileBarX + 251 && m_mouse.MouseX() < m_fileBarX + 273 &&
      m_mouse.MouseY() > m_fileBarY + 21 && m_mouse.MouseY() < m_fileBarY + 51 && m_bOpen == false ){
        m_editLevel = 3;
        data.m_pObject.clear();
        data.LoadGameLevel("data//level3.dat", con);
    }

    //save level file
    if(m_mouse.MouseX() > m_fileBarX + 439 && m_mouse.MouseX() < m_fileBarX + 524 &&
      m_mouse.MouseY() > m_fileBarY + 21 && m_mouse.MouseY() < m_fileBarY + 51 && m_bSave == false ){
      if(m_editLevel == 1)
        data.SaveGameLevel(data.GetLevelFilename(1));
      else if(m_editLevel == 2)
        data.SaveGameLevel(data.GetLevelFilename(2));
      else if(m_editLevel == 3)
        data.SaveGameLevel(data.GetLevelFilename(3));
    }

    //undo last game object
    if(m_mouse.MouseX() > m_fileBarX + 515 && m_mouse.MouseX() < m_fileBarX + 590 &&
      m_mouse.MouseY() > m_fileBarY + 15 && m_mouse.MouseY() < m_fileBarY + 55 && m_bUndo == false ){
      if(data.m_pObject.size() > 0)
        data.m_pObject.pop_back();
      //m_bUndo = true;
    }

    timeSprite = 0;
  }

  //allow user to move around the game world quickly
  static bool bMap = false;
  gi = con.GetSprite(45);
  m_mapX = m_editorX + gi.width + 20;
  m_mapY = m_editorY + 30;
  if(m_mouse.MouseX() > m_mapX && m_mouse.MouseX() < m_mapX + 80 && m_mouse.MouseY() > m_mapY && m_mouse.MouseY() < m_mapY + 80){
    bMap = true;
  }
  else
    bMap = false;

  if(data.m_bLeftMouseDown == true && bMap == true){
    if((m_mouse.MouseX() - m_mapX) * 32 > 0 && (m_mouse.MouseY() - m_mapY) * 32 > 0){
      data.m_playerX = (m_mouse.MouseX() - m_mapX) * 32;
      data.m_playerY = (m_mouse.MouseY() - m_mapY) * 32;      
    }
  }

  //add game objects to level
  static double addTime = 0.0;
  addTime += timeDiff;
  int posX = 0;
  int posY = 0;
  if(m_snapOn == true){
    posX = (int)(data.m_playerX - con.GetScreenCenterX() + m_mouse.MouseX()) / m_gridSpace;
    posY = (int)(data.m_playerY - con.GetScreenCenterY() + m_mouse.MouseY()) / m_gridSpace;
  }
  else{
    posX = (int)(data.m_playerX - con.GetScreenCenterX() + m_mouse.MouseX());// / m_gridSpace;
    posY = (int)(data.m_playerY - con.GetScreenCenterY() + m_mouse.MouseY());// / m_gridSpace;
  }
  static int lastPosX = -100;
  static int lastPosY = -100;

  //mouse click NOT on editor panel
  if(data.m_bLeftMouseDown == true && bPanel == false && bFileBar == false && bMap == false){
    go = data.GetObjectInfo(m_currentObjectID);
    if( posX == lastPosX && posY == lastPosY){
      //do nothing if mouse cursor has not moved since last mouse click
    }
    else if(go.spriteID > -1){// && (posX < lastPosX || posX > lastPosX) && (posY < lastPosY || posY > lastPosY)){ //don't place tiles on top of last tile
      gi = con.GetSprite(go.spriteID);
      go.alive = true;
      go.alpha = gi.alpha;
      go.angle = gi.angle;
      go.maxFrames = gi.maxFrames;
      go.updateInterval = gi.updateInterval;
      go.frameCount = 0;
      go.width = gi.width;
      go.height = gi.height;
      if(m_snapOn == true){
        go.pos.x = posX  * m_gridSpace;
        go.pos.y = posY  * m_gridSpace;
      }
      else{
        go.pos.x = posX;//  * m_gridSpace;
        go.pos.y = posY;//  * m_gridSpace;
      }
      if(gi.spriteID == 90)//elf
        go.speed = 10 + rand() % 35;
      else if(gi.spriteID == 91)//fairy
        go.speed = 35 + rand() % 50;
      else if(gi.spriteID = 92) //witch
        go.speed = 60 + rand() % 70;
      
      //add only if mouse position is within world limits
      if(go.pos.x < data.m_world_right && go.pos.x >= data.m_world_left && go.pos.y >= data.m_world_top && go.pos.y < data.m_world_bottom){
        lastPosX = posX;
        lastPosY = posY;
        //pLog->Log("Pos",go.pos.x, go.pos.y);
        go.totalTime = 0;
        data.AddGameObject(go);
      }
    }
    addTime = 0.0;
  }

  //delete item in game level
  int nDelete;
  int nStore;
  bool bDeleted = false;
  if(data.m_bRightMouseDown == true && addTime > 0.06 && bPanel == false && bFileBar == false && bMap == false){// && addTime > 0.25){
    posX = (int)(data.m_playerX - con.GetScreenCenterX() + m_mouse.MouseX());
    posY = (int)(data.m_playerY - con.GetScreenCenterY() + m_mouse.MouseY());
    
    nStore = -1;
    for(int i = 0; i < data.m_screen.size(); ++i){
      nDelete = data.m_screen[i];

      if(posX > data.m_pObject[nDelete]->go.pos.x && posX < data.m_pObject[nDelete]->go.pos.x + data.m_pObject[nDelete]->go.width &&
       posY > data.m_pObject[nDelete]->go.pos.y && posY < data.m_pObject[nDelete]->go.pos.y + data.m_pObject[nDelete]->go.height)
      {
         if(data.m_pObject[nDelete]->go.layer < 4){
           nStore = nDelete; //continue on
         }
         else{
           //data.m_pObject.erase(data.m_pObject.begin() + nDelete, data.m_pObject.begin() + nDelete + 1); 
           data.m_pObject[nDelete]->go.alive = false;
           bDeleted = true;
           break;
         }
      }    
     // if(bDeleted == true)
     //   break;
    }

    if(bDeleted == false && nStore > -1){
      //data.m_pObject.erase(data.m_pObject.begin() + nStore, data.m_pObject.begin() + nStore + 1);     
      data.m_pObject[nDelete]->go.alive = false;
    }
    addTime = 0.0;
  }


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
void cGameStateEditor::render(CGraphics &con, Camera &cam, CGameData &data){
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

    gi = con.GetSprite(42);
    m_panelX = con.GetScreenWidth() - 50 - gi.width;
    m_panelY = 50;
    gi = con.GetSprite(47);
    m_fileBarY = con.GetScreenHeight() - 50 - gi.height;

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
    
  //render grid points
  if(m_gridOn == true){
    int maxPoints = (2560 / m_gridSpace) + 1;
    gi = con.GetSprite(40);
    float px = 0;
    float py = 0;
    int lowerX = 0;
    int upperX = 0;
    int lowerY = 0;
    int upperY = 0;
    int w = con.GetScreenWidth()/2/m_gridSpace;
    int h = con.GetScreenHeight()/2/m_gridSpace;
    lowerX = data.m_playerX/m_gridSpace - w;
    upperX = data.m_playerX/m_gridSpace + w;
    lowerY = data.m_playerY/m_gridSpace - h;
    upperY = data.m_playerY/m_gridSpace + h;
    if(m_gridSpace > 1){
      for(int i = 0; i < maxPoints; ++i){
        for(int j = 0; j < maxPoints; ++j){
          px = con.GetScreenWidth()/2 + ((i * m_gridSpace) - data.m_playerX);
          py = con.GetScreenHeight()/2 + ((j * m_gridSpace) - data.m_playerY);
          if(i > lowerX && i <= upperX && j > lowerY && j <= upperY)
            con.DrawRect(px, py, px+1, py+1, 0, 255, 255);
        }
      }
    }
  }
  
  //********************************************************************************************
  // R E N D E R              R E N D E R               R E N D E R              R E N D E R  //
  //********************************************************************************************
  //render all objects
  int w = con.GetScreenWidth()/2;
  int h = con.GetScreenHeight()/2;
  int px = data.m_playerX;
  int py = data.m_playerY;

  data.m_screen.clear(); //clear before each render
  for(int j = 2; j < 5; ++j){
    for(int i = 0; i < data.m_pObject.size(); ++i){
      if(data.m_pObject[i]->go.layer == j && data.m_pObject[i]->go.alive == true ){
        if(data.m_pObject[i]->go.pos.x > px - w - 128  && data.m_pObject[i]->go.pos.x < px + w &&
          data.m_pObject[i]->go.pos.y > py - h - 128 && data.m_pObject[i]->go.pos.y < py + h){
          con.RenderGraphicModulate(data.m_pObject[i]->go, con.GetScreenWidth()/2 + (data.m_pObject[i]->go.pos.x - data.m_playerX), con.GetScreenHeight()/2 + (data.m_pObject[i]->go.pos.y - data.m_playerY), 255, 255, 255);
          data.m_screen.push_back(i); //add this to list of things on the screen
          }
      }
    }
  }

  //draws cindy's face as a graphic place holder
  //gi = con.GetSprite(21);
  //gi.scale = 0.75f/2.0f;//pos.y;
  //con.RenderGraphicModulate(gi, con.GetScreenWidth()/2 - (gi.scale * gi.width)/2 , con.GetScreenHeight()/2 - (gi.scale * gi.height)/2, 255, 255, 255);

  for(int j = 5; j < 9; ++j){
    for(int i = 0; i < data.m_pObject.size(); ++i){
      if(data.m_pObject[i]->go.layer == j && data.m_pObject[i]->go.alive == true){
        if(data.m_pObject[i]->go.pos.x > px - w - 128  && data.m_pObject[i]->go.pos.x < px + w &&
          data.m_pObject[i]->go.pos.y > py - h - 128 && data.m_pObject[i]->go.pos.y < py + h){
          con.RenderGraphicModulate(data.m_pObject[i]->go, con.GetScreenWidth()/2 + (data.m_pObject[i]->go.pos.x - data.m_playerX), con.GetScreenHeight()/2 + (data.m_pObject[i]->go.pos.y - data.m_playerY), 255, 255, 255);
          data.m_screen.push_back(i); //add this to list of things on the screen
          }
      }
    }
  }

  //draws player position...always (280, 280)
  gi = con.GetSprite(21);
  gi.scale = 0.75f/2.0f;//pos.y;
  con.RenderGraphicModulate(gi, con.GetScreenWidth()/2 + (280 - data.m_playerX), con.GetScreenHeight()/2 + (280 - data.m_playerY), 255, 255, 255);
  con.DrawCircle(con.GetScreenWidth()/2 + (290 - data.m_playerX), con.GetScreenHeight()/2 + (290 - data.m_playerY),0,255,255,20);
  
  //draws a circle around Cindy's position
  //con.DrawCircle(data.m_screenWidth/2, data.m_screenHeight/2, 155, 155, 155, 40);
  //con.DrawCircle(data.m_screenWidth/2, data.m_screenHeight/2, 200, 200, 200, 44);
  //con.DrawCircle(data.m_screenWidth/2, data.m_screenHeight/2, 255, 255, 255, 48);
  
  //draw rectangle around screen
  con.DrawRect(3, 3, data.m_screenWidth - 6, data.m_screenHeight - 6, 255, 255, 255);

  //sprite attached to mouse
  go = data.GetObjectInfo(m_currentObjectID);
  if(go.spriteID > -1){
    gi = con.GetSprite(go.spriteID);
    go.angle = 0; //m_angle;
    go.pos.x = m_mouse.MouseX();
    go.pos.y = m_mouse.MouseY();
    con.RenderGraphicModulate(go, 255,255,255);
  }

  //draw control panel
  gi = con.GetSprite(42);
  con.RenderGraphicModulate(gi, m_panelX, m_panelY, 255, 255, 255);

  //draw file bar
  gi = con.GetSprite(47);
  con.RenderGraphicModulate(gi, m_fileBarX, m_fileBarY, 255, 255,255);

  //draw rectangles on top of control panel
  if(m_gridOn == true)
    con.DrawRect((float)(m_panelX + 99), (float)(m_panelY + 554), (float)(m_panelX + 157),(float)(m_panelY + 592),255,0,0);
  else
    con.DrawRect((float)(m_panelX + 158), (float)(m_panelY + 554), (float)(m_panelX + 225),(float)(m_panelY + 592),255,0,0);

  if(m_snapOn == true)
    con.DrawRect((float)(m_panelX + 99), (float)(m_panelY + 593), (float)(m_panelX + 157),(float)(m_panelY + 635),255,0,0);
  else
    con.DrawRect((float)(m_panelX + 158), (float)(m_panelY + 593), (float)(m_panelX + 225),(float)(m_panelY + 635),255,0,0);

  if(m_gridSpace == 32)
    con.DrawRect((float)(m_panelX + 28), (float)(m_panelY + 642), (float)(m_panelX + 84),(float)(m_panelY + 686),255,0,0);
  else if(m_gridSpace == 64)
    con.DrawRect((float)(m_panelX + 85), (float)(m_panelY + 642), (float)(m_panelX + 145),(float)(m_panelY + 686),255,0,0);
  else if(m_gridSpace == 128)
    con.DrawRect((float)(m_panelX + 146), (float)(m_panelY + 642), (float)(m_panelX + 217),(float)(m_panelY + 686),255,0,0);

  //render list of available game objects
  for(int i = 0; i < data.m_objectType.size(); ++i){
    if(i < 30)
      con.Print(data.m_objectType[i].name, F_V16B, m_panelX + 16, m_panelY + 65 + (i * 16), 255, 255, 255);
    else
      break;
  }

  //highlight selected object type
  con.Print(data.m_objectType[m_currentObjectID].name, F_V16B, m_panelX + 16, m_panelY + 65 + (m_currentObjectID * 16), 0, 0, 255);

  //draw on filebar
  con.Print(data.GetLevelFilename(m_editLevel),F_V14B, m_fileBarX + 299, m_fileBarY + 28, 255, 255, 0);

  //clear
  if(m_mouse.MouseX() > m_fileBarX + 16 && m_mouse.MouseX() < m_fileBarX + 103 &&
    m_mouse.MouseY() > m_fileBarY + 15 && m_mouse.MouseY() < m_fileBarY + 55){
    con.DrawRect((float)m_fileBarX + 16, (float)m_fileBarY + 15, (float)m_fileBarX + 103, (float)m_fileBarY + 55, 0, 255, 0);
  }

  //open level file 1
  if(m_mouse.MouseX() > m_fileBarX + 195 && m_mouse.MouseX() < m_fileBarX + 217 &&
    m_mouse.MouseY() > m_fileBarY + 15 && m_mouse.MouseY() < m_fileBarY + 55){
    con.DrawRect((float)m_fileBarX + 195, (float)m_fileBarY + 15, (float)m_fileBarX + 217, (float)m_fileBarY + 55, 0, 255, 0);
  }

  //open level file 2
  if(m_mouse.MouseX() > m_fileBarX + 223 && m_mouse.MouseX() < m_fileBarX + 245 &&
    m_mouse.MouseY() > m_fileBarY + 15 && m_mouse.MouseY() < m_fileBarY + 55){
    con.DrawRect((float)m_fileBarX + 223, (float)m_fileBarY + 15, (float)m_fileBarX + 245, (float)m_fileBarY + 55, 0, 255, 0);
  }

  //open level file 3
  if(m_mouse.MouseX() > m_fileBarX + 251 && m_mouse.MouseX() < m_fileBarX + 273 &&
    m_mouse.MouseY() > m_fileBarY + 15 && m_mouse.MouseY() < m_fileBarY + 55){
    con.DrawRect((float)m_fileBarX + 251, (float)m_fileBarY + 15, (float)m_fileBarX + 273, (float)m_fileBarY + 55, 0, 255, 0);
  }

  //save level file
  if(m_mouse.MouseX() > m_fileBarX + 434 && m_mouse.MouseX() < m_fileBarX + 514 &&
    m_mouse.MouseY() > m_fileBarY + 15 && m_mouse.MouseY() < m_fileBarY + 55){
    con.DrawRect((float)m_fileBarX + 434, (float)m_fileBarY + 15, (float)m_fileBarX + 514, (float)m_fileBarY + 55, 0, 255, 0);
  }

  //undo last game object
  if(m_mouse.MouseX() > m_fileBarX + 520 && m_mouse.MouseX() < m_fileBarX + 615 &&
    m_mouse.MouseY() > m_fileBarY + 15 && m_mouse.MouseY() < m_fileBarY + 55){
    con.DrawRect((float)m_fileBarX + 520, (float)m_fileBarY + 15, (float)m_fileBarX + 615, (float)m_fileBarY + 55, 0, 255, 0);
  }  
    

  //draw mouse cursor
  con.DrawLine(m_mouse.MouseX()+ 5, m_mouse.MouseY(), m_mouse.MouseX()+ 25 , m_mouse.MouseY(),0, 255, 255);
  con.DrawLine(m_mouse.MouseX()- 5, m_mouse.MouseY(), m_mouse.MouseX() - 25 , m_mouse.MouseY(),0, 255, 255);
  con.DrawLine(m_mouse.MouseX(), m_mouse.MouseY() + 5, m_mouse.MouseX(), m_mouse.MouseY() + 25,0, 255, 255);
  con.DrawLine(m_mouse.MouseX(), m_mouse.MouseY() - 5, m_mouse.MouseX(), m_mouse.MouseY() - 25,0, 255, 255);

  //editor label at top
  gi = con.GetSprite(45);
  con.RenderGraphicModulate(gi, m_editorX, m_editorY, 255, 255, 255);
  con.Print(data.m_playerX + (m_mouse.MouseX() - data.m_screenWidth/2), F_V20, m_editorX + 144, m_editorY + 68, 255, 255, 255);
  con.Print(data.m_playerY + (m_mouse.MouseY() - data.m_screenHeight/2), F_V20,  m_editorX + 391, m_editorY + 68, 255, 255, 255);
  con.Print(data.m_pObject.size(), F_V20, m_editorX + 440, m_editorY + 22, 0, 255, 255);

  //map
  gi = con.GetSprite(45);
  m_mapX = m_editorX + gi.width + 20;
  m_mapY = m_editorY + 30;
  con.DrawRect((float)m_mapX, (float)m_mapY, (float)(m_mapX + 80), (float)(m_mapY + 80), 255,0,0);//draw map
  int smallW = con.GetScreenWidth()/64;
  int smallH = con.GetScreenHeight()/85;
  
  con.DrawRect((float)m_mapX + data.m_playerX/32 - smallW, (float)m_mapY + data.m_playerY/32 - smallH, 
               (float)m_mapX + data.m_playerX/32 + smallW, (float)m_mapY + data.m_playerY/32 + smallH,255, 255, 255); //draw player

  //print text to display
  if(data.m_displayDebugInfo == true){
    con.Print("EDITOR STATE - Version", data.m_version, F_V16, 50, 10,  255, 255, 255, 255);
    con.Print("FPS", data.m_FPS , F_V16B, 530, 10,  255, 255, 0, 255);
  }
  
  //mouse coordinates
  con.Print("Press ESC to quit", F_V20, data.m_screenWidth/2 - 100, data.m_screenHeight/2 + 200, 255, 0, 0, 255);

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

void cGameStateEditor::processEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
}

void cGameStateEditor::deactivate(){
  CAudioManager *pAudio = CAudioManager::Instance();
  pAudio->KillAll();
//  Delete<psys::PSystem*>( Gun );
  
}
void cGameStateEditor::resume(){
}
void cGameStateEditor::pause(){
}
void cGameStateEditor::save(){
}

void cGameStateEditor::addTransitionEvent(int event, cGameStateObject* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}
