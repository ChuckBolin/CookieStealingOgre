#include "..\include\CGameData.h"

//game data constructor
CGameData::CGameData(){

  //screen and window information
  m_screenLeft = 0;
  m_screenTop = 0;
  m_screenWidth = 1024;
  m_screenHeight = 768;
  m_bFullScreen = 1;  //set to fullscreen by default

  //display while programming
  m_displayDebugInfo = true;

  //sound
  m_playMusic = true;

  //initialize level data
  m_currentLevel = 1;
  m_maxLevels = 1;
  for(int i = 0; i < 10; i++)
    m_levelFilename[i] = "";

  //initialize player data
  m_playerX = 400;
  m_playerY = 300;

  //initialize world limits
  m_world_left = 0;
  m_world_top = 0;
  m_world_right = 2559;
  m_world_bottom = 2559;

  //initialization of weapon inventory
  m_weapons[0].exists = false; m_weapons[0].maxRounds = 1000; m_weapons[0].rounds = 0;//bat
  m_weapons[1].exists = false; m_weapons[1].maxRounds = 15; m_weapons[1].rounds = 0;//pistol
  m_weapons[2].exists = false; m_weapons[2].maxRounds = 10; m_weapons[2].rounds = 0;//shotgun
  m_weapons[3].exists = false; m_weapons[3].maxRounds = 50; m_weapons[3].rounds = 0;//machine gun
  m_weapons[4].exists = false; m_weapons[4].maxRounds = 20; m_weapons[4].rounds = 0;//rockets
  m_weapons[5].exists = false; m_weapons[5].maxRounds = 20; m_weapons[5].rounds = 0;//flame thrower
  m_weapons[6].exists = false; m_weapons[6].maxRounds = 0; m_weapons[6].rounds = 0;
  m_weapons[7].exists = false; m_weapons[7].maxRounds = 0; m_weapons[7].rounds = 0;
  m_weaponType = 1;//pistol

  //offsets for flame - 16 frames
  //float m_flameTop[16];
  //float m_flameHeight[16];
  m_flameTop[0] = 86; m_flameHeight[0] = 34;
  m_flameTop[1] = 69; m_flameHeight[1] = 51;
  m_flameTop[2] = 42; m_flameHeight[2] = 78;
  m_flameTop[3] = 22; m_flameHeight[3] = 98;
  m_flameTop[4] = 0; m_flameHeight[4] = 120;
  m_flameTop[5] = 0; m_flameHeight[5] = 120;
  m_flameTop[6] = 0; m_flameHeight[6] = 120;
  m_flameTop[7] = 0; m_flameHeight[7] = 120;
  m_flameTop[8] = 0; m_flameHeight[8] = 120;
  m_flameTop[9] = 0; m_flameHeight[9] = 120;
  m_flameTop[10] = 0; m_flameHeight[10] = 100;
  m_flameTop[11] = 0; m_flameHeight[11] = 84;
  m_flameTop[12] = 0; m_flameHeight[12] = 60;
  m_flameTop[13] = 0; m_flameHeight[13] = 39;
  m_flameTop[14] = 0; m_flameHeight[14] = 24;
  m_flameTop[15] = 0; m_flameHeight[15] = 14;

}

bool CGameData::LoadConfigFile(std::string filename){
  CLog *pLog = CLog::Instance();

  //load config.txt file to configure game
  CINIReader cir;
  cir.LoadFile(filename);

  if(cir.IsValid() == false){
    pLog->Log("INI File invalid!");
    return false;
  }
  if(cir.GetNumberOfLines() < 1){
    pLog->Log("No parameters in INI file!");
    return false;
  }

  std::string value;
  std::string key;
  pLog->Log("******** Loading Config.txt **********", filename);
  
  for(int i = 0; i < cir.GetNumberOfLines(); ++i){

      //read key and value from file
      key = cir.GetTerm(cir.GetLineFromFile(i),1);
      value = cir.GetTerm(cir.GetLineFromFile(i),2);
     // pLog->Log("config.txt data", key, value);

      //screen and window information
      if(key == "screen_left")
        m_screenLeft = atoi(value.c_str());
      if(key == "screen_top")
        m_screenTop = atoi(value.c_str());
      if(key == "screen_width")
        m_screenWidth = atoi(value.c_str());
      if(key == "screen_height")
        m_screenHeight = atoi(value.c_str());

      if(key == "full_screen")
        if(value == "1")
          m_bFullScreen = true;
        else
          m_bFullScreen = false;

      //display debug information
      if(key == "display_debug_info"){
        if(value == "1")
          m_displayDebugInfo = true;
        else
          m_displayDebugInfo = false;
      }

      //play background music
      if(key == "play_music"){
        if(value == "1")
          m_playMusic = true;
        else
          m_playMusic = false;
      }

      //level information
      if(key == "current_level")
        m_currentLevel = atoi(value.c_str());
      if(key == "max_levels")
        m_maxLevels = atoi(value.c_str());
      if(key == "level_file1")
        m_levelFilename[0] = value;
      if(key == "level_file2")
        m_levelFilename[1] = value;
      if(key == "level_file3")
        m_levelFilename[2] = value;
      if(key == "level_file4")
        m_levelFilename[3] = value;
      if(key == "level_file5")
        m_levelFilename[4] = value;
      if(key == "level_file6")
        m_levelFilename[5] = value;
      if(key == "level_file7")
        m_levelFilename[6] = value;
      if(key == "level_file8")
        m_levelFilename[7] = value;
      if(key == "level_file9")
        m_levelFilename[8] = value;
      if(key == "level_file10")
        m_levelFilename[9] = value;

  }

  //LogKeyValue();

  return true;
}

void CGameData::LogKeyValue(){
  CLog *pLog = CLog::Instance();

  //pLog->Log("int m_screenWidth", m_screenWidth);
  //pLog->Log("int m_screenHeight", m_screenHeight);
  //pLog->Log("bool m_displayDebugInfo", m_displayDebugInfo);
 // pLog->Log("bool m_playMusic", m_playMusic);
}

void CGameData::LoadGameLevel(std::string filename, CGraphics &con){
  CLog *pLog = CLog::Instance();
  
  //read assets.dat file
  CFileReader cfr(filename);
  if(cfr.IsValid() == false)
    return; //file is invalid or contains no usable data

  //validate asset data and add to appropriate vectors
  GAME_OBJECT temp;
  GRAPHIC_IMAGE gi;
  GAME_OBJECT go;
  GAME_OBJECT_TYPE got;
  std::string value;
  std::string asset;
  m_level.clear();
  unsigned long size = 0;
  CGrandma tempMa;
  //CElf elf;
  //CLandscape land;
  //CCharacter character;
  srand((unsigned)time(NULL));

  for(int i = 0; i < cfr.GetNumberOfLines(); ++i){
    asset = cfr.GetTerm(cfr.GetLineFromFile(i),1);

    if(asset == "sprite")
      temp.asset = C_SPRITE;      //sprite
    else if(asset == "player")
      temp.asset = C_PLAYER;      //player
    else if(asset == "worldlimits")
      temp.asset = C_WORLD;       //world
    else if(asset == "tree1")
      temp.asset = C_TREE1; 
    else if(asset == "tree2")
      temp.asset = C_TREE2; 
    else if(asset == "tree3")
      temp.asset = C_TREE3; 
    
    //mesh or splatter
    //if (temp.asset == C_MESH || temp.asset == C_SPLATTER){
    if(asset == "mesh" || asset == "splatter"){
      if(asset == "mesh")
        temp.asset = C_MESH;        //mesh
      else if(asset == "splatter")
        temp.asset = C_SPLATTER;    //splatter      

      value = cfr.GetTerm(cfr.GetLineFromFile(i),2);
      temp.meshID = atoi(value.c_str());
      value = cfr.GetTerm(cfr.GetLineFromFile(i),3);
      temp.meshBadID = atoi(value.c_str());
      value = cfr.GetTerm(cfr.GetLineFromFile(i),4);
      temp.x = atof(value.c_str());
      value = cfr.GetTerm(cfr.GetLineFromFile(i),5);
      temp.y = atof(value.c_str());
      value = cfr.GetTerm(cfr.GetLineFromFile(i),6);
      temp.z = atof(value.c_str());
      value = cfr.GetTerm(cfr.GetLineFromFile(i),7);
      temp.rx = atof(value.c_str());
      value = cfr.GetTerm(cfr.GetLineFromFile(i),8);
      temp.ry = atof(value.c_str());
      value = cfr.GetTerm(cfr.GetLineFromFile(i),9);
      temp.rz = atof(value.c_str());
      temp.alive = true;
      m_level.push_back(temp);
    }

    //game time
    else if(asset == "time"){
      value = cfr.GetTerm(cfr.GetLineFromFile(i),2);
      m_player.m_time = atoi(value.c_str());
    }

    else if(asset == "player"){// temp.asset == C_PLAYER){
      value = cfr.GetTerm(cfr.GetLineFromFile(i),2);
      if(value == "position"){
        value = cfr.GetTerm(cfr.GetLineFromFile(i),3);
        m_playerX = atoi(value.c_str());
        m_player.go.pos.x = atoi(value.c_str());
        value = cfr.GetTerm(cfr.GetLineFromFile(i),4);
        m_playerY = atoi(value.c_str());
        m_player.go.pos.y = atoi(value.c_str());
      }
    }

    else if(asset == "grandma"){
      //m_grandma.push_back(new CGrandma);

      //look for match between level object name and game object types
      go.spriteID = -1;
      for(int j = 0; j < m_objectType.size(); ++j){
        //pLog->Log("Object", m_objectType[j].name, m_objectType[j].spriteID);

        if(m_objectType[j].name == "grandma"){

          //load game object data from file
          value = cfr.GetTerm(cfr.GetLineFromFile(i),2);
          go.pos.x = atoi(value.c_str());
          value = cfr.GetTerm(cfr.GetLineFromFile(i),3);
          go.pos.y = atoi(value.c_str());
          value = cfr.GetTerm(cfr.GetLineFromFile(i),4);
          go.mode = atoi(value.c_str());

          //load game object data from assets and sprite data
          go.spriteID = m_objectType[j].spriteID;
          go.spriteBadID = m_objectType[j].badSpriteID;
          go.objectID = m_objectType[j].objectID;
          //if(go.objectID > 15){
          //  pLog->Log("Loaded", go.objectID, go.pos.x, go.pos.y);
          //}
          go.radius = m_objectType[j].radius;
          go.layer = m_objectType[j].layer;
          gi = con.GetSprite(go.spriteID);
          go.totalTime = (double)( rand() % 1000)/1000;
          go.width = gi.width;
          go.height = gi.height;
          go.maxFrames = gi.maxFrames; //1=static, >1 is an animation
          go.updateInterval = gi.updateInterval; //how often to change frames
          go.alive = true;
          go.dying = false;
          go.angle = 0;
          if(go.maxFrames > 1)
            go.frameCount = rand() % go.maxFrames;
          else
            go.frameCount = 0;  //sets initial animation frame
          tempMa.go = go;
          m_grandma.push_back(tempMa);// [m_grandma.size() - 1].go = go;

          //pLog->Log("object added", m_pObject[m_pObject.size() - 1]->go.objectID);
          break;
        }
      }
    }
/*
    else if(asset == "ogre"){
      //m_ogregrandma.push_back(new CGrandma);

      //look for match between level object name and game object types
      go.spriteID = -1;
      for(int j = 0; j < m_objectType.size(); ++j){
        //pLog->Log("Object", m_objectType[j].name, m_objectType[j].spriteID);

        if(m_objectType[j].name == "ogre"){

          //load game object data from file
          value = cfr.GetTerm(cfr.GetLineFromFile(i),2);
          go.pos.x = atoi(value.c_str());
          value = cfr.GetTerm(cfr.GetLineFromFile(i),3);
          go.pos.y = atoi(value.c_str());
          //value = cfr.GetTerm(cfr.GetLineFromFile(i),4);
          //go.mode = atoi(value.c_str());

          //load game object data from assets and sprite data
          go.spriteID = m_objectType[j].spriteID;
          go.spriteBadID = m_objectType[j].badSpriteID;
          go.objectID = m_objectType[j].objectID;
          //if(go.objectID > 15){
          //  pLog->Log("Loaded", go.objectID, go.pos.x, go.pos.y);
          //}
          go.radius = m_objectType[j].radius;
          go.layer = m_objectType[j].layer;
          gi = con.GetSprite(go.spriteID);
          go.totalTime = (double)( rand() % 1000)/1000;
          go.width = gi.width;
          go.height = gi.height;
          go.maxFrames = gi.maxFrames; //1=static, >1 is an animation
          go.updateInterval = gi.updateInterval; //how often to change frames
          go.alive = true;
          go.dying = false;
          go.angle = 0;
          if(go.maxFrames > 1)
            go.frameCount = rand() % go.maxFrames;
          else
            go.frameCount = 0;  //sets initial animation frame
          //m_ogre.go = go;
          //m_grandma[m_grandma.size() - 1].go = go;

          //pLog->Log("object added", m_pObject[m_pObject.size() - 1]->go.objectID);
          break;
        }
      }
    }
*/
    else if(asset == "elf"){
      m_pObject.push_back(new CElf);

      //look for match between level object name and game object types
      go.spriteID = -1;
      for(int j = 0; j < m_objectType.size(); ++j){
        //pLog->Log("Object", m_objectType[j].name, m_objectType[j].spriteID);

        if(m_objectType[j].name == "elf"){

          //load game object data from file
          value = cfr.GetTerm(cfr.GetLineFromFile(i),2);
          go.pos.x = atoi(value.c_str());
          value = cfr.GetTerm(cfr.GetLineFromFile(i),3);
          go.pos.y = atoi(value.c_str());
          value = cfr.GetTerm(cfr.GetLineFromFile(i),4);
          go.speed = atof(value.c_str());

          //load game object data from assets and sprite data
          go.spriteID = m_objectType[j].spriteID;
          go.spriteBadID = m_objectType[j].badSpriteID;
          go.objectID = m_objectType[j].objectID;
          //if(go.objectID > 15){
          //  pLog->Log("Loaded", go.objectID, go.pos.x, go.pos.y);
          //}
          go.radius = m_objectType[j].radius;
          go.layer = m_objectType[j].layer;
          gi = con.GetSprite(go.spriteID);
          go.totalTime = (double)( rand() % 1000)/1000;
          go.width = gi.width;
          go.height = gi.height;
          go.maxFrames = gi.maxFrames; //1=static, >1 is an animation
          go.updateInterval = gi.updateInterval; //how often to change frames
          go.alive = true;
          go.dying = false;
          go.angle = 0;
          if(go.maxFrames > 1)
            go.frameCount = rand() % go.maxFrames;
          else
            go.frameCount = 0;  //sets initial animation frame
          m_pObject[m_pObject.size() - 1]->go = go;

          //pLog->Log("object added", m_pObject[m_pObject.size() - 1]->go.objectID);
          break;
        }
      }
    }

    else if(asset == "fairy"){
      m_pObject.push_back(new CFairy);

      //look for match between level object name and game object types
      go.spriteID = -1;
      for(int j = 0; j < m_objectType.size(); ++j){
        //pLog->Log("Object", m_objectType[j].name, m_objectType[j].spriteID);

        if(m_objectType[j].name == "fairy"){

          //load game object data from file
          value = cfr.GetTerm(cfr.GetLineFromFile(i),2);
          go.pos.x = atoi(value.c_str());
          value = cfr.GetTerm(cfr.GetLineFromFile(i),3);
          go.pos.y = atoi(value.c_str());
          value = cfr.GetTerm(cfr.GetLineFromFile(i),4);
          go.speed = atof(value.c_str());

          //load game object data from assets and sprite data
          go.spriteID = m_objectType[j].spriteID;
          go.spriteBadID = m_objectType[j].badSpriteID;
          go.objectID = m_objectType[j].objectID;
          //if(go.objectID > 15){
          //  pLog->Log("Loaded", go.objectID, go.pos.x, go.pos.y);
          //}
          go.radius = m_objectType[j].radius;
          go.layer = m_objectType[j].layer;
          gi = con.GetSprite(go.spriteID);
          go.totalTime = (double)( rand() % 1000)/1000;
          go.width = gi.width;
          go.height = gi.height;
          go.maxFrames = gi.maxFrames; //1=static, >1 is an animation
          go.updateInterval = gi.updateInterval; //how often to change frames
          go.alive = true;
          go.angle = 0;
          if(go.maxFrames > 1)
            go.frameCount = rand() % go.maxFrames;
          else
            go.frameCount = 0;  //sets initial animation frame
          m_pObject[m_pObject.size() - 1]->go = go;

          //pLog->Log("object added", m_pObject[m_pObject.size() - 1]->go.objectID);
          break;
        }
      }
    }

    else if(asset == "witch"){
      m_pObject.push_back(new CWitch);

      //look for match between level object name and game object types
      go.spriteID = -1;
      for(int j = 0; j < m_objectType.size(); ++j){
        //pLog->Log("Object", m_objectType[j].name, m_objectType[j].spriteID);

        if(m_objectType[j].name == "witch"){

          //load game object data from file
          value = cfr.GetTerm(cfr.GetLineFromFile(i),2);
          go.pos.x = atoi(value.c_str());
          value = cfr.GetTerm(cfr.GetLineFromFile(i),3);
          go.pos.y = atoi(value.c_str());
          value = cfr.GetTerm(cfr.GetLineFromFile(i),4);
          go.speed = atof(value.c_str());

          //load game object data from assets and sprite data
          go.spriteID = m_objectType[j].spriteID;
          go.spriteBadID = m_objectType[j].badSpriteID;
          go.objectID = m_objectType[j].objectID;
          //if(go.objectID > 15){
          //  pLog->Log("Loaded", go.objectID, go.pos.x, go.pos.y);
          //}
          go.radius = m_objectType[j].radius;
          go.layer = m_objectType[j].layer;
          gi = con.GetSprite(go.spriteID);
          go.totalTime = (double)( rand() % 1000)/1000;
          go.width = gi.width;
          go.height = gi.height;
          go.maxFrames = gi.maxFrames; //1=static, >1 is an animation
          go.updateInterval = gi.updateInterval; //how often to change frames
          go.alive = true;
          go.angle = 0;
          if(go.maxFrames > 1)
            go.frameCount = rand() % go.maxFrames;
          else
            go.frameCount = 0;  //sets initial animation frame

          m_pObject[m_pObject.size() - 1]->go = go;

          //pLog->Log("object added", m_pObject[m_pObject.size() - 1]->go.objectID);
          break;
        }
      }
    }

    else if(asset == "worldlimits"){ //temp.asset == C_WORLD){
      value = cfr.GetTerm(cfr.GetLineFromFile(i),2);
      m_world_left = atof(value.c_str());
      value = cfr.GetTerm(cfr.GetLineFromFile(i),3);
      m_world_top = atof(value.c_str());
      value = cfr.GetTerm(cfr.GetLineFromFile(i),4);
      m_world_right = atof(value.c_str());
      value = cfr.GetTerm(cfr.GetLineFromFile(i),5);
      m_world_bottom = atof(value.c_str());
    }

    //generic objects
    else if(cfr.GetNumberOfTermsFromLine(i) == 3){
      m_pObject.push_back(new CGeneric);
      go.pos.x = 0;
      go.pos.y = 0;
      go.pos.z = 0;

      //read name of game object
      value = cfr.GetTerm(cfr.GetLineFromFile(i),1);

      //look for match between level object name and game object types
      go.spriteID = -1;
      for(int j = 0; j < m_objectType.size(); ++j){
        if(m_objectType[j].name == value){
          
          //load game object data from file
          value = cfr.GetTerm(cfr.GetLineFromFile(i),2);
          go.pos.x = atoi(value.c_str());
          value = cfr.GetTerm(cfr.GetLineFromFile(i),3);
          go.pos.y = atoi(value.c_str());

          //load game object data from assets and sprite data
          go.spriteID = m_objectType[j].spriteID;
          go.spriteBadID = m_objectType[j].badSpriteID;
          go.objectID = m_objectType[j].objectID;
          go.radius = m_objectType[j].radius;
          go.layer = m_objectType[j].layer;
          gi = con.GetSprite(go.spriteID);
          go.totalTime = (double)( rand() % 1000)/1000;
          go.width = gi.width;
          go.height = gi.height;
          go.maxFrames = gi.maxFrames; //1=static, >1 is an animation
          go.updateInterval = gi.updateInterval; //how often to change frames
          go.alive = true;
          go.dying = false;
          go.angle = 0;
          if(go.maxFrames > 1)
            go.frameCount = rand() % go.maxFrames;
          else
            go.frameCount = 0;  //sets initial animation frame

          m_pObject[m_pObject.size() - 1]->go = go;
          //pLog->Log("object added", m_pObject[m_pObject.size() - 1]->go.objectID);
          break;
        }
      }
    }
  }
}

int CGameData::LevelSize(){
    return m_level.size();
}

GAME_OBJECT CGameData::GetLevelData(int i){
  return m_level[i];
}

void CGameData::UpdateLevelData(int i, GAME_OBJECT gob){
  if(i < 0 || i > m_level.size() - 1)
    return;
  m_level[i] = gob;
}

std::string CGameData::GetLevelFilename(int level){
  if(level < 1 || level > 10)
    return "";

  return m_levelFilename[level - 1];
}

void CGameData::AddGameObject(GRAPHIC_IMAGE gi, int x, int y){
  CLog *pLog = CLog::Instance();

  int cx = m_screenWidth/2;  //center of screen
  int cy = m_screenHeight/2;

  int wx = m_playerX + x - cx;
  int wy = m_playerY + y - cy;

  //validate asset data and add to appropriate vectors
  GAME_OBJECT temp;
  temp.asset = C_SPRITE;
  temp.maxFrames = gi.maxFrames;
  temp.updateInterval = gi.updateInterval;
  temp.spriteID = gi.spriteID;
  temp.left = wx;
  temp.top = wy;
  temp.spriteBadID = -1;
  temp.totalTime = 0.0;
  temp.frameCount = 0;      
  temp.alive = true;
  m_level.push_back(temp);
 // pLog->Log("AddSprite", wx, wy, LevelSize());//  m_level.size());
}

void CGameData::AddGameObject(int spriteID, int x, int y){
  CLog *pLog = CLog::Instance();

  int cx = m_screenWidth/2;  //center of screen
  int cy = m_screenHeight/2;

  int wx = m_playerX + x - cx;
  int wy = m_playerY + y - cy;

  //validate asset data and add to appropriate vectors
  GAME_OBJECT temp;
  temp.asset = C_SPRITE;
  temp.maxFrames = 1;//gi.frameMax;
  temp.updateInterval = 0;//gi.updateInterval;
  temp.spriteID = spriteID;
  temp.left = wx;
  temp.top = wy;
  temp.spriteBadID = -1;
  temp.totalTime = 0.0;
  temp.frameCount = 0;      
  temp.alive = true;
  m_level.push_back(temp);
 // pLog->Log("AddSprite", wx, wy, LevelSize());//  m_level.size());
}

void CGameData::AddGameObject(GAME_OBJECT go){//, int x, int y){
  CLog *pLog = CLog::Instance();
    
  m_pObject.push_back(new CGeneric);
  m_pObject[m_pObject.size() - 1]->go = go;
  //pLog->Log("Angle", go.angle);
  //pLog->Log("AddGameObject(go)", go.objectID,m_pObject[m_pObject.size() - 1]->go.pos.x, m_pObject[m_pObject.size() - 1]->go.pos.y);
}

GAME_OBJECT CGameData::GetObjectInfo(int objectID){
  GAME_OBJECT go;
  for(int i = 0; i< m_objectType.size(); ++i){
    if(m_objectType[i].objectID == objectID){
      go.spriteID = m_objectType[i].spriteID;
      go.objectID = m_objectType[i].objectID;
      go.spriteBadID = m_objectType[i].badSpriteID;
      go.layer = m_objectType[i].layer;
      go.radius = m_objectType[i].radius;
      return go;
    }
  }

  go.spriteID = -1;
  return go;
}

int CGameData::GameObjectTypeSize(){
  return m_pObject.size();
}


void CGameData::LoadGameLevelBU(std::string filename){
  CLog *pLog = CLog::Instance();
  
  //read assets.dat file
  CFileReader cfr(filename);
  if(cfr.IsValid() == false)
    return; //file is invalid or contains no usable data

  //validate asset data and add to appropriate vectors
  GAME_OBJECT temp;
  std::string value;
  std::string asset;
  m_level.clear();

  for(int i = 0; i < cfr.GetNumberOfLines(); ++i){
    asset = cfr.GetTerm(cfr.GetLineFromFile(i),1);
    if(asset == "mesh")
      temp.asset = C_MESH;        //mesh
    else if(asset == "splatter")
      temp.asset = C_SPLATTER;    //splatter
    else if(asset == "sprite")
      temp.asset = C_SPRITE;      //sprite
    else if(asset == "player")
      temp.asset = C_PLAYER;      //player
    else if(asset == "worldlimits")
      temp.asset = C_WORLD;       //world
    
    //mesh or splatter
    if (temp.asset == C_MESH || temp.asset == C_SPLATTER){
      value = cfr.GetTerm(cfr.GetLineFromFile(i),2);
      temp.meshID = atoi(value.c_str());
      value = cfr.GetTerm(cfr.GetLineFromFile(i),3);
      temp.meshBadID = atoi(value.c_str());
      value = cfr.GetTerm(cfr.GetLineFromFile(i),4);
      temp.x = atof(value.c_str());
      value = cfr.GetTerm(cfr.GetLineFromFile(i),5);
      temp.y = atof(value.c_str());
      value = cfr.GetTerm(cfr.GetLineFromFile(i),6);
      temp.z = atof(value.c_str());
      value = cfr.GetTerm(cfr.GetLineFromFile(i),7);
      temp.rx = atof(value.c_str());
      value = cfr.GetTerm(cfr.GetLineFromFile(i),8);
      temp.ry = atof(value.c_str());
      value = cfr.GetTerm(cfr.GetLineFromFile(i),9);
      temp.rz = atof(value.c_str());
      temp.alive = true;
      m_level.push_back(temp);
    }

    else if(temp.asset == C_SPRITE){
      value = cfr.GetTerm(cfr.GetLineFromFile(i),2);
      temp.spriteID = atoi(value.c_str());
      value = cfr.GetTerm(cfr.GetLineFromFile(i),3);
      temp.spriteBadID = atoi(value.c_str());
      value = cfr.GetTerm(cfr.GetLineFromFile(i),4);
      temp.left = atoi(value.c_str());
      value = cfr.GetTerm(cfr.GetLineFromFile(i),5);
      temp.top = atoi(value.c_str());
      temp.totalTime = 0.0;
      temp.frameCount = 0;      
      temp.alive = true;
      m_level.push_back(temp);
    }

    else if(temp.asset == C_PLAYER){
      //value = cfr.GetTerm(cfr.GetLineFromFile(i),2);
      //temp.spriteID = atoi(value.c_str());
      value = cfr.GetTerm(cfr.GetLineFromFile(i),3);
      m_playerX = atoi(value.c_str());
      value = cfr.GetTerm(cfr.GetLineFromFile(i),4);
      m_playerY = atoi(value.c_str());
      //temp.alive = true;
      //m_level.push_back(temp);
    }

    else if(temp.asset == C_WORLD){
      //pLog->Log("World found");
      value = cfr.GetTerm(cfr.GetLineFromFile(i),2);
      m_world_left = atof(value.c_str());
      value = cfr.GetTerm(cfr.GetLineFromFile(i),3);
      m_world_top = atof(value.c_str());
      value = cfr.GetTerm(cfr.GetLineFromFile(i),4);
      m_world_right = atof(value.c_str());
      value = cfr.GetTerm(cfr.GetLineFromFile(i),5);
      m_world_bottom = atof(value.c_str());

    }

  }
//pLog->Log("world top", m_world_top);
}

//m_objectType
bool CGameData::LoadAssetFile(std::string filename){
  CLog *pLog = CLog::Instance();
  
  //read assets.dat file
  CFileReader cfr(filename);
  if(cfr.IsValid() == false)
    return false; //file is invalid or contains no usable data

  //validate asset data and add to appropriate vectors
  std::string parameter;
  std::string assetType;
  GAME_OBJECT_TYPE got;

  for(int i = 0; i < cfr.GetNumberOfLines(); ++i){

    if(cfr.GetNumberOfTermsFromLine(i) > 1){
      assetType = cfr.GetTerm(cfr.GetLineFromFile(i), 1);
      
      //ignore these asset types
      if(assetType == "graphicfile" || assetType == "sprite" || assetType == "mesh" || assetType == "soundfile"){
        //do nothing..ignore these assets
      }
    
      //this should be a game object type
      else{
        if(cfr.GetNumberOfTermsFromLine(i) == 6){ // need 6 parameters to be valid
          got.name = cfr.GetTerm(cfr.GetLineFromFile(i), 1);
          parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 2);
          got.objectID = atoi(parameter.c_str());
          parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 3);
          got.spriteID = atoi(parameter.c_str());
          parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 4);
          got.badSpriteID = atoi(parameter.c_str());
          parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 5);
          got.layer = atoi(parameter.c_str());
          parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 6);
          got.radius = atof(parameter.c_str());
          //pLog->Log("Object added",got.name, got.objectID);
          m_objectType.push_back(got);
        }        
      }
    }
  }
  
  //log results of load
//  pLog->Log("Number of game object types",  m_objectType.size());

  return true;
}

//saves data
void CGameData::SaveGameLevel(std::string filename){
  std::ofstream file;
  std::ostringstream ofile;
  ofile << "data//" << filename.c_str();
  std::string sFile;
  sFile = ofile.str();
  file.open(sFile.c_str(), std::ios_base::out);
  //file.open( filename.c_str(), std::ios_base::app);
  //GAME_OBJECT go;
  std::string name;

  if(file){
    std::ostringstream oss;

    //add header
    oss << "//Cindy and the Cookie Stealing Ogre" << std::endl;
    oss << "//" << filename << " Auto generated by the in-game Editor" << std::endl;
    oss << "player, position, 280, 280" << std::endl;
    oss << "time, 300" << std::endl;

    for(int i = 0; i < m_pObject.size(); ++i){
      name = GetObjectTypeName(m_pObject[i]->go.objectID);
      if(name == "elf" || name == "fairy" || name == "witch"){
        oss << name << ", " << m_pObject[i]->go.pos.x << ", " << m_pObject[i]->go.pos.y << ", " << m_pObject[i]->go.speed << std::endl;
      }
      else if(name == "grandma"){
        oss << name << ", " << m_pObject[i]->go.pos.x << ", " << m_pObject[i]->go.pos.y << ", 1" << std::endl;
      }
      else if(name == "ogre"){
        oss << name << ", " << m_pObject[i]->go.pos.x << ", " << m_pObject[i]->go.pos.y << ", 1" << std::endl;
      }

      else if(name.size() > 0){
        oss << name << ", " << m_pObject[i]->go.pos.x << ", " << m_pObject[i]->go.pos.y << std::endl;
      }
    }

    oss << std::endl;
    file << oss.str();    
  }

  file.close();  


}

std::string CGameData::GetObjectTypeName(int objectID){
  for(int i = 0; i < m_objectType.size(); ++i){
    if(m_objectType[i].objectID == objectID)
      return m_objectType[i].name;
  }
  return "";
}


/**************************************************************
  Used to determine if player collides with a game object. The
  only game objects of concern are those on layer 6.  The next
  factor is the radius. 
  radius = 0 no risk of collision, ignore
  radius = -1, treat player and object as rectangles, look
           for overlap
  radius > 0, treat object and player as points with radii

**************************************************************/
bool CGameData::IsPlayerCollision(int index){
  CLog *pLog = CLog::Instance();
  GAME_OBJECT lastGo =  m_player.go;
  if(index < 0 || index > m_pObject.size() - 1)
    return false;

  if(m_pObject[index]->go.radius == 0)
    return false;

  float radius = 0;
  float distance = 0;
  float ax,ay,aw,ah,bx,by,bw,bh;
  float cx,cy,cw,ch;            
  float sl, sr, st, sb, sw, sh; //defines new rect
  float dx, dy;
  float ox, oy;
  float mx, my;

  //rectangular collision - radius == -1
  if(m_pObject[index]->go.radius == -1 && m_pObject[index]->go.layer != 7){
    
    //step 1 of algorithm
    ox = m_pObject[index]->go.pos.x - m_player.go.pos.x;
    oy = m_pObject[index]->go.pos.y - m_player.go.pos.y;

    //step #2 of algorithm
    if(abs(ox) < 32 && abs(oy) < 32){
      
      //step #3 of algorithm
      mx = 0;
      my = 0;
      if(oy >= 0)
        my = oy - 32;
      if(oy < 0)
        my = oy + 32;
      if(ox >= 0)
        mx = ox -32;
      if(ox < 0)
        mx = ox + 32;

      //step #4 of algorithm
      m_player.go.pos.x += mx/2;
      m_player.go.pos.y += my/2;

      return true;
  
    }

    /*
    dx = abs(m_player.go.pos.x - m_pObject[index]->go.pos.x);
    dy = abs(m_player.go.pos.y - m_pObject[index]->go.pos.y);
    cx = m_pObject[index]->go.width/2 + 16;
    cy = m_pObject[index]->go.height/2 + 16;

    if(dx < (cx - 4) && dy < (cy - 4)){  //6
      if(m_player.go.pos.x <= m_pObject[index]->go.pos.x)      
        m_player.go.pos.x -= 8;
      else
        m_player.go.pos.x += 8;

      if(m_player.go.pos.y <= m_pObject[index]->go.pos.y)      
        m_player.go.pos.y -= 8;
      else
        m_player.go.pos.y += 8;

      return true;   
    }
    */
  }

  //distance - radius > 0
  else if(m_pObject[index]->go.layer != 7){
  
    radius =  m_pObject[index]->go.radius + 16; //replace with player radius
    distance = sqrt( ((m_player.go.pos.x - m_pObject[index]->go.pos.x) *
                      (m_player.go.pos.x - m_pObject[index]->go.pos.x)) +
                     ((m_player.go.pos.y - m_pObject[index]->go.pos.y) *
                      (m_player.go.pos.y - m_pObject[index]->go.pos.y)) );

    if(distance < radius){
      return true;
    }
  }

  //these are power ups
  else if(m_pObject[index]->go.radius == -1 && m_pObject[index]->go.layer == 7){
    radius =  m_pObject[index]->go.radius + 32; //replace with player radius
    distance = sqrt( ((m_player.go.pos.x - m_pObject[index]->go.pos.x) *
                      (m_player.go.pos.x - m_pObject[index]->go.pos.x)) +
                     ((m_player.go.pos.y - m_pObject[index]->go.pos.y) *
                      (m_player.go.pos.y - m_pObject[index]->go.pos.y)) );

    if(distance < radius){
      pLog->Log("Delete weapon find!");
      return true;
    }
  }
  return false;
}

void CGameData::AddProjectile(float x, float y, float vx, float vy, int type, float dir){
  WEAPON_OBJECT temp;
  temp.pos.x = x;
  temp.pos.y = y;
  temp.vel.x = vx;
  temp.vel.y = vy;
  temp.weaponType = type;
  temp.alive = true;
  temp.distance = 0;
  temp.dir = dir;
  GAME_OBJECT go;
  go.pos.x = x;
  go.pos.y = y;
  go.frameCount = 0;
  go.angle = dir;
  go.alive = true;

  CAudioManager *pAudio = CAudioManager::Instance();
  if(type == WEAPON_PISTOL)// && pAudio->IsPlaying(5) == false)
    pAudio->PlaySoundClip(5, 0);
  if(type == WEAPON_SHOTGUN)// && pAudio->IsPlaying(4) == false)
    pAudio->PlaySoundClip(4, 0);
  if(type == WEAPON_MACHINEGUN && pAudio->IsPlaying(2) == false)
    pAudio->PlaySoundClip(2, 0);
  if(type == WEAPON_ROCKET)// && pAudio->IsPlaying(6) == false)
    pAudio->PlaySoundClip(6, 0);
  if(type == WEAPON_FLAMETHROWER)// && pAudio->IsPlaying(3) == false)
    pAudio->PlaySoundClip(3, 0);


  switch(type){
    case 0:  //bat      
      temp.damage = 1;
      temp.radius = 1;
      temp.range = 10;
      temp.spriteID = 87;   
      go.maxFrames = 8;
      go.updateInterval =0.2;
      go.totalTime = 0;      
      go.height = 32;
      go.width = 32;
      break;
    case 1: //pistol
      temp.damage = 1;
      temp.radius = 1;
      temp.range = 10;
      temp.spriteID = 84;      
      go.maxFrames = 12;
      go.updateInterval =0.1;
      go.totalTime = 0;      
      go.height = 32;
      go.width = 32;
      break;
    case 2: //shotgun
      temp.damage = 1;
      temp.radius = 1;
      temp.range = 10;
      temp.spriteID = 85;  
      go.maxFrames = 12;
      go.updateInterval =0.1;
      go.totalTime = 0;      
      go.height = 32;
      go.width = 32;
      break;
    case 3: //machinegun
      temp.damage = 1;
      temp.radius = 1;
      temp.range = 10;
      temp.spriteID = 86;  
      go.maxFrames = 4;
      go.updateInterval =0.1;
      go.totalTime = 0;      
      go.height = 32;
      go.width = 32;
      break;
    case 4: //rocket
      temp.damage = 1;
      temp.radius = 1;
      temp.range = 10;
      temp.spriteID = 88;  
      go.maxFrames = 12;
      go.updateInterval =0.1;
      go.totalTime = 0;      
      go.height = 32;
      go.width = 32;
      break;
    case 5: //flamethrower
      temp.damage = 1;
      temp.radius = 1;
      temp.range = 10;
      temp.spriteID = 89;      
      go.maxFrames = 16;
      go.updateInterval =0.1;
      go.totalTime = 0;    
      go.height = 124;
      go.width = 32;
      break;
  }
  CProjectile cp;
  cp.m_WO = temp;
  go.spriteID = temp.spriteID;
  cp.go = go;  
  m_projectiles.push_back(cp);
}

void CGameData::UpdateProjectiles(double timeDifference){
  int offset = 0;
  bool bRocket = false;

  //let's process each projectile
  for(int i = 0; i < m_projectiles.size(); ++i){

    if(m_projectiles[i].m_WO.spriteID == 84)//pistol
      offset = 5;
    else if(m_projectiles[i].m_WO.spriteID == 85)//shotgun
      offset = 15;
    else if(m_projectiles[i].m_WO.spriteID == 86)//machine gun
      offset = 10;
    else if(m_projectiles[i].m_WO.spriteID == 88)//rocket
      offset = 32;

    //iterate through all objects
    for(int j = 0; j < m_pObject.size(); ++j){
      
      //let's consider only bad guys with pistol, machine gun, shotgun,  and rocket
      if(m_pObject[j]->go.objectID != 30 &&  m_pObject[j]->go.dying == false && m_projectiles[i].m_WO.spriteID < 89 && m_pObject[j]->go.layer == 8){ //(m_pObject[j]->go.objectID == 27 || m_pObject[j]->go.objectID == 28 || m_pObject[j]->go.objectID == 29)){
  
        //let's narrow it down to a offset x offset box around the projectiles
        if((m_pObject[j]->go.pos.x > m_projectiles[i].go.pos.x - offset) && (m_pObject[j]->go.pos.x < m_projectiles[i].go.pos.x + offset) &&
          (m_pObject[j]->go.pos.y > m_projectiles[i].go.pos.y - offset) && (m_pObject[j]->go.pos.y < m_projectiles[i].go.pos.y + offset)){
              m_pObject[j]->go.dying = true;
              m_pObject[j]->go.frameCount = 0;
              m_pObject[j]->go.spriteID = m_pObject[j]->go.spriteBadID;
              m_projectiles[i].go.alive = false;

              if(m_pObject[j]->go.objectID == 27){ //elf
                m_player.m_score += m_currentLevel * 10;
                m_player.m_kindness -= 8;
              }
              else if(m_pObject[j]->go.objectID == 28){ //fairy
                m_player.m_score += m_currentLevel * 20;
                m_player.m_kindness -= 8;
              }
              else if(m_pObject[j]->go.objectID == 29){ //witch
                m_player.m_score += m_currentLevel * 30;
                m_player.m_kindness -= 8;
              }
        }
      }

      //rocket attack - elf, fairy, witch //m_pObject[j]->go.alive == true &&
      else if( m_pObject[j]->go.dying == false && m_projectiles[i].m_WO.spriteID == 88 && (m_pObject[j]->go.layer == 6 || (m_pObject[j]->go.layer == 7 && m_pObject[j]->go.radius > 0))){//block or tree

        if((m_pObject[j]->go.pos.x > m_projectiles[i].go.pos.x - offset) && (m_pObject[j]->go.pos.x < m_projectiles[i].go.pos.x + offset) &&
          (m_pObject[j]->go.pos.y > m_projectiles[i].go.pos.y - offset) && (m_pObject[j]->go.pos.y < m_projectiles[i].go.pos.y + offset)){

              m_pObject[j]->go.dying = true;
              m_pObject[j]->go.frameCount = 0;
              m_pObject[j]->go.maxFrames = 8;
              m_pObject[j]->go.spriteID = m_pObject[j]->go.spriteBadID;
              if(m_pObject[j]->go.layer == 7 && m_pObject[j]->go.radius > 0)//trees
                m_pObject[j]->go.updateInterval = .5;
              else
                m_pObject[j]->go.updateInterval = .2;

              m_pObject[j]->go.spriteID = m_pObject[j]->go.spriteBadID;
              //m_projectiles[i].go.alive = false;
             bRocket = true;

              if(m_pObject[j]->go.objectID == 27){ //elf
                m_player.m_score += m_currentLevel * 10;
                m_player.m_kindness -= 8;
              }
              else if(m_pObject[j]->go.objectID == 28){ //fairy
                m_player.m_score += m_currentLevel * 20;
                m_player.m_kindness -= 8;
              }
              else if(m_pObject[j]->go.objectID == 29){ //witch
                m_player.m_score += m_currentLevel * 30;
                m_player.m_kindness -= 8;
              }
              else if((m_pObject[j]->go.objectID >= 4 && m_pObject[j]->go.objectID <= 8) ||  m_pObject[j]->go.objectID == 11)//blocks
                m_player.m_score += m_currentLevel * 5;
              else if(m_pObject[j]->go.objectID >= 0 && m_pObject[j]->go.objectID <= 2)//trees
                m_player.m_score += m_currentLevel * 5;

            //}
        }
      }

      //flamethrower - kill elf, fairy, witch, trees
      //else if(m_pObject[j]->go.alive == true && (m_pObject[j]->go.objectID == 27 || m_pObject[j]->go.objectID == 28 || m_pObject[j]->go.objectID == 29)){// || (m_pObject[j]->go.layer == 7 && m_pObject[j]->go.radius > 0) )){ //
      else if(m_pObject[j]->go.dying == false && m_projectiles[i].m_WO.spriteID == 89 && (m_pObject[j]->go.objectID == 27 || m_pObject[j]->go.objectID == 28 || m_pObject[j]->go.objectID == 29 || (m_pObject[j]->go.layer == 7 && m_pObject[j]->go.radius > 0) )){ //

        //flame up    
        if(m_projectiles[i].go.angle < 0.1){
          if((m_pObject[j]->go.pos.x > m_projectiles[i].go.pos.x - 15) && (m_pObject[j]->go.pos.x < m_projectiles[i].go.pos.x + 15) &&
             (m_pObject[j]->go.pos.y > m_projectiles[i].go.pos.y + m_flameTop[m_projectiles[i].go.frameCount]) && (m_pObject[j]->go.pos.y < m_projectiles[i].go.pos.y + m_flameTop[m_projectiles[i].go.frameCount] + m_flameHeight[m_projectiles[i].go.frameCount])){
              //m_pObject[j]->go.alive = false;
              m_pObject[j]->go.dying = true;
              m_pObject[j]->go.frameCount = 0;
              m_pObject[j]->go.maxFrames = 8;
              m_pObject[j]->go.spriteID = m_pObject[j]->go.spriteBadID;
              if(m_pObject[j]->go.layer == 7 && m_pObject[j]->go.radius > 0)//trees
                m_pObject[j]->go.updateInterval = .5;
              else
                m_pObject[j]->go.updateInterval = .125;

              if(m_pObject[j]->go.objectID == 27){ //elf
                m_player.m_score += m_currentLevel * 10;
                m_player.m_kindness -=  8;
              }
              else if(m_pObject[j]->go.objectID == 28){ //fairy
                m_player.m_score += m_currentLevel * 20;
                m_player.m_kindness -=  8;
              }
              else if(m_pObject[j]->go.objectID == 29){ //witch
                m_player.m_score += m_currentLevel * 30;
                m_player.m_kindness -=  8;
              }

          }
        }
        //flame right
        else if(m_projectiles[i].go.angle < 1.6){
          if((m_pObject[j]->go.pos.y > m_projectiles[i].go.pos.y + 30) && (m_pObject[j]->go.pos.y < m_projectiles[i].go.pos.y + 60) &&
             (m_pObject[j]->go.pos.x > m_projectiles[i].go.pos.x + 120 - 64 - m_flameHeight[m_projectiles[i].go.frameCount] -  m_flameTop[m_projectiles[i].go.frameCount]) && (m_pObject[j]->go.pos.x < m_projectiles[i].go.pos.x + 120 - 64 - m_flameTop[m_projectiles[i].go.frameCount])){
            //m_pObject[j]->go.alive = false;
              m_pObject[j]->go.dying = true;
              m_pObject[j]->go.frameCount = 0;
              m_pObject[j]->go.maxFrames = 8;
              m_pObject[j]->go.spriteID = m_pObject[j]->go.spriteBadID;
              if(m_pObject[j]->go.layer == 7 && m_pObject[j]->go.radius > 0)//trees
                m_pObject[j]->go.updateInterval = .5;
              else
                m_pObject[j]->go.updateInterval = .125;

              if(m_pObject[j]->go.objectID == 27){ //elf
                m_player.m_score += m_currentLevel * 10;
                m_player.m_kindness -= 8;
              }
              else if(m_pObject[j]->go.objectID == 28){ //fairy
                m_player.m_score += m_currentLevel * 20;
                m_player.m_kindness -= 8;
              }
              else if(m_pObject[j]->go.objectID == 29){ //witch
                m_player.m_score += m_currentLevel * 30;
                m_player.m_kindness -= 8;
              }

             }
        }

        //flame down
        else if(m_projectiles[i].go.angle < 3.2){
          if((m_pObject[j]->go.pos.x > m_projectiles[i].go.pos.x - 15) && (m_pObject[j]->go.pos.x < m_projectiles[i].go.pos.x + 15) &&
             (m_pObject[j]->go.pos.y > m_projectiles[i].go.pos.y + 120 - m_flameTop[m_projectiles[i].go.frameCount] - m_flameHeight[m_projectiles[i].go.frameCount]) && (m_pObject[j]->go.pos.y < m_projectiles[i].go.pos.y + 120 - m_flameTop[m_projectiles[i].go.frameCount])){
              //m_pObject[j]->go.alive = false;
              m_pObject[j]->go.dying = true;
              m_pObject[j]->go.frameCount = 0;
              m_pObject[j]->go.maxFrames = 8;
              m_pObject[j]->go.spriteID = m_pObject[j]->go.spriteBadID;
              if(m_pObject[j]->go.layer == 7 && m_pObject[j]->go.radius > 0)//trees
                m_pObject[j]->go.updateInterval = .5;
              else
                m_pObject[j]->go.updateInterval = .125;

              if(m_pObject[j]->go.objectID == 27){ //elf
                m_player.m_score += m_currentLevel * 10;
                m_player.m_kindness -= 8;
              }
              else if(m_pObject[j]->go.objectID == 28){ //fairy
                m_player.m_score += m_currentLevel * 20;
                m_player.m_kindness -= 8;
              }
              else if(m_pObject[j]->go.objectID == 29){ //witch
                m_player.m_score += m_currentLevel * 30;
                m_player.m_kindness -= 8;
              }

           }
        }

        //flame left
        else if(m_projectiles[i].go.angle < 4.8){
          if((m_pObject[j]->go.pos.y > m_projectiles[i].go.pos.y + 30) && (m_pObject[j]->go.pos.y < m_projectiles[i].go.pos.y + 60) &&
             (m_pObject[j]->go.pos.x > m_projectiles[i].go.pos.x - 120 + 64 + m_flameTop[m_projectiles[i].go.frameCount]) && (m_pObject[j]->go.pos.x < m_projectiles[i].go.pos.x - 120 + 64 + m_flameTop[m_projectiles[i].go.frameCount] + m_flameHeight[m_projectiles[i].go.frameCount])){
            //m_pObject[j]->go.alive = false;
              m_pObject[j]->go.dying = true;
              m_pObject[j]->go.frameCount = 0;
              m_pObject[j]->go.maxFrames = 8;
              m_pObject[j]->go.spriteID = m_pObject[j]->go.spriteBadID;
              if(m_pObject[j]->go.layer == 7 && m_pObject[j]->go.radius > 0)//trees
                m_pObject[j]->go.updateInterval = .5;
              else
                m_pObject[j]->go.updateInterval = .125;

               if(m_pObject[j]->go.objectID == 27){ //elf
                m_player.m_score += m_currentLevel * 10;
                m_player.m_kindness -=  8;
              }
              else if(m_pObject[j]->go.objectID == 28){ //fairy
                m_player.m_score += m_currentLevel * 20;
                m_player.m_kindness -=  8;
              }
              else if(m_pObject[j]->go.objectID == 29){ //witch
                m_player.m_score += m_currentLevel * 30;
                m_player.m_kindness -= 8;
              }

             }
        }
      }
    }

    if(bRocket == true)
      m_projectiles[i].go.alive = false;    
  }

}

bool CGameData::IsElfCollision(int elf, int index){
  CLog *pLog = CLog::Instance();
  
  if(index < 0 || index > m_pObject.size() - 1)
    return false;

  float cx,cy,cw,ch;            
  float dx, dy;
  float ox, oy;
  float mx, my;
    
  //step 1 of algorithm
  ox = m_pObject[index]->go.pos.x - m_pObject[elf]->go.pos.x;
  oy = m_pObject[index]->go.pos.y - m_pObject[elf]->go.pos.y;

  //step #2 of algorithm
  if(abs(ox) < 32 && abs(oy) < 32){
    
    //step #3 of algorithm
    mx = 0;
    my = 0;

    if(ox > 0)
      mx = ox -32;
    else if(ox < 0)
      mx = ox + 32;
    else if(ox == 0)
      mx = 0;

    if(oy > 0)
      my = oy - 32;
    else if(oy < 0)
      my = oy + 32;
    else if(oy == 0)
      my = 0;

    //pLog->Log(ox, oy, mx, my);
    //step #4 of algorithm
    m_pObject[elf]->go.pos.x += mx;
    m_pObject[elf]->go.pos.y += my;

    return true;
  }

    /*
    dx = abs(m_player.go.pos.x - m_pObject[index]->go.pos.x);
    dy = abs(m_player.go.pos.y - m_pObject[index]->go.pos.y);
    cx = m_pObject[index]->go.width/2 + 16;
    cy = m_pObject[index]->go.height/2 + 16;

    if(dx < (cx - 4) && dy < (cy - 4)){  //6
      if(m_player.go.pos.x <= m_pObject[index]->go.pos.x)      
        m_player.go.pos.x -= 8;
      else
        m_player.go.pos.x += 8;

      if(m_player.go.pos.y <= m_pObject[index]->go.pos.y)      
        m_player.go.pos.y -= 8;
      else
        m_player.go.pos.y += 8;

      return true;   
    }
    */

  return false;
}

