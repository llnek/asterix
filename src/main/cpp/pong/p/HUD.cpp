// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Ken Leung. All rights reserved.

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "HUD.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(pong)

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::regoPlayers(const Player &p1, const Player &p2) {

  auto tile= CC_CSV(c::Float, "TILE");
  auto wb= cx::visBox();
  float tw2;

  _title->setString(p1.pid + " / " + p2.pid);
  tw2= cx::getScaledWidth(_title) * 0.5;
  _parr[2]= p2;
  _parr[1]= p1;

  _score1->setPosition(wb.cx - tw2 - cx::getScaledWidth(_score1) * 0.5 - 10,
      wb.top - tile * 6 /2 - 2);
  _score2->setPosition(wb.cx + tw2 + cx::getScaledWidth(_score2) * 0.5 + 6,
      wb.top - tile * 6 /2 - 2);

}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::endGame() {
  CC_SHOW(_resultMsg);
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::decoUI() {
  auto tile = CC_CSV(c::Float, "TILE");
  auto wb= cx::visBox();

  _title= cx::reifyBmfLabel("TinyBoxBB");
  _title->setScale(XCFG()->getScale() * 0.3);
  _title->setPosition(wb.cx, wb.top - tile * 6 /2 );
  addItem(_title);

  _score1= cx::reifyBmfLabel("OCR", "8");
  _score1->setScale(XCFG()->getScale() * 0.25);
  addItem(_score1);

  _score2= cx::reifyBmfLabel("OCR", "8");
  _score2->setScale(XCFG()->getScale() * 0.25);
  addItem(_score2);

  _resultMsg = cx::reifyBmfLabel("CoffeeBuzzed");
  CC_HIDE(_resultMsg);
  _resultMsg->setPosition(wb.cx, 100);
  _resultMsg->setScale(XCFG()->getScale() * 0.15);
  addItem(_resultMsg);

  _scores.fill(0);
  drawScores();
}

//////////////////////////////////////////////////////////////////////////////
//
int HUDLayer::isDone() {
  auto cfg = MGMS()->getLCfg()->getValue();
  auto pts = JS_INT(cfg["NUM+POINTS"]);
  auto s2= _scores[2];
  auto s1= _scores[1];
  int rc= -1;

  if (s2 >= pts) { rc = 2; }
  if (s1 >= pts) { rc = 1; }
  return rc;
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::updateScores(j::json scs) {
  auto &p2= _parr[2];
  auto &p1= _parr[1];
  _scores[2] = JS_INT(scs[p2.color]);
  _scores[1] = JS_INT(scs[p1.color]);
  drawScores();
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::updateScore(const sstr &color, int pnum, int value) {
  assert(pnum > 0 && pnum < _scores.size());
  _scores[pnum] += value;
  drawScores();
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::drawScores() {
  auto s2 = _scores[2];
  auto s1 = _scores[1];
  _score1->setString(s::to_string(s1));
  _score2->setString(s::to_string(s2));
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::drawResult(int winner) {
  auto &p2= _parr[2];
  auto &p1= _parr[1];
  sstr msg;
  sstr pn;
  if (winner == 2) { pn= p2.pname; }
  if (winner == 1) { pn= p1.pname; }
  if (pn.length() > 0) {
    msg= gets("whowin", s_vec<sstr> { pn });
  }
  _resultMsg->setString(msg);
}



NS_END


