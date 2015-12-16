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

  bool countDownState=false;
  int countDownValue=0;

  s::map<sstr, int> scores;

  c::Label *countDown= nullptr;
  c::Label *result=nullptr;
  c::Label *status=nullptr;
  c::Label *title=nullptr;
  c::Label *score1=nullptr;
  c::Label *score2=nullptr;
  c::Color3B  color;

  sstr play2;
  sstr play1;
  sstr p2Long;
  sstr p1Long;
  sstr p2ID;
  sstr p1ID;

public:

  virtual int getIID() { return 3; }
  virtual void decorate();

  void regoPlayers(const sstr &color1,
                   const sstr &p1k, const sstr &p1n,
                   const sstr &color2,
                   const sstr &p2k, const sstr &p2n);

  void showMenu(c::Ref*);
  void resetAsNew();
  void reset();

  void updateScore(const sstr& pcolor,  int value);
  void drawXXXText(c::Label*, const sstr&);
  void draw(bool running,  int pnum);
  void showCountDown(const sstr&);
  void drawResult(int pnum);
  void drawStatus(int pnum);
  void endGame(int winner);
  void updateTimer(float);
  void drawScores();
  void showTimer();
  void killTimer();

  virtual ~HUDLayer() {}
  HUDLayer() {}

  NOCPYASS(HUDLayer)
};


NS_END(tttoe)
#endif


