/**************************************************************************************
Filename: CGameData.h Date: June 1, 2008
Purpose: Manages all game data.
**************************************************************************************/

#ifndef CGAMEDATA_H
#define CGAMEDATA_H
#include <string>
#include <fstream>
#include <iostream>
#include <windows.h>
#include "CLog.h"
#include "CINIReader.h"
#include "CFileReader.h"
#include "CGraphics.h"
#include "CObject.h"
#include "CLandscape.h"
#include "CCharacter.h"
#include "CGeneric.h"
#include "CPlayer.h"
#include "CProjectile.h"
#include "CElf.h"
#include "CFairy.h"
#include "CWitch.h"
#include "COgre.h"
#include "CGrandma.h"
#include "CAudioManager.h"

//constants used for game objects
const int C_MESH = 0;
const int C_SPLATTER = 1;
const int C_SPRITE = 2;//
const int C_PLAYER = 3;//
const int C_WITCH = 4;
const int C_ELF = 5;
const int C_FAIRY = 6;
const int C_OGRE = 7;
const int C_SHOTGUN = 9;
const int C_MACHINEGUN = 10;
const int C_GRENADE = 11;
const int C_AMMO_SG = 12;
const int C_AMMO_MG = 13;
const int C_ROCKETLAUNCHER = 14;
const int C_AMMO_ROCKET = 15;
const int C_AMMO_GUN = 16;
const int C_FLAME_THROWER = 17;
const int C_FUEL_FLAME = 18;
const int C_TANK = 19;
const int C_AMMO_TANK = 20;
const int C_FUEL_TANK = 21;
const int C_HELICOPTER = 22;
const int C_BAT = 23;
const int C_SWORD = 24;
const int C_BOW = 25;
const int C_AMMO_ARROWS = 26;
const int C_PISTOL = 27;
const int C_GRASS = 28;
const int C_DIRT = 29;
const int C_WALL = 30;
const int C_TREE1 = 31;
const int C_PRIZE = 32;
const int C_POWER = 33;
const int C_GRANDMA = 34;
const int C_DESTINATION = 35;
const int C_WORLD = 36;
const int C_TREE2 = 37;
const int C_TREE3 = 38;

//weapon inventory
const int WEAPON_BAT = 0;
const int WEAPON_PISTOL = 1;
const int WEAPON_SHOTGUN = 2;
const int WEAPON_MACHINEGUN = 3;
const int WEAPON_ROCKET = 4;
const int WEAPON_FLAMETHROWER = 5;

struct WEAPON_TYPE{
  bool exists;
  int rounds;
  int maxRounds;
};



class CGameData{
public:
  CGameData();
  bool LoadConfigFile(std::string filename);
  void AddGameObject(GRAPHIC_IMAGE gi, int x, int y);
  void AddGameObject(int spriteID, int x, int y);
  void AddGameObject(GAME_OBJECT go);//, int x, int y);
  GAME_OBJECT GetObjectInfo(int objectID);
  int GameObjectTypeSize();
  std::string GetObjectTypeName(int objectID);

  GAME_OBJECT GetLevelData(int i);
  void UpdateLevelData(int i, GAME_OBJECT gob);  
  void LogKeyValue();
  void LoadGameLevel(std::string filename, CGraphics &console);
  void SaveGameLevel(std::string filename);
  void LoadGameLevelBU(std::string filename);
  int LevelSize();
  void ClearLevel(){m_level.clear();}
  bool LoadAssetFile(std::string filename);
  bool IsPlayerCollision(int index);
  bool IsElfCollision(int elf, int index);
  
  int m_FPS;
  std::string m_version;
  HWND m_hWnd;

  //mouse
  POINTS m_mouse;         //Mouse g_mouse
  bool m_bLeftMouseDown;
  bool m_bRightMouseDown;

  //screen information
  int m_screenLeft;
  int m_screenTop;
  int m_screenWidth;
  int m_screenHeight;
  bool m_bFullScreen;

  //display debug info on screen
  bool m_displayDebugInfo;

  //sound
  bool m_playMusic;

  //level information from 'config.txt'
  int m_currentLevel;               //current level being played
  std::string m_levelFilename[10];  //filename containing level data
  std::string GetLevelFilename(int level); //retrieves filename
  int m_maxLevels;                  //last level
  std::vector<GAME_OBJECT> m_level;

  //player information
  CPlayer m_player;
  float m_playerX;
  float m_playerY;
  float m_lastPlayerX;
  float m_lastPlayerY;

  //world boundaries
  float m_world_left;
  float m_world_top;
  float m_world_right;
  float m_world_bottom;
  bool m_gameOver;
  //long m_score;
  bool m_levelComplete;
  bool m_playerQuit;
  bool m_timeOut;
  bool m_healthOut;
  bool m_kindnessOut;

  //std::vector<CLandscape> m_landscape;
  std::vector<CObject*> m_pObject;      //points to all game objects
  std::vector<int> m_screen;            //these are screen objects
  std::vector<GAME_OBJECT_TYPE> m_objectType; //stores each type of game object allowed

  WEAPON_TYPE m_weapons[8];
  int m_weaponType;
  std::vector<CProjectile> m_projectiles;
  void AddProjectile(float x, float y, float vx, float vy, int type, float dir);
  void UpdateProjectiles(double timeDifference);

  std::vector<CGrandma> m_grandma;

  COgre m_ogre;
  float m_flameTop[16];
  float m_flameHeight[16];

private:
  
};

#endif