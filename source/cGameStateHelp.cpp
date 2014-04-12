#include "..\include\cGameStateHelp.h"

cGameStateHelp::cGameStateHelp(){
}

cGameStateHelp::~cGameStateHelp()
{
	m_TE.clear();
}

void cGameStateHelp::initialize(){

}

void cGameStateHelp::activate(CGameData &data){
}

cGameStateObject* cGameStateHelp::update(double timeDifference, Camera &cam, CGameData &data, CGraphics &con){
  m_event = EVENT_GO_NO_WHERE;

  //set aircraft camera to 1000 feet
  static bool bFirstTime = true;
  if(bFirstTime == true){
    bFirstTime = false;
  }

  //CAudioManager *pAudio = CAudioManager::Instance();
  m_mouse.SetHandle(data.m_hWnd);

  if(::GetActiveWindow() == data.m_hWnd && keyDown(VK_ESCAPE))
    m_event = EVENT_GO_MAIN;

  for(unsigned int i=0; i< m_TE.size(); i++){
    if (m_TE[i].event == m_event){
      return m_TE[i].p_gso;
    }
  }
  return 0;
}

void cGameStateHelp::render(CGraphics &con, Camera &cam, CGameData &data){
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
  GRAPHIC_IMAGE gi;
  // clear the window to a deep blue
  con.m_d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(160, 0, 0), 1.0f, 0);
  con.m_d3ddev->BeginScene();    // begins the 3D scene

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
      
      con.Print("Press ESC to continue...", F_V20, 410, 740, 255, 0, 0, 255);
  
  con.m_d3ddev->EndScene();    // ends the 3D scene
  hr = con.m_d3ddev->Present(NULL, NULL, NULL, NULL);   // displays the created frame on the screen

  if(FAILED(hr = con.m_d3ddev->TestCooperativeLevel())){
    if(hr == D3DERR_DEVICELOST)
      bLostDevice = true;
  }

  return;
}

void cGameStateHelp::processEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
}

void cGameStateHelp::deactivate(){
//  CAudioManager *pAudio = CAudioManager::Instance();
//  pAudio->KillAll();

}
void cGameStateHelp::resume(){
}
void cGameStateHelp::pause(){
}
void cGameStateHelp::save(){
}

void cGameStateHelp::addTransitionEvent(int event, cGameStateObject* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}
