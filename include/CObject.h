//CObject.h
//base class...all other game states are derived from here
#ifndef OBJECT_H
#define OBJECT_H
#include <vector>
#include <d3d9.h>
#include <d3dx9.h>
#include <time.h>


struct GAME_OBJECT{
  int asset;        //0 = mesh, 1 = splatter
  int meshID;       //unique ID
  int meshBadID;    //ID of mesh used when damaged
  D3DXVECTOR3 pos;
  
  float x;          //position
  float y;
  float z;
  float rx;         //rotational angles - orientation
  float ry;
  float rz;
  //float collisionDiameter; 
  bool alive;
  bool dying;
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
  int maxFrames;
  double updateInterval;
  int objectID;
  int layer;
  float radius;
  float speed;
  int mode;
};

struct GAME_OBJECT_TYPE{
  std::string name;
  int objectID;
  int spriteID;
  int badSpriteID;
  int layer;
  float radius;
};

class CObject{
public:
  virtual void Update(double timeDifference) = 0;
  GAME_OBJECT go;
  D3DXVECTOR3 playerPos;
  virtual void UpdatePlayerPosition(float x, float y) = 0;

};

#endif //GAME_STATE_OBJECT_H