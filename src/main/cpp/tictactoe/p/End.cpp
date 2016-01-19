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
#include "Game.h"
#include "End.h"
#include "n/N.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
//
void ELayer::onReplay() {
  auto x= (GCXX*)getSceneX()->emitCtx();
  x->count++;
  cx::runEx(Game::reify(x));
}

//////////////////////////////////////////////////////////////////////////////
//
void ELayer::onQuit() {
  cx::runEx(XCFG()->prelude());
}

//////////////////////////////////////////////////////////////////////////////
//
void ELayer::decorate() {

  auto qn= cx::reifyBmfLabel("font.OCR", gets("gameover"));
  auto ctx= (ECX*) getCtx();
  auto cw= cx::center();
  auto wb= cx::visBox();

  // clear out the grid
  auto css= CC_GNLF(CSquares,ctx->arena,"squares");
  F__LOOP(it,css->sqs) {
    auto q= *it;
    q->sprite->setOpacity(0.1f * 255);
  }

  // text msg
  qn->setScale(XCFG()->getScale() * 0.3f);
  qn->setPosition(cw.x, wb.top * 0.75f);
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

  menu->setPosition(cw.x, wb.top * 0.5f);
  addItem(menu);
}


NS_END(tttoe)


