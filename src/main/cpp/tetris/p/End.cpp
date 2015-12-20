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
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(tetris)

//////////////////////////////////////////////////////////////////////////////
//
void ELayer::onReplay(c::Ref*) {
  auto x= (f::GCX*)getSceneX()->getCtx();
  auto m= MGMS()->getMode();
  getSceneX()->setCtx(nullptr,false);
  cx::runScene(
      Game::reify(mc_new(f::GCX)),
      CC_CSV(c::Float, "SCENE_DELAY"));
}

//////////////////////////////////////////////////////////////////////////////
//
void ELayer::onQuit(c::Ref*) {
  cx::runScene(
      XCFG()->prelude(),
      CC_CSV(c::Float, "SCENE_DELAY"));
}

//////////////////////////////////////////////////////////////////////////////
//
void ELayer::decorate() {

  auto qn= cx::reifyBmfLabel("font.OCR",XCFG()->getL10NStr("gameover"));
  auto wz= cx::visRect();
  auto cw= cx::center();
  auto wb= cx::visBox();
  int tag;

  // text msg
  qn->setScale(XCFG()->getScale() * 0.3f);
  qn->setPosition(cw.x, wb.top * 0.75f);
  qn->setColor(XCFG()->getColor("text"));
  qn->setOpacity(0.9f*255);
  addItem(qn);

  // btns
  auto b1= cx::reifyMenuBtn("play.png");
  auto b2= cx::reifyMenuBtn("quit.png");
  auto menu= cx::mkVMenu(s_vec<c::MenuItem*> {b1, b2} );

  b1->setTarget(this,
      CC_MENU_SELECTOR(ELayer::onReplay));

  b2->setTarget(this,
      CC_MENU_SELECTOR(ELayer::onQuit));

  menu->setPosition(cw.x, wb.top * 0.5f);
  addItem(menu);
}


NS_END(tetris)


