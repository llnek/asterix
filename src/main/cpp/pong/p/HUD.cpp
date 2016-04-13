// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Kenneth Leung. All rights reserved.

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "HUD.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(pong)

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::regoPlayers(const Player &p1, const Player &p2) {

  _title->setString(p1.pid + " / " + p2.pid);
  _title->setAnchorPoint(cx::anchorT());
  XCFG()->scaleNode(_title, 52);

  auto sz= CC_CSIZE(_title);
  auto gap= sz.height/2;
  auto wb= cx::visBox();

  _parr[2]= p2;
  _parr[1]= p1;

  _score2->setAnchorPoint(cx::anchorR());
  _score1->setAnchorPoint(cx::anchorL());

  CC_POS2(_score2, wb.right - gap, wb.top-gap);
  CC_POS2(_score1, wb.left + gap, wb.top-gap);
  CC_POS2(_title, wb.cx, wb.top - gap);
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::endGame() {
  CC_SHOW(_resultMsg);
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::decoUI() {

  auto wb= cx::visBox();

  _title= cx::reifyBmfLabel("title");
  addItem(_title);

  _score1= cx::reifyBmfLabel("dft", "8");
  XCFG()->scaleNode(_score1, 24);
  addItem(_score1);

  _score2= cx::reifyBmfLabel("dft", "8");
  XCFG()->scaleNode(_score2, 24);
  addItem(_score2);

  _resultMsg = cx::reifyBmfLabel("dft","?");
  XCFG()->scaleNode(_resultMsg, 32);
  CC_HIDE(_resultMsg);
  CC_POS2(_resultMsg, wb.cx, 100);
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
  _score1->setString(FTOS(s1));
  _score2->setString(FTOS(s2));
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


