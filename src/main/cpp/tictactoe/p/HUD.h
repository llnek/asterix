// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2015, Ken Leung. All rights reserved.

#if !defined(__HUD_H__)
#define __HUD_H__

#include "x2d/XLayer.h"
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////
//
class CC_DLL HUDLayer : public f::XLayer {
protected:

  void ShowMenu(c::Ref*);
  void OnReplay(c::Ref*);

  void InitLabels();
  void InitIcons();

  void ShowTimer();
  void UpdateTimer(float);
  void ShowCountDown(const stdstr& msg);
  void KillTimer();
  void UpdateScore(int pcolor,  int value);
  void Draw(bool running,  int pnum);
  void EndGame(int winner);
  void DrawStatusText(obj, const stdstr& msg);
  void DrawScores();
  void DrawResult(int pnum);
  void DrawStatus(int pnum);
  void RegoPlayers(color1,p1ids,color2,p2ids);

  c::Color3B  color;
  stdstr p2Long;
  stdstr p1Long;
  stdstr p2ID;
  stdstr p1ID;

public:

  virtual int GetIID() { return 3; }
  virtual f::XLayer* Realize();

  void ResetAsNew();
  void Reset();

  NO__CPYASS(HUDLayer)
  IMPL_CTOR(HUDLayer)

};


NS_END(tttoe)
#endif

