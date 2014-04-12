#ifndef GAME_STATE_EDITOR_H
#define GAME_STATE_EDITOR_H

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

class cGameStateEditor :  public cGameStateObject{
public:
  cGameStateEditor(void);
  ~cGameStateEditor(void);
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

private:
  std::vector<TRANSITION_EVENT> m_TE; //stores all transition events
  CTimer m_timer;
  bool m_bDone;
  int m_event;
  CMouse m_mouse;
  int m_viewChoice;
  int m_currentSprite;
  int m_editLevel;

  //editor information
  //top-left corner of working area
  int m_workViewX;
  int m_workViewY;
  int m_gridSpace; //32, 64, 128, 256, 512
  int m_lastGridSpace; //stores last space value before turning off snap
  int m_currentObjectID;
  bool m_gridOn;
  bool m_snapOn;
  int m_panelX;
  int m_panelY;
  int m_editorX;
  int m_editorY;
  float m_angle;
  int m_fileBarX;
  int m_fileBarY;
  bool m_bSave;
  bool m_bClear;
  bool m_bOpen;
  bool m_bUndo;
  int m_mapX;
  int m_mapY;
  bool m_bFillGround;

};

#endif
