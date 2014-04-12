#include "../include/CGraphics.h"

//skybox vertex
struct Vertex3
{
	Vertex3(){}
	Vertex3(
		float x, float y, float z,
		//float nx, float ny, float nz,
		float u, float v)
	{
		_x  = x;  _y  = y;  _z  = z;
		//_nx = nx; _ny = ny; _nz = nz;
		_u  = u;  _v  = v;
	}
    float _x, _y, _z;
    //float _nx, _ny, _nz;
    float _u, _v; // texture coordinates

	static const DWORD SKYFVF;
};
const DWORD Vertex3::SKYFVF = D3DFVF_XYZ |  D3DFVF_TEX1;//D3DFVF_NORMAL | D3DFVF_TEX1;

/*******************************************************************
  CGraphics constructor
*******************************************************************/
CGraphics::CGraphics(HWND hWnd, int screenWidth, int screenHeight, bool fullScreen){
  m_hWnd = hWnd;
  m_screenWidth = screenWidth;
  m_screenHeight = screenHeight;
  m_fullScreen = fullScreen;
}

/*******************************************************************
  Initialize DirectX.
*******************************************************************/
bool CGraphics::InitializeDirectX(){
  m_d3d = Direct3DCreate9(D3D_SDK_VERSION); // create the Direct3D interface
  if(!m_d3d){
    ::MessageBox(m_hWnd, "DirectX failed!", "Fatal Error!", MB_OK | MB_ICONSTOP | MB_APPLMODAL);
    return false;
  }

  ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));    // clear out the struct for use
	D3DDISPLAYMODE        CurrentMode;
  
  // Select back buffer format etc
	m_d3d->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &CurrentMode);

  //controlled by config.txt
  m_d3dpp.Windowed = !m_fullScreen;  //windowed
  m_d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;    // set the back buffer format to 32-bit
  m_d3dpp.BackBufferWidth = m_screenWidth;    // set the width of the buffer
  m_d3dpp.BackBufferHeight = m_screenHeight;    // set the height of the buffer
  m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;    // discard old frames
  m_d3dpp.hDeviceWindow = m_hWnd;    // set the window to be used by Direct3D
  m_d3dpp.PresentationInterval	= D3DPRESENT_INTERVAL_IMMEDIATE;
  m_d3dpp.FullScreen_RefreshRateInHz = m_fullScreen ? CurrentMode.RefreshRate : 0;
  m_d3dpp.EnableAutoDepthStencil = TRUE;
  m_d3dpp.AutoDepthStencilFormat		=  FindDepthStencilFormat( D3DADAPTER_DEFAULT, CurrentMode, D3DDEVTYPE_HAL );

  // create a device class using this information and the info from the d3dpp stuct
  m_d3d->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,m_hWnd,
                    D3DCREATE_SOFTWARE_VERTEXPROCESSING, &m_d3dpp, &m_d3ddev);
  
  if(FAILED( D3DXCreateSprite(m_d3ddev, &m_pSprite)) )
  {
    ::MessageBox(m_hWnd, "Failed to create a sprite!", "Fatal Error!", 0);//MB_OK | MB_ICONSTOP | MB_APPLMODAL);
    return false;
  }

  ReloadLost();

  return true;
}

/*******************************************************************
  Clean up DirectX
*******************************************************************/
void CGraphics::CleanupDirectX(){
  CLog *pLog = CLog::Instance();
  pLog->Log("Cleanup");

  DeleteLost();
  pLog->Log("cleanup 1");
  m_d3ddev->Release(); // close and release the 3D device
  m_d3d->Release(); // close and release Direct3D
  
  pLog->Log("cleanup 2");
    return;
}

/*******************************************************************
  Define all fonts
*******************************************************************/
void CGraphics::InitializeFonts(){

  //load 12 different fonts for Arial and Verdana, 12-16, normal and bold
  D3DXCreateFont(m_d3ddev, 12, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, 
    OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
    DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_Font[F_A12] );
  D3DXCreateFont(m_d3ddev, 12, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, 
    OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
    DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_Font[F_A12B] );
  D3DXCreateFont(m_d3ddev, 14, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, 
    OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
    DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_Font[F_A14] );
  D3DXCreateFont(m_d3ddev, 14, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, 
    OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
    DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_Font[F_A14B] );
  D3DXCreateFont(m_d3ddev, 16, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, 
    OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
    DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_Font[F_A16] );
  D3DXCreateFont(m_d3ddev, 16, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, 
    OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
    DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_Font[F_A16B] );
  D3DXCreateFont(m_d3ddev, 12, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, 
    OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
    DEFAULT_PITCH | FF_DONTCARE, TEXT("Verdana"), &m_Font[F_V12] );
  D3DXCreateFont(m_d3ddev, 12, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, 
    OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
    DEFAULT_PITCH | FF_DONTCARE, TEXT("Verdana"), &m_Font[F_V12B] );
  D3DXCreateFont(m_d3ddev, 14, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, 
    OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
    DEFAULT_PITCH | FF_DONTCARE, TEXT("Verdana"), &m_Font[F_V14] );
  D3DXCreateFont(m_d3ddev, 14, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, 
    OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
    DEFAULT_PITCH | FF_DONTCARE, TEXT("Verdana"), &m_Font[F_V14B] );
  D3DXCreateFont(m_d3ddev, 16, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, 
    OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
    DEFAULT_PITCH | FF_DONTCARE, TEXT("Verdana"), &m_Font[F_V16] );    
  D3DXCreateFont(m_d3ddev, 16, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, 
    OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
    DEFAULT_PITCH | FF_DONTCARE, TEXT("Verdana"), &m_Font[F_V16B] );
  D3DXCreateFont(m_d3ddev, 20, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, 
    OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
    DEFAULT_PITCH | FF_DONTCARE, TEXT("Verdana"), &m_Font[F_V20] );
  D3DXCreateFont(m_d3ddev, 20, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, 
    OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
    DEFAULT_PITCH | FF_DONTCARE, TEXT("Verdana"), &m_Font[F_V20B] );
}

void CGraphics::ReloadLost(){
  CLog *pLog = CLog::Instance();
  InitializeFonts();

  //lighting stuff
  m_d3ddev->SetRenderState(D3DRS_LIGHTING, true);
  D3DLIGHT9 dir;
  ::ZeroMemory(&dir, sizeof(dir));
  dir.Type = D3DLIGHT_DIRECTIONAL;
  dir.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
  dir.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f) * 0.3f;
  dir.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f) * 2.6f;
  dir.Direction = D3DXVECTOR3(0.0f, -100.0f, 0.0f);

  m_d3ddev->SetLight(0, &dir);
  m_d3ddev->LightEnable(0, true);
  m_d3ddev->SetRenderState(D3DRS_NORMALIZENORMALS, true);
  m_d3ddev->SetRenderState(D3DRS_SPECULARENABLE, true);
}

void CGraphics::DeleteLost(){
  CLog *pLog = CLog::Instance();


  for(int i = 0; i < C_NUMBER_FONTS; i++)
    m_Font[i] = NULL; 

  m_assetSpriteData.empty();// .clear();
  m_assetFileData.empty();// .erase( .erase( );//clear();
  m_assetMeshData.empty();//clear();
}

/*
void CGraphics::SetupStuff(){
  CLog *pLog = CLog::Instance();
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj,
			                       D3DX_PI * 0.25f, // 90 - degree
			                       (float)m_screenWidth / (float)m_screenHeight,                              
			                       0.1f,
			                       800.0f);
  
	m_d3ddev->SetTransform(D3DTS_PROJECTION, &proj);
  m_d3ddev->SetRenderState(D3DRS_ZENABLE, true);
	m_d3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);//D3DFILL_SOLID);// D3DFILL_WIREFRAME);
}
*/

float CGraphics::GetLayerFloatZ(int layer){
  if(layer <= 2)
    return 1.0f;
  if(layer == 3)
    return 0.8f;
  if(layer == 4)
    return 0.6f;
  if(layer == 5)
    return 0.4f;
  if(layer == 6)
    return 0.2f;
  if(layer >= 7)
    return 0.0f;
}

//*****************************************************************************
// Renders a sprite based upon graphic image sprite data
//*****************************************************************************
void CGraphics::RenderGraphicModulate(GRAPHIC_IMAGE gi, int positionX, int positionY, int red, int green, int blue){
  //CLog *pLog = CLog::Instance();
  if(gi.bValid == false)
    return;
  
  RECT rect;
  rect.left = gi.sourceX ;
  rect.top = gi.sourceY;
  rect.right = gi.sourceX + gi.width;// * 2;
  rect.bottom = gi.sourceY + gi.height;// * 2;  
  //D3DXVECTOR3 pLayer((float)gi.sourceX, (float)gi.sourceY, gi.layer); 

  if(NULL == m_pSprite)
    return;
  
  m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
  m_d3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  D3DXVECTOR2 pos;
  //if(gi.spriteID == 51){
  //  pos.x = positionX;// - gi.width/4;
  //  pos.y = positionY;// - gi.height/4;
  //}
  //else{
    pos.x = positionX;// + gi.width/2;
    pos.y = positionY;// + gi.height/2;
  //}
  D3DXVECTOR2 scaling;
  scaling.x = gi.scale;
  scaling.y = gi.scale;
  D3DXVECTOR2 rotcenter;
  rotcenter.x = (rect.right - rect.left)/2;
  rotcenter.y = (rect.bottom - rect.top)/2;
  D3DXMATRIX mat;

  D3DXMatrixTransformation2D(&mat, NULL, 0.0, &scaling, &rotcenter, gi.angle, &pos);
  m_pSprite->SetTransform(&mat);  
  
  if(NULL != GetTexture(gi.fileID)){
    if(FAILED(m_pSprite->Draw(GetTexture(gi.fileID), &rect, NULL, NULL, D3DCOLOR_ARGB(gi.alpha , red, green, blue)) )){
    }
  }
  D3DXMatrixIdentity(&mat);
  m_pSprite->SetTransform(&mat);
  m_pSprite->End();
}

//*****************************************************************************
// Renders a sprite based upon game object
//*****************************************************************************
void CGraphics::RenderGraphicModulate(GAME_OBJECT go, int positionX, int positionY, int red, int green, int blue){
  CLog *pLog = CLog::Instance();
  
  GRAPHIC_IMAGE gi;
  gi = GetSprite(go.spriteID);

  //if(gi.bValid == false)
  //  return;
  RECT rect;

  //used for animation
  if(go.maxFrames > 1){
    rect.left = gi.sourceX + (go.frameCount * gi.width); 
    rect.right = rect.left + gi.width;
  }
  //static sprite
  else{
    rect.left = gi.sourceX;
    rect.right = gi.sourceX + gi.width;
  }

  rect.top = gi.sourceY;  
  rect.bottom = gi.sourceY + gi.height;// * 2;

  if(NULL == m_pSprite)
    return;
  
  m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
  m_d3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  D3DXVECTOR2 pos;
  pos.x = positionX;
  pos.y = positionY;
  D3DXVECTOR2 scaling;
  scaling.x = gi.scale;
  scaling.y = gi.scale;
  D3DXVECTOR2 rotcenter;
  rotcenter.x = (rect.right - rect.left)/2;
  rotcenter.y = (rect.bottom - rect.top)/2;
  D3DXMATRIX mat;
 
  D3DXMatrixTransformation2D(&mat, NULL, 0.0, &scaling, &rotcenter, go.angle, &pos);
  m_pSprite->SetTransform(&mat);  
  
  if(NULL != GetTexture(gi.fileID)){
    if(FAILED(m_pSprite->Draw(GetTexture(gi.fileID), &rect, NULL, NULL, D3DCOLOR_ARGB(gi.alpha , red, green, blue)) )){
    }
  }
  D3DXMatrixIdentity(&mat);
  m_pSprite->SetTransform(&mat);
  m_pSprite->End();
}

//*****************************************************************************
// Renders a sprite based upon game object
//*****************************************************************************
void CGraphics::RenderGraphicModulate(GAME_OBJECT go, int red, int green, int blue){
  //CLog *pLog = CLog::Instance();
  
  GRAPHIC_IMAGE gi;
  gi = GetSprite(go.spriteID);
  
  if(gi.bValid == false)
    return;

  RECT rect;

  //used for animation
  if(go.maxFrames > 1){
    rect.left = gi.sourceX + ((go.frameCount) * gi.width); 
    rect.right = rect.left + gi.width;
  }

  //static sprite
  else{
    rect.left = gi.sourceX;
    rect.right = gi.sourceX + gi.width;
  }
  rect.top = gi.sourceY;  
  rect.bottom = gi.sourceY + gi.height;// * 2;

  if(NULL == m_pSprite)
    return;  
  
  //D3DXVECTOR3 pLayer((float)gi.sourceX, (float)gi.sourceY, go.layer); 

  m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
  m_d3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  D3DXVECTOR2 pos;
  pos.x = go.pos.x - go.width/2;
  pos.y = go.pos.y - go.height/2;
  D3DXVECTOR2 scaling;
  scaling.x = gi.scale;
  scaling.y = gi.scale;
  D3DXVECTOR2 rotcenter;
  rotcenter.x = (rect.right - rect.left)/2;
  rotcenter.y = (rect.bottom - rect.top)/2;
  D3DXMATRIX mat;

  D3DXMatrixTransformation2D(&mat, NULL, 0.0, &scaling, &rotcenter, go.angle, &pos);
  m_pSprite->SetTransform(&mat);  
  
  if(NULL != GetTexture(gi.fileID)){
    if(FAILED(m_pSprite->Draw(GetTexture(gi.fileID), &rect, NULL, NULL, D3DCOLOR_ARGB(gi.alpha , red, green, blue)) )){
    }
  }
  D3DXMatrixIdentity(&mat);
  m_pSprite->SetTransform(&mat);
  m_pSprite->End();
}


//********************************************************
//unique mesh id, model filename, and scaled matrix
//********************************************************
void CGraphics::LoadMesh(int meshID, std::string filename, float sx, float sy, float sz){
  CLog *pLog = CLog::Instance();
  pLog->Log("LoadMesh", meshID, filename);
  
  //create variables
  std::string sPath;
  std::ostringstream oss;
  LPD3DXBUFFER pD3DXMtrlBuffer;
  HRESULT hr;
  std::string sourceMesh;
  sourceMesh = "assets//models//" + filename;
  
  //used to store mesh data
  MESH3D tempMesh;
  tempMesh.sx = sx;
  tempMesh.sy = sy;
  tempMesh.sz = sz;
  tempMesh.id = meshID;

  //load mesh
  hr = D3DXLoadMeshFromX(sourceMesh.c_str(),
                          D3DXMESH_SYSTEMMEM,
                          m_d3ddev, NULL,
                          &pD3DXMtrlBuffer, NULL,
                          &tempMesh.m_dwNumMaterials,
                          &tempMesh.m_pMesh);

  if(FAILED(hr)){
    pLog->Log("Failure loading mesh");
    return;
  }
  
  //prepare to retrieve data from .x file
  D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
  tempMesh.m_pMeshMaterials = new D3DMATERIAL9[tempMesh.m_dwNumMaterials];
  tempMesh.m_pMeshTextures = new LPDIRECT3DTEXTURE9[tempMesh.m_dwNumMaterials];

  //iterate through all materials
  for(DWORD i = 0; i < tempMesh.m_dwNumMaterials; ++i){
    tempMesh.m_pMeshMaterials[i] = d3dxMaterials[i].MatD3D;
    tempMesh.m_pMeshMaterials[i].Ambient = tempMesh.m_pMeshMaterials[i].Diffuse;
    tempMesh.m_pMeshTextures[i] = NULL;

    //this mesh has a texture
    if(d3dxMaterials[i].pTextureFilename != NULL &&
      lstrlen(d3dxMaterials[i].pTextureFilename) > 0){
      
      //.x files and their skin textures are in the same folder
      oss.str("");
      oss << d3dxMaterials[i].pTextureFilename;
      sPath = "assets//models//" + oss.str();

      //create texture
      if(FAILED(D3DXCreateTextureFromFile(m_d3ddev,
                                          sPath.c_str(),//d3dxMaterials[i].pTextureFilename,
                                          &tempMesh.m_pMeshTextures[i]))){
                                            pLog->Log("Texture failed to load", i);
                                          }
      }
  }
  
  //add mesh to m_mesh3d (stores all meshes)
  m_mesh3d.push_back(tempMesh);
  
  //cleanup
  pD3DXMtrlBuffer->Release();
  pLog->Log("Mesh loaded", sourceMesh);
}

//********************************************************
//unique mesh id, world coordinates GAME_OBJECT
//********************************************************
void CGraphics::RenderMesh(GAME_OBJECT &go){
  CLog *pLog = CLog::Instance();

  //verify id is a valid mesh
  int index = -1;
  for(int i = 0; i < m_mesh3d.size(); ++ i){
    //if(m_mesh3d[i].id == go.meshID) //||
    if(go.alive == true && m_mesh3d[i].id == go.meshID){     
      index = i;
      break;
    }
    else if(go.alive == false && m_mesh3d[i].id == go.meshBadID){     
      index = i;
      break;
    }    
  }

  if(index < 0){
    pLog->Log("Incorrect mesh id", go.meshID);
    return;
  }

  D3DXMATRIX matW, matS, matR, matP;
  D3DXMatrixIdentity(&matW);//world matrix
  D3DXMatrixIdentity(&matS);//scaling matrix
  D3DXMatrixIdentity(&matR);//rotational matrix
  
  D3DXMatrixRotationZ(&matP, go.rz);
  D3DXMatrixMultiply(&matR, &matR, &matP);
  D3DXMatrixRotationY(&matP, go.ry);
  D3DXMatrixMultiply(&matR, &matR, &matP);
  D3DXMatrixRotationX(&matP, go.rx);  
  D3DXMatrixMultiply(&matR, &matR, &matP);

  D3DXMatrixScaling(&matS, m_mesh3d[index].sx, m_mesh3d[index].sy, m_mesh3d[index].sz);
  D3DXMatrixTranslation(&matW, go.x, go.y, go.z);
  m_d3ddev->SetTransform( D3DTS_WORLD, &(matR * matS * matW));

  for(DWORD i = 0; i < m_mesh3d[index].m_dwNumMaterials; ++i){
    
    m_d3ddev->SetMaterial(&m_mesh3d[index].m_pMeshMaterials[i]);
    m_d3ddev->SetTexture(0, m_mesh3d[index].m_pMeshTextures[i]);
    m_mesh3d[index].m_pMesh->DrawSubset(i);
  }
}

//********************************************************
//unique mesh id, world coordinates
//********************************************************
void CGraphics::RenderMesh(int id, float wx, float wy, float wz){
  CLog *pLog = CLog::Instance();

  //verify id is a valid mesh
  int index = -1;
  for(int i = 0; i < m_mesh3d.size(); ++ i){
    if(m_mesh3d[i].id == id){
      index = i;
      break;
    }
  }
  if(index < 0){
    pLog->Log("Incorrect mesh id", id);
    return;
  }
/*
  m_d3ddev->SetRenderState( D3DRS_ZENABLE, FALSE );
  m_d3ddev->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
  m_d3ddev->SetRenderState( D3DRS_LIGHTING, FALSE );
*/

  D3DXMATRIX matW, matS, matR;
  D3DXMatrixIdentity(&matW);//world matrix
  D3DXMatrixIdentity(&matS);//scaling matrix
  D3DXMatrixIdentity(&matR);//rotational matrix
  D3DXMatrixScaling(&matS, m_mesh3d[index].sx, m_mesh3d[index].sy, m_mesh3d[index].sz);
  D3DXMatrixTranslation(&matW, wx, wy, wz);
  m_d3ddev->SetTransform( D3DTS_WORLD, &(matR * matS * matW));

  for(DWORD i = 0; i < m_mesh3d[index].m_dwNumMaterials; ++i){
    m_d3ddev->SetMaterial(&m_mesh3d[index].m_pMeshMaterials[i]);
    m_d3ddev->SetTexture(0, m_mesh3d[index].m_pMeshTextures[i]);
    m_mesh3d[index].m_pMesh->DrawSubset(i);
  }

}

void CGraphics::RenderSplatter(int id, float x, float y, float z, float ry){
  m_d3ddev->SetRenderState( D3DRS_ZENABLE, FALSE );
  m_d3ddev->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
  m_d3ddev->SetRenderState( D3DRS_LIGHTING, FALSE );

	D3DXMATRIX V;
  m_d3ddev->SetFVF( Vertex3::SKYFVF );
  m_d3ddev->SetStreamSource( 0, m_textureQuad, 0, sizeof(Vertex3));

  //darkened texture
  m_d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
  m_d3ddev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
  m_d3ddev->SetRenderState(D3DRS_ALPHAREF, (DWORD)8);
  m_d3ddev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE); 

  m_d3ddev->SetTexture( 0, GetTexture(id));
  D3DXMatrixIdentity(&V);
  D3DXMatrixTranslation(&V, x, y, z);
  m_d3ddev->SetTransform(D3DTS_WORLD, &V);
  m_d3ddev->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

  m_d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
  m_d3ddev->SetRenderState( D3DRS_ZENABLE, TRUE );
  m_d3ddev->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
  m_d3ddev->SetRenderState( D3DRS_LIGHTING, TRUE );

}

void CGraphics::RenderSplatter(int id, float x, float y, float z, float rx, float ry, float rz){
  m_d3ddev->SetRenderState( D3DRS_ZENABLE, FALSE );
  m_d3ddev->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
  m_d3ddev->SetRenderState( D3DRS_LIGHTING, FALSE );

	D3DXMATRIX V;
  m_d3ddev->SetFVF( Vertex3::SKYFVF );
  m_d3ddev->SetStreamSource( 0, m_textureQuad, 0, sizeof(Vertex3));

  //darkened texture
  m_d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
  m_d3ddev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
  m_d3ddev->SetRenderState(D3DRS_ALPHAREF, (DWORD)8);
  m_d3ddev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE); 

  m_d3ddev->SetTexture( 0, GetTexture(id));
  D3DXMatrixIdentity(&V);

  D3DXMATRIX matR, matP;
  D3DXMatrixIdentity(&matR);//rotational matrix
  D3DXMatrixIdentity(&matP);//rotational matrix
  
  D3DXMatrixRotationZ(&matP, rz);
  D3DXMatrixMultiply(&matR, &matR, &matP);
  D3DXMatrixIdentity(&matP);//rotational matrix
  D3DXMatrixRotationY(&matP, ry);
  D3DXMatrixMultiply(&matR, &matR, &matP);
  D3DXMatrixIdentity(&matP);//rotational matrix
  D3DXMatrixRotationX(&matP, rx);  
  D3DXMatrixMultiply(&matR, &matR, &matP);
  
  D3DXMatrixTranslation(&V, x, y, z);


  m_d3ddev->SetTransform(D3DTS_WORLD, &(matR * V));
  m_d3ddev->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

  m_d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
  m_d3ddev->SetRenderState( D3DRS_ZENABLE, TRUE );
  m_d3ddev->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
  m_d3ddev->SetRenderState( D3DRS_LIGHTING, TRUE );

}


/*****************************************************
 Reads and loads data from assets.dat file into one of
 the vectors below.

 std::vector<GRAPHIC_FILE_DATA> m_assetFileData;
 std::vector<SPRITE_DATA> m_assetSpriteData;
 std::vector<MESH_DATA> m_assetMeshData;
 std::vector<SOUND_FILE_DATA> m_assetSoundData;

 Parameters in cfr begin with 1 not 0
*****************************************************/
bool CGraphics::LoadAssetFile(std::string filename){
  CLog *pLog = CLog::Instance();
  
  //read assets.dat file
  CFileReader cfr(filename);
  if(cfr.IsValid() == false)
    return false; //file is invalid or contains no usable data

  //validate asset data and add to appropriate vectors
  std::string parameter;
  std::string assetType;
  GRAPHIC_FILE_DATA   tempGraphic;
  GRAPHIC_IMAGE         tempSprite;
  MESH_DATA           tempMesh;

  for(int i = 0; i < cfr.GetNumberOfLines(); ++i){

    if(cfr.GetNumberOfTermsFromLine(i) > 1){
      assetType = cfr.GetTerm(cfr.GetLineFromFile(i), 1);
      
      if(assetType == "graphicfile" && cfr.GetNumberOfTermsFromLine(i) == 4){
        parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 2);//fileID
        tempGraphic.fileID = atoi(parameter.c_str());
        parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 3);//filename
        tempGraphic.filename = parameter;
        parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 4);//description
        tempGraphic.description = parameter;
        m_assetFileData.push_back(tempGraphic);
      }

      else if(assetType == "sprite" && cfr.GetNumberOfTermsFromLine(i) == 13){
        parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 2);//spriteID
        tempSprite.spriteID = atoi(parameter.c_str());
        parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 3);//fileID
        tempSprite.fileID = atoi(parameter.c_str());
        parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 4);//left
        tempSprite.sourceX = atof(parameter.c_str());
        parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 5);//right
        tempSprite.sourceY = atof(parameter.c_str());
        parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 6);//width
        tempSprite.width = atof(parameter.c_str());
        parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 7);//height
        tempSprite.height = atof(parameter.c_str());
        parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 8);//scale
        tempSprite.scale =  atof(parameter.c_str());
        parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 9);//alpha
        tempSprite.alpha = atoi(parameter.c_str());
        parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 10);//alpha
        tempSprite.angle = atof(parameter.c_str());
        parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 11);//no. frames
        tempSprite.maxFrames = atoi(parameter.c_str());
        parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 12);//time between frames
        tempSprite.updateInterval = atof(parameter.c_str());
        parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 13);//description
        tempSprite.description = parameter;
        m_assetSpriteData.push_back(tempSprite);
      }

      else if(assetType == "mesh" && cfr.GetNumberOfTermsFromLine(i) == 7){
        parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 2);//meshID
        tempMesh.meshID = atoi(parameter.c_str());
        parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 3);//filename
        tempMesh.filename = parameter;
        parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 4);//scale x
        tempMesh.sx = atof(parameter.c_str());
        parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 5);//scale y
        tempMesh.sy = atof(parameter.c_str());
        parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 6);//scale z
        tempMesh.sz = atof(parameter.c_str());
        parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 7);//description
        tempMesh.description = parameter;
        m_assetMeshData.push_back(tempMesh);
        pLog->Log("Mesh found....");
      }
    }
  }

  //log results of load
  pLog->Log("Number of graphic files",  m_assetFileData.size());
  pLog->Log("Number of sprites",        m_assetSpriteData.size());
  pLog->Log("Number of meshes",         m_assetMeshData.size());

  //load graphics files
  for(int i = 0; i < m_assetFileData.size(); ++i){
    pLog->Log("graphic file loaded", m_assetFileData[i].filename);
    LoadGraphicFile(m_assetFileData[i].fileID, "assets//graphics//" + m_assetFileData[i].filename);
  }

  //load sprite information
  GRAPHIC_IMAGE gi;
  for(int i = 0; i < m_assetSpriteData.size(); ++i){
    gi.spriteID = m_assetSpriteData[i].spriteID;
    gi.fileID = m_assetSpriteData[i].fileID;
    gi.sourceX = m_assetSpriteData[i].sourceX;
    gi.sourceY = m_assetSpriteData[i].sourceY;
    gi.width = m_assetSpriteData[i].width;
    gi.height = m_assetSpriteData[i].height;
    //gi.layer = m_assetSpriteData[i].layer;
    //gi.radius = m_assetSpriteData[i].radius;
    gi.scale = m_assetSpriteData[i].scale;    
    gi.alpha = m_assetSpriteData[i].alpha;
    gi.angle = m_assetSpriteData[i].angle;
    gi.maxFrames = m_assetSpriteData[i].maxFrames;
    gi.updateInterval = m_assetSpriteData[i].updateInterval;
    gi.description = m_assetSpriteData[i].description;
    
    //ensure the fileID really exists
    gi.bValid = false; //assume file is invalid
    for(int j = 0; j < m_assetFileData.size(); ++j){
      if(gi.fileID == m_assetFileData[j].fileID){
        gi.bValid = true;
        m_sprites.push_back(gi);
        //pLog->Log("-------Sprite", gi.spriteID,gi.updateInterval, gi.maxFrames);
        break;
      }
    }
  }

  //load 3D meshes
  for(int i = 0; i < m_assetMeshData.size(); ++i){
   /* pLog->Log("****** 3D mesh added ******");
    pLog->Log("meshID", m_assetMeshData[i].meshID);
    pLog->Log("scaleX", m_assetMeshData[i].sx);
    pLog->Log("scaleY", m_assetMeshData[i].sy);
    pLog->Log("scaleZ", m_assetMeshData[i].sz);
    pLog->Log("description", m_assetMeshData[i].description);*/
    LoadMesh(m_assetMeshData[i].meshID,
             m_assetMeshData[i].filename, 
             m_assetMeshData[i].sx,
             m_assetMeshData[i].sy,
             m_assetMeshData[i].sz);
  }

  return true;
}

/****************************************************
 Loads texture files and skins
****************************************************/
void CGraphics::LoadGraphicFile(int fileID, std::string filename){
  S_TEXTURE temp;

  CLog *pLog = CLog::Instance();
  HRESULT hr = D3DXCreateTextureFromFile(m_d3ddev,
                                         filename.c_str(),
                                         &temp.pBuffer);

  if(FAILED(hr)){
    pLog->Log("Failed to load graphic file", filename);
    return;
  }
  pLog->Log("Success....................", filename);
  temp.id = fileID;
  m_texture.push_back(temp);
}

/****************************************************
 Returns pointer to texture graphic...using file ID
****************************************************/
IDirect3DTexture9* CGraphics::GetTexture(int id){
  
  for (int i = 0; i < m_texture.size(); ++i){
    if(m_texture[i].id == id){
      return m_texture[i].pBuffer;
    }
  }
  return NULL;
}

/****************************************************
 Returns sprite data based upon sprite ID
****************************************************/
GRAPHIC_IMAGE CGraphics::GetSprite(int id){
  for(int i = 0; i < m_sprites.size(); ++i){
    if(m_sprites[i].spriteID == id){
      return m_sprites[i];
    }
  }
  GRAPHIC_IMAGE gi;
  gi.bValid = false;
  return gi;
}

GRAPHIC_IMAGE CGraphics::GetSprite2(int i){
  GRAPHIC_IMAGE gi;
  gi.bValid = false;
  if(i < 0 || i > m_sprites.size() - 1)
    return gi;

  return m_sprites[i];
}

int CGraphics::GetSpriteCount(){
 return m_sprites.size();
}

//-----------------------------------------------------------------------------
// Name : FindDepthStencilFormat ()		(private)
// Desc : This function simply determines the best depth format that is
//        available for the specified mode.
// Note : No tests for stencil active depth buffers are made.
//-----------------------------------------------------------------------------
D3DFORMAT CGraphics::FindDepthStencilFormat( ULONG AdapterOrdinal, D3DDISPLAYMODE Mode, D3DDEVTYPE DevType )
{

    // Test for 24 bith depth buffer
    if (SUCCEEDED( m_d3d->CheckDeviceFormat(AdapterOrdinal, DevType, Mode.Format, D3DUSAGE_DEPTHSTENCIL , D3DRTYPE_SURFACE , D3DFMT_D32 )))
    {
        if (SUCCEEDED( m_d3d->CheckDepthStencilMatch ( AdapterOrdinal, DevType, Mode.Format, Mode.Format, D3DFMT_D32 ))) return D3DFMT_D32;
    
    } // End if 32bpp Available

    // Test for 24 bit depth buffer
    if (SUCCEEDED( m_d3d->CheckDeviceFormat(AdapterOrdinal, DevType, Mode.Format, D3DUSAGE_DEPTHSTENCIL , D3DRTYPE_SURFACE , D3DFMT_D24X8 )))
    {
        if (SUCCEEDED( m_d3d->CheckDepthStencilMatch ( AdapterOrdinal, DevType, Mode.Format, Mode.Format, D3DFMT_D24X8 ))) return D3DFMT_D24X8;
    
    } // End if 24bpp Available

    // Test for 16 bit depth buffer
    if (SUCCEEDED( m_d3d->CheckDeviceFormat(AdapterOrdinal, DevType, Mode.Format, D3DUSAGE_DEPTHSTENCIL , D3DRTYPE_SURFACE , D3DFMT_D16 )))
    {
        if (SUCCEEDED( m_d3d->CheckDepthStencilMatch ( AdapterOrdinal, DevType, Mode.Format, Mode.Format, D3DFMT_D16 ))) return D3DFMT_D16;
    
    } // End if 16bpp Available

    // No depth buffer supported
    return D3DFMT_UNKNOWN;

}

void CGraphics::DrawLine(float x1, float y1,float x2, float y2, int red, int green, int blue){

  const int NUMPOINTS = 2;
  struct line_vertex{
      float x, y, z, rhw;
      DWORD color;       
  };
  line_vertex line[NUMPOINTS + 1];
  const DWORD line_fvf = D3DFVF_XYZRHW|D3DFVF_DIFFUSE;  

  line[0].x = x1-1 ;
  line[0].y = y1 ;
  line[0].z = 0;
  line[0].rhw = 1.0f;
  line[0].color = D3DCOLOR_XRGB(red, green, blue);
  
  line[1].x = x2-1;
  line[1].y = y2;
  line[1].z = 0;
  line[1].rhw = 1.0f;
  line[1].color = D3DCOLOR_XRGB(red, green, blue);


  m_d3ddev->SetFVF(line_fvf);
  m_d3ddev->SetTexture(0, NULL);
  m_d3ddev->DrawPrimitiveUP(D3DPT_LINESTRIP, NUMPOINTS, &line[0],sizeof(line[0]));

}


void CGraphics::DrawRect(float x1, float y1,float x2, float y2, int red, int green, int blue){

  const int NUMPOINTS = 5;
  struct line_vertex{
      float x, y, z, rhw;
      DWORD color;       
  };
  line_vertex line[NUMPOINTS + 1];
  const DWORD line_fvf = D3DFVF_XYZRHW|D3DFVF_DIFFUSE;  

  line[0].x = x1;
  line[0].y = y1;
  line[0].z = 0;
  line[0].rhw = 1.0f;
  line[0].color = D3DCOLOR_XRGB(red, green, blue);
  
  line[1].x = x2;
  line[1].y = y1;
  line[1].z = 0;
  line[1].rhw = 1.0f;
  line[1].color = D3DCOLOR_XRGB(red, green, blue);

  line[2].x = x2;
  line[2].y = y2;
  line[2].z = 0;
  line[2].rhw = 1.0f;
  line[2].color = D3DCOLOR_XRGB(red, green, blue);
  
  line[3].x = x1;
  line[3].y = y2;
  line[3].z = 0;
  line[3].rhw = 1.0f;
  line[3].color = D3DCOLOR_XRGB(red, green, blue);

  line[4].x = x1;
  line[4].y = y1;
  line[4].z = 0;
  line[4].rhw = 1.0f;
  line[4].color = D3DCOLOR_XRGB(red, green, blue);

  m_d3ddev->SetFVF(line_fvf);
  m_d3ddev->SetTexture(0, NULL);
  m_d3ddev->DrawPrimitiveUP(D3DPT_LINESTRIP, NUMPOINTS, &line[0],sizeof(line[0]));

}

//draws circle
void CGraphics::DrawCircle(float x, float y, int red, int green, int blue, float radius){

  const int NUMPOINTS = 24;//24
  const float PI = 3.14159;
  struct line_vertex{
      float x, y, z, rhw;
      DWORD color;       
  };
  line_vertex circle[NUMPOINTS + 1];
  const DWORD line_fvf=D3DFVF_XYZRHW|D3DFVF_DIFFUSE;  
  float theta;
  float wedgeAngle;
  wedgeAngle = (float)((2 * PI)/NUMPOINTS);
  for (int i = 0; i <= NUMPOINTS; i++){
    theta = i * wedgeAngle;
    circle[i].x = (float)(x + radius * cos(theta));
    circle[i].y = (float)(y - radius * sin(theta));
    circle[i].z = 0;
    circle[i].rhw = 1.0f;
    circle[i].color = D3DCOLOR_XRGB(red, green, blue);
  }

  m_d3ddev->SetFVF(line_fvf);
  m_d3ddev->SetTexture(0, NULL);
  m_d3ddev->DrawPrimitiveUP(D3DPT_LINESTRIP, NUMPOINTS, &circle[0],sizeof(circle[0]));
  
}