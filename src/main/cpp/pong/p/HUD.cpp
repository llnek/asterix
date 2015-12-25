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

#include "HUD.h"

NS_BEGIN(pong)

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::regoPlayers(int p1,
    const sstr &p1k, const sstr &p1n,
    int p2,
    const sstr &p2k, const sstr &p2n) {

  auto tile= CC_CSV(c::Integer, "TILE");
  auto cw= cx::center();
  auto wb= cx::visBox();

  play2= p2;
  play1= p1;
  p2Long= p2n;
  p1Long= p1n;
  p2ID= p2k;
  p1ID= p1k;

  title->setString(p1ID + " / " + p2ID);

  score1->setPosition( cw.x - cx::getScaledWidth(title)/2 -
                           cx::getScaledWidth(score1)/2 - 10,
                           wb.top - tile * 6 /2 - 2);
  score2->setPosition( cw.x + cx::getScaledWidth(title)/2 +
                           cx::getScaledWidth(score2)/2 + 6,
                           wb.top - tile * 6 /2 - 2);
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::resetAsNew() {
  reset();
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::reset() {
  scores.fill(0);
  //scores[csts.P2_COLOR] = 0;
  //scores[csts.P1_COLOR] = 0;

  replayBtn->setVisible(false);
  resultMsg->setVisible(false);
  drawScores();
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::endGame() {
  replayBtn->setVisible(true);
  resultMsg->setVisible(true);
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::decorate() {
  auto tile = CC_CSV(c::Integer, "TILE");
  auto cw= cx::center();
  auto wb= cx::visBox();

  title= cx::reifyBmfLabel("font.TinyBoxBB");
  title->setScale(XCFG()->getScale() * 0.3f);
  title->setPosition(cw.x, wb.top - tile * 6 /2 );
  addItem(title);

  score1= cx::reifyBmfLabel("font.OCR", "8");
  score1->setScale(XCFG()->getScale() * 0.25f);
  addItem(score1);

  score2= cx::reifyBmfLabel("font.OCR", "8");
  score2->setScale(XCFG()->getScale() * 0.25f);
  addItem(score2);

  resultMsg = cx::reifyBmfLabel("font.CoffeeBuzzed");
  resultMsg->setVisible(false);
  resultMsg->setPosition(cw.x, 100);
  resultMsg->setScale(XCFG()->getScale() * 0.15f);
  addItem(resultMsg);
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::isDone() {
  auto pts = CC_CSV(c::Integer, "NUM_POINTS");
  auto s2= scores[play2];
  auto s1= scores[play1];
  rc= [false, null];

  if (s2 >= pts) { rc = [ true, this.play2]; }
  if (s1 >= pts) { rc = [ true, this.play1]; }
  return rc;
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::updateScores(j::json scs) {
  scores[play2] = JS_INT(scs[play2]);
  scores[play1] = JS_INT(scs[play1]);
  drawScores();
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::updateScore(const sstr &color, int value) {
  scores[color] += value;
  drawScores();
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::drawScores() {
  auto s2 = scores[play2];
  auto s1 = scores[play1];
  score1->setString(s::to_string(s1));
  score2->setString(s::to_string(s2));
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::drawResult(const sstr &winner) {
  sstr msg;
  if (winner == CC_CSS("P2_COLOR")) {
    msg= XCFG()->getL1oNStr("whowin", s_vec<sstr> { p2n });
  } else {
    msg= XCFG()->getL10NStr("whowin", s_vec<sstr> { p1n });
  }
  resultMsg->setString(msg);
}



NS_END(pong)


