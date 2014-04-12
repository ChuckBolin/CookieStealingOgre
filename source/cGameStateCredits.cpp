#include "..\include\cgamestatecredits.h"

cGameStateCredits::cGameStateCredits(void){
}

cGameStateCredits::~cGameStateCredits(void)
{
	m_TE.clear();
}

void cGameStateCredits::initialize(){

  SCROLL_MSG msg;
  msg.red = 0; msg.green = 255; msg.blue = 0;
  msg.msg ="Game Institute Student Challenge 9" ; m_msg.push_back(msg);
  msg.msg ="\"http://www.gameinstitute.com\"" ; m_msg.push_back(msg);
  msg.msg =""; m_msg.push_back(msg);
  msg.red = 255; msg.green = 255; msg.blue = 0;
  msg.msg = "\"Cindy and The Cookie Stealing Ogre\"";   m_msg.push_back(msg);
  msg.msg =""; m_msg.push_back(msg);
  msg.red = 0; msg.green = 255; msg.blue = 0;
  msg.msg = "October 18th - December 12th, 2008"; m_msg.push_back(msg);
  msg.red = 255; msg.green = 0; msg.blue = 0;
  msg.msg ="Created by Team Charleston" ; m_msg.push_back(msg);
  msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="2D Lead Artist and Level Designer - Chuck Bolin" ; m_msg.push_back(msg);
  msg.msg ="2D Concept Artist - Greg Rexroad" ; m_msg.push_back(msg);
  msg.msg ="Lead Programmer and Game Designer - Chuck Bolin" ; m_msg.push_back(msg);
  msg.msg ="3D Concept Artist - Xenthal" ; m_msg.push_back(msg);
  msg.msg ="Musician  - See Readme.txt file for credits" ; m_msg.push_back(msg);
  msg.msg ="Testing and QA - Sam Bolin" ; m_msg.push_back(msg);
  msg.red = 255; msg.green = 255; msg.blue = 255;  //white
  msg.msg ="Special thanks to my wife Grace for"; m_msg.push_back(msg);
  msg.msg ="allowing me \'family time\' to work "; m_msg.push_back(msg);
  msg.msg = "on this project." ; m_msg.push_back(msg);
  msg.msg ="" ; m_msg.push_back(msg);

  msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="" ; m_msg.push_back(msg);

  m_timer.initialize(); 
  m_scrollValue = 0;

  //used to show several images in the background
  m_secondTimer.initialize();
  m_secondCount = 0;

  //used for fading
  m_red = 255;//255;
  m_green =255;//255;
  m_blue = 255;//255;
  m_fadeTimer.initialize();
  
}

void cGameStateCredits::activate(CGameData &data){

}

cGameStateObject* cGameStateCredits::update(double timeDifference, Camera &cam, CGameData &data, CGraphics &con){
  m_event = EVENT_GO_NO_WHERE;
  static double timeDiffTotal = 0;

  CAudioManager *pAudio = CAudioManager::Instance();
  m_mouse.SetHandle(data.m_hWnd);
  //pAudio->KillAll();
  if(::GetActiveWindow() == data.m_hWnd && keyDown(VK_ESCAPE))
    m_event = EVENT_GO_QUIT;

  //terminate program after scrolling greater than 15 seconds
  if(m_secondCount > 20)
    m_event = EVENT_GO_QUIT;

  //update counter for displaying images
  if(m_secondTimer.secondTimer() == true){
    m_secondCount++;
  }

  //tracks time
  timeDiffTotal += timeDifference;
  if(timeDiffTotal > 0.025){
    timeDiffTotal = 0;
    m_scrollValue += 2;  
  }

  for(unsigned int i=0; i< m_TE.size(); i++){
    if (m_TE[i].event == m_event){
      return m_TE[i].p_gso;
    }
  }
  return 0;
}

void cGameStateCredits::render(CGraphics &con, Camera &cam, CGameData &data){
 
::ShowCursor(false);
  CLog *pLog = CLog::Instance();
  static bLostDevice = false;

  HRESULT hr;

  if(FAILED(hr = con.m_d3ddev->TestCooperativeLevel())){

    pLog->Log("Failed test");
    if(hr == D3DERR_DEVICELOST)
      return;
    pLog->Log("No lost");
    if( hr == D3DERR_DEVICENOTRESET ){
      pLog->Log("Time to reset");
      con.DeleteLost();
      pLog->Log("Deleted everything");
      hr = con.m_d3ddev->Reset(&con.GetDPP());
      pLog->Log("Dev reset");
      con.ReloadLost();
      pLog->Log("ReloadLost");
      bLostDevice = false;
      return;
    }
  }

  D3DXMATRIX V;
  cam.getViewMatrix(&V, 0);
  con.m_d3ddev->SetTransform(D3DTS_VIEW, &V);

  // clear the window to a deep blue
  con.m_d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
  con.m_d3ddev->BeginScene();    // begins the 3D scene

  //render credits
  GRAPHIC_IMAGE gi;
  gi = con.GetSprite(2);
  con.RenderGraphicModulate(gi, data.m_screenWidth/2 - gi.width/2, 255, 255, 255, 255);

  //scrolls message
  int posY = 0;
  
  for(int i = 0; i < m_msg.size(); i++){
    posY = m_scrollValue + ((int)m_msg.size() - i * 20);
    if(posY < con.GetScreenHeight() + 20){
      con.Print(m_msg[i].msg, F_V20, 20, con.GetScreenHeight() - posY, m_msg[i].red, m_msg[i].green, m_msg[i].blue,255);
    }
  }

  con.Print("Press ESC to quit", F_V16, data.m_screenWidth/2 - 100, data.m_screenHeight - 60, 255, 255, 255, 255);

  if(data.m_displayDebugInfo == true){
    con.Print("Version", data.m_version, F_V16, 10, 10,  255, 255, 255, 255);
    con.Print("FPS", data.m_FPS , F_V16, 10, 30,  255, 255, 255, 255);
  }

  con.m_d3ddev->EndScene();    // ends the 3D scene
  hr = con.m_d3ddev->Present(NULL, NULL, NULL, NULL);   // displays the created frame on the screen

  if(FAILED(hr = con.m_d3ddev->TestCooperativeLevel())){
    if(hr == D3DERR_DEVICELOST)
      bLostDevice = true;
  }

  return;
}

void cGameStateCredits::processEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
}

void cGameStateCredits::deactivate(){
//  CAudioManager *pAudio = CAudioManager::Instance();
//  pAudio->KillAll();

}
void cGameStateCredits::resume(){
}
void cGameStateCredits::pause(){
}
void cGameStateCredits::save(){
}

void cGameStateCredits::addTransitionEvent(int event, cGameStateObject* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}
