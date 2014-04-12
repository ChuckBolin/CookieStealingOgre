/**************************************************************************************
Filename: CGraphics.h Date: June 1, 2008
Purpose: Manages all DirectX
Initial code: http://www.directxtutorial.com/Tutorial9/B-Direct3DBasics/dx9B2.aspx

**************************************************************************************/

#ifndef CGRAPHICS_H
#define CGRAPHICS_H
#include <windows.h>
#include <vector>
#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include <sstream>
#include "CLog.h"
#include "CMesh.h"
#include <sstream>
#include "CFileReader.h"
#include "CObject.h"

const int F_A12 = 0;  //Arial
const int F_A12B = 1;
const int F_A14 = 2;
const int F_A14B = 3;
const int F_A16 = 4;
const int F_A16B = 5;
const int F_V12 = 6;  //Verdana
const int F_V12B = 7;
const int F_V14 = 8;
const int F_V14B = 9;
const int F_V16 = 10;
const int F_V16B = 11;
const int F_V20 = 12;
const int F_V20B = 13;
const int C_NUMBER_FONTS = 16;

//used with all sprites (graphic images)
struct GRAPHIC_IMAGE{
  int spriteID;
  int fileID;
  int sourceX;
  int sourceY;
  int width;
  int height;
  float angle;
  int maxFrames;
  double updateInterval;
  std::string description;
  float scale;
  bool bValid;
  int alpha;
};

//structure for 3D buildings
struct MESH{
  float x;
  float y;
  float z;
  float rotateY;
};

struct MESH3D{
  int id;
  float sx, sy, sz; //scale
  LPD3DXMESH m_pMesh;
  D3DMATERIAL9* m_pMeshMaterials;
  LPDIRECT3DTEXTURE9* m_pMeshTextures;
  DWORD m_dwNumMaterials;
};

//*******************************************
//structures store textures for rendering
//*******************************************
struct S_TEXTURE{
  int id;
  IDirect3DTexture9* pBuffer;
};

//********************************************
//structures for various asset data
//this is loaded from file into vectors
struct GRAPHIC_FILE_DATA{
  int fileID;
  std::string filename;
  std::string description;
};

struct MESH_DATA{
  int meshID;
  std::string filename;
  float sx, sy, sz;
  std::string description;
};

//***************************************************  
struct CUSTOMVERTEX {FLOAT X, Y, Z, RHW; DWORD COLOR;};
#define CUSTOMFVF (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

class CGraphics{
public:

  //public methods
  CGraphics(HWND hWnd, int screenWidth, int screenHeight, bool fullScreen);
  bool InitializeDirectX();
  void CleanupDirectX();
  D3DFORMAT FindDepthStencilFormat( ULONG AdapterOrdinal, D3DDISPLAYMODE Mode, D3DDEVTYPE DevType );
  template <typename T1> void Print(T1 value, int fontType, int posX, int posY, int red, int green, int blue, int alpha = 255);
  template <typename T1, typename T2> void Print(T1 value, T2 value2, int fontType, int posX, int posY, int red, int green, int blue, int alpha = 255);
  int GetScreenWidth(){return m_screenWidth;};
  int GetScreenHeight(){return m_screenHeight;};
  int GetScreenCenterX(){return m_screenWidth/2;};
  int GetScreenCenterY(){return m_screenHeight/2;};
  HWND GetWindowHandle(){return m_hWnd;};
  D3DPRESENT_PARAMETERS GetDPP(){return m_d3dpp;};
  void ReloadLost();
  void DeleteLost();
  void RenderMesh();
  void LoadMesh(int meshID, std::string filename, float sx, float sy, float sz);
  void RenderMesh(int id, float wx, float wy, float wz);
  void RenderMesh(GAME_OBJECT &go);
  void RenderSplatter(int id, float x, float y, float z, float ry);
  void RenderSplatter(int id, float x, float y, float z, float rx, float ry, float rz);
  bool LoadAssetFile(std::string filename);
  void RenderGraphicModulate(int positionX, int positionY, int red, int green, int blue);
  void RenderGraphicModulate(GRAPHIC_IMAGE gi, int positionX, int positionY, int red, int green, int blue);
  void RenderGraphicModulate(GAME_OBJECT go, int positionX, int positionY, int red, int green, int blue);
  void RenderGraphicModulate(GAME_OBJECT go, int red, int green, int blue);
  void LoadGraphicFile(int fileID, std::string filename);
  IDirect3DTexture9* GetTexture(int id);
  GRAPHIC_IMAGE GetSprite(int id);
  GRAPHIC_IMAGE CGraphics::GetSprite2(int i);
  void DrawLine(float x1, float y1,float x2, float y2, int red, int green, int blue);
  void DrawRect(float x1, float y1,float x2, float y2, int red, int green, int blue);
  void DrawCircle(float x, float y, int red, int green, int blue, float radius);
  int GetSpriteCount();
  float GetLayerFloatZ(int layer);

  //private variables
  LPDIRECT3DDEVICE9 m_d3ddev; // the pointer to the device class  
  ID3DXSprite* m_pSprite;//2D sprite rendering

  std::vector<GRAPHIC_FILE_DATA> m_assetFileData;
  std::vector<GRAPHIC_IMAGE> m_assetSpriteData; 
  std::vector<MESH_DATA> m_assetMeshData;
  std::vector<S_TEXTURE> m_texture;
  std::vector<GRAPHIC_IMAGE> m_sprites;

private:

  //private methods
  void InitializeFonts();

  //private variables
  LPDIRECT3D9 m_d3d; // the pointer to our Direct3D interface
  HWND m_hWnd;
  int m_screenWidth;
  int m_screenHeight;
  bool m_fullScreen;
  ID3DXFont *m_Font[C_NUMBER_FONTS];
  D3DPRESENT_PARAMETERS m_d3dpp;
  IDirect3DVertexBuffer9* m_textureQuad;

  //3d model stuff
  LPD3DXMESH m_pMesh;
  D3DMATERIAL9* m_pMeshMaterials;
  LPDIRECT3DTEXTURE9* m_pMeshTextures;
  DWORD m_dwNumMaterials;
  LPD3DXMESH m_pMesh1;
  D3DMATERIAL9* m_pMeshMaterials1;
  LPDIRECT3DTEXTURE9* m_pMeshTextures1;
  DWORD m_dwNumMaterials1;
  std::vector<MESH3D> m_mesh3d;   
};

//**********************************************************************
// Print(value1, ....)
//**********************************************************************
template <typename T1> void CGraphics::Print(T1 value, int fontType, int posX, int posY, int red, int green, int blue, int alpha){
  D3DCOLOR fontColor = D3DCOLOR_ARGB(alpha, red, green, blue);
  int width = 0;
  int fontHeight = 0;

  if(fontType == 0 || fontType == 1 || fontType == 6 || fontType == 7)
    fontHeight = 12;
  else if(fontType == 2 || fontType == 3 || fontType == 8 || fontType == 9)
    fontHeight = 14;
  else if(fontType == 4 || fontType == 5 || fontType == 10 || fontType == 11)
    fontHeight = 16;
  else if(fontType == 12 || fontType == 13)
    fontHeight = 20;
  
  std::ostringstream oss;
  oss << value;
  width = oss.str().size() * fontHeight;
  RECT rct;
  rct.left = posX;
  rct.top = posY;
  rct.right = rct.left + width;
  rct.bottom = rct.top + fontHeight;
  std::string output = oss.str();
  m_Font[fontType]->DrawText(NULL, output.c_str(), -1, &rct, 0, fontColor);  

}

//**********************************************************************
// Print(value1, value2, ....)
//**********************************************************************
template <typename T1, typename T2> void CGraphics::Print(T1 value, T2 value2, int fontType, int posX, int posY, int red, int green, int blue, int alpha){
  D3DCOLOR fontColor = D3DCOLOR_ARGB(alpha, red, green, blue);
  int width = 0;
  int fontHeight = 0;

  if(fontType == 0 || fontType == 1 || fontType == 6 || fontType == 7)
    fontHeight = 12;
  else if(fontType == 2 || fontType == 3 || fontType == 8 || fontType == 9)
    fontHeight = 14;
  else if(fontType == 4 || fontType == 5 || fontType == 10 || fontType == 11)
    fontHeight = 16;
  else if(fontType == 12 || fontType == 13)
    fontHeight = 20;
  
  std::ostringstream oss;
  oss << value << ": " << value2;
  width = oss.str().size() * fontHeight;
  RECT rct;
  rct.left = posX;
  rct.top = posY;
  rct.right = rct.left + width;
  rct.bottom = rct.top + fontHeight;
  std::string output = oss.str();
  m_Font[fontType]->DrawText(NULL, output.c_str(), -1, &rct, 0, fontColor);  
}

#endif