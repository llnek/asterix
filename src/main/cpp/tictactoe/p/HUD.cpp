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

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "x2d/XLib.h"
#include "HUD.h"
NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////
//
void HUDLayer::decorate() {

  auto color = XCFG()->getColor("default");
  auto soff= CC_CSV(c::Integer, "S_OFF");
  auto tile= CC_CSV(c::Integer, "TILE");
  auto scale= XCFG()->getScale();
  auto cw= cx::center();
  auto wb= cx::visBox();

  regoAtlas("game-pics");
  incIndexZ();

  //score1
  score1= cx::reifyBmfLabel("font.SmallTypeWriting", "0");
  score1->setColor(XCFG()->getColor("text"));
  score1->setAnchorPoint(cx::anchorTL());
  score1->setScale(scale);
  score1->setPosition(tile+soff+2, wb.top-tile-soff);
  addItem(score1);

  //score2
  score2= cx::reifyBmfLabel( "font.SmallTypeWriting", "0");
  score2->setColor(XCFG()->getColor("text"));
  score2->setAnchorPoint(cx::anchorTR());
  score2->setScale(scale);
  score2->setPosition(wb.right-tile-soff, wb.top-tile-soff);
  addItem(score2);

  // status
  status= cx::reifyBmfLabel( "font.CoffeeBuzzed");
  status->setColor(XCFG()->getColor("text"));
  status->setScale(scale * 0.3f);
  status->setPosition(cw.x, wb.bottom + tile * 10);
  addItem(status);

  // result
  result= cx::reifyBmfLabel( "font.CoffeeBuzzed");
  result->setColor(XCFG()->getColor("text"));
  result->setScale(scale * 0.3f);
  result->setPosition(cw.x, wb.bottom + tile * 10);
  result->setVisible(false);
  addItem(result);

  //title
  title = cx::reifyBmfLabel( "font.JellyBelly");
  title->setScale(scale * 0.6f);
  title->setAnchorPoint(cx::anchorT());
  title->setColor(color);
  title->setPosition(cw.x, wb.top - 2*tile);
  addItem(title);

  auto b = cx::reifyMenuBtn("icon_menu.png");
  auto hh = cx::getHeight(b) * 0.5f;
  auto hw = cx::getWidth(b) * 0.5f;
  b->setTarget(this,
      CC_MENU_SELECTOR(HUDLayer::showMenu));
  b->setColor(color);
  auto menu = cx::mkMenu(b);

  menu->setPosition(wb.right-tile-hw, wb.bottom+tile+hh);
  addItem(menu);
}

//////////////////////////////////////////////////////////////////////////
//
void HUDLayer::showMenu(c::Ref*) {
  SENDMSG("/hud/showmenu");
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::showTimer() {
  auto ptt= CC_CSV(c::Integer, "PLAYER_THINK_TIME");
  auto tile= CC_CSV(c::Integer,"TILE");
  auto scale= XCFG()->getScale();
  auto cw= cx::center();
  auto wb= cx::visBox();

  // timer is already showing, go away
  if (countDownState) {
    return;
  }

  if (ENP(countDown)) {
    countDown= cx::reifyBmfLabel( "font.AutoMission");
    countDown->setPosition(cw.x, wb.top - 10*tile);
    countDown->setAnchorPoint(cx::anchorC());
    countDown->setScale(scale * 0.5f);
    countDown->setColor(XCFG()->getColor("text"));
    addItem(countDown);
  }

  countDownState= true;
  countDownValue= ptt;

  showCountDown(s::to_string(ptt));

  schedule(CC_SCHEDULE_SELECTOR(HUDLayer::updateTimer), 1.0f);
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::updateTimer(float dt) {
  if (!countDownState) { return; } else {
    countDownValue -= 1;
  }
  if (countDownValue < 0) {
    killTimer();
    SENDMSG("/player/timer/expired");
  } else {
    showCountDown(s::to_string(countDownValue));
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::showCountDown(const sstr &msg) {
  if (NNP(countDown)) {
    countDown->setString(msg);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::killTimer() {
  if (countDownState) {
    unschedule(CC_SCHEDULE_SELECTOR(HUDLayer::updateTimer));
    showCountDown(" ");
  }
  countDownValue=0;
  countDownState=false;
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::updateScore(int pnum, int value) {
  assert(pnum >=0 && pnum < scores.size());
  scores[pnum] += value;
  drawScores();
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::draw(bool running, int pnum) {
  if (running) {
    drawStatus(pnum);
  } else {
    drawResult(pnum);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::endGame(int winner) {
  status->setVisible(false);
  result->setVisible(true);
  killTimer();
  drawResult(winner);
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::drawXXXText(c::Label *obj, const sstr &msg) {
  obj->setString(msg);
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::drawScores() {
  auto s2 = scores[2];
  auto s1 = scores[1];
  auto n2 = s::to_string(s2);
  auto n1 = s::to_string(s1);

  score1->setString(n1);
  score2->setString(n2);
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::drawResult(int pnum) {
  auto msg = XCFG()->getL10NStr("whodraw");

  switch (pnum) {
    case 2:
      msg= XCFG()->getL10NStr("whowin", s_vec<sstr> { this->p2Long});
      break;
    case 1:
      msg= XCFG()->getL10NStr("whowin", s_vec<sstr> { this->p1Long});
      break;
  }

  drawXXXText(result, msg);
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::drawStatus(int pnum) {
  if (pnum > 0) {
    auto pfx = pnum == 1 ? p1Long : p2Long;
    drawXXXText(status,
      XCFG()->getL10NStr("whosturn", s_vec<sstr> { pfx }));
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::regoPlayers(const sstr &color1,
    const sstr &p1k, const sstr &p1n,
    const sstr &color2, const sstr &p2k, const sstr &p2n) {

  title->setString(p1k + " / " + p2k);
  scores= {0,0,0};
  play2= color2;
  play1= color1;
  p2Long= p2n;
  p1Long= p1n;
  p2ID= p2k;
  p1ID= p1k;
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::resetAsNew() {
  reset();
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::reset() {
  result->setVisible(false);
  status->setVisible(true);
  scores = {0,0,0};
}


NS_END(tttoe)


