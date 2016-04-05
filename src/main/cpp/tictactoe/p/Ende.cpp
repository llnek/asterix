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
#include "Game.h"
#include "Ende.h"
#include "n/C.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
//
void Ende::onReplay() {
  auto x= (GCXX*)getSceneX()->ejectCtx();
  ++x->count;
  cx::runEx(Game::reify(x));
}

//////////////////////////////////////////////////////////////////////////////
//
void Ende::onQuit() {
  cx::runEx(XCFG()->prelude());
}

//////////////////////////////////////////////////////////////////////////////
//
void Ende::decoUI() {

  auto qn= cx::reifyBmfLabel("dft", gets("gameover"));
  auto ctx= (ECX*) getCtx();
  auto wb= cx::visBox();

  centerImage("game.bg")->setOpacity(0.8 * 255);

  // text msg
  qn->setScale(XCFG()->getScale() * 0.3);
  qn->setPosition(wb.cx, wb.top * 0.75);
  qn->setColor(XCFG()->getColor("text"));
  addItem(qn);

  // btns
  auto b1= cx::reifyMenuBtn("play.png");
  auto b2= cx::reifyMenuBtn("quit.png");
  s_vec<c::MenuItem*> btns {b1, b2} ;
  auto menu= cx::mkVMenu(btns);

  b1->setCallback(
      [=](c::Ref*) { this->onReplay(); });

  b2->setCallback(
      [=](c::Ref*) { this->onQuit(); });

  menu->setPosition(wb.cx, wb.cy);
  addItem(menu);

}


NS_END


