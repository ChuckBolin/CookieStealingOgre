#ifndef GAME_STATE_PLAY4_H
#define GAME_STATE_PLAY4_H

#include "cgamestateobject.h"
#include "CGraphics.h"
#include "CAudioManager.h"
#include "CTimer.h"
#include <sstream>
#include <string>
#include "keystatus.h"
#include "..\resource.h"
#include "CGameData.h"
#include "CLog.h"
#include "camera.h"

class cGameStatePlay4 :  public cGameStateObject{
public:
  cGameStatePlay4(void);
  ~cGameStatePlay4(void);
  void initialize();
  void activate(CGameData &data);
  cGameStateObject* update(double timeDifference, Camera &cam, CGameData &data, CGraphics &con);
  void render(CGraphics &con, Camera &cam, CGameData &data);
  void processEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
  void deactivate();
  void resume();
  void pause();
  void save();
  void addTransitionEvent(int event, cGameStateObject* p_Next);
  bool m_bInventory;
  bool m_bPause;
  bool m_bHelp;

private:
  std::vector<TRANSITION_EVENT> m_TE; //stores all transition events
  CTimer m_timer;
  bool m_bDone;
  int m_event;
  CMouse m_mouse;
  int m_viewChoice;
  int m_currentSprite;

  //test
  float m_badgeAngleX;
  float m_badgeAngleZ;
  int m_mapX;
  int m_mapY;
  int m_inventoryX;
  int m_inventoryY;
  int m_statusX;
  int m_statusY;
  CTimer m_secondTimer;
  
};

#endif

