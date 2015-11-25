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

  s::map<stdstr, int> scores;
  bool countDownState;
  int countDownValue;

  c::Label* countDown;
  c::Label* result;
  c::Label* status;
  c::Label* title;
  c::Label* score1;
  c::Label* score2;
  c::Color3B  color;

  stdstr play2;
  stdstr play1;
  stdstr p2Long;
  stdstr p1Long;
  stdstr p2ID;
  stdstr p1ID;

public:

  virtual int getIID() { return 3; }
  virtual f::XLayer* realize();

    void regoPlayers(const stdstr& color1,
                     const stdstr& p1k, const stdstr& p1n,
                     const stdstr& color2,
                     const stdstr& p2k, const stdstr& p2n);

  void resetAsNew();
  void reset();
    void showMenu(c::Ref*) {}

    void initLabels();
    void initIcons();
    void initScores();

    void showTimer();
    void updateTimer(float);
    void showCountDown(const stdstr& msg);
    void killTimer();
    void updateScore(const stdstr& pcolor,  int value);
    void draw(bool running,  int pnum);
    void endGame(int winner);
    void drawXXXText(c::Label*, const stdstr& msg);
    void drawScores();
    void drawResult(int pnum);
    void drawStatus(int pnum);


  NO__CPYASS(HUDLayer)
  DECL_CTOR(HUDLayer)

};


NS_END(tttoe)
#endif

