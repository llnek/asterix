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
#include "s/utils.h"
#include "Game.h"
#include "End.h"
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
//
BEGIN_NS_UNAMED()
class CC_DLL UILayer : public f::XLayer {
protected:

  void onReplay(c::Ref*);
  void onQuit(c::Ref*);

public:

  STATIC_REIFY_LAYER(UILayer)

  virtual void decorate();
  virtual ~UILayer() {}
  UILayer() {}
  NOCPYASS(UILayer)
};

//////////////////////////////////////////////////////////////////////////////
//
void UILayer::onReplay(c::Ref*) {
  auto m= MGMS()->getMode();
  auto d= fmtGameData(m);
  //cx::runScene( Game::reify(g, mode) );
}

//////////////////////////////////////////////////////////////////////////////
//
void UILayer::onQuit(c::Ref*) {
  cx::runScene( XCFG()->prelude() );
}

//////////////////////////////////////////////////////////////////////////////
//
void UILayer::decorate() {

  auto qn= cx::reifyBmfLabel("font.OCR", XCFG()->getL10NStr("gameover"));
  auto wz= cx::visRect();
  auto cw= cx::center();
  auto wb= cx::visBox();
  int tag;

  centerImage("game.bg");

  // text msg
  qn->setScale(XCFG()->getScale() * 0.3f);
  qn->setPosition(cw.x, wb.top * 0.75f);
  qn->setColor(cx::white());
  qn->setOpacity(0.9*255);
  addItem(qn);

  // btns
  auto b1= cx::reifyMenuBtn("play.png");
  auto b2= cx::reifyMenuBtn("quit.png");
  auto menu= cx::mkVMenu(s_vec<c::MenuItem*> {b1, b2} );

  b1->setTarget(this,
      CC_MENU_SELECTOR(UILayer::onReplay));

  b2->setTarget(this,
      CC_MENU_SELECTOR(UILayer::onQuit));

  menu->setPosition(cw.x, wb.top * 0.5f);
  addItem(menu);
}

END_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
void EndGame::decorate() {
  UILayer::reify(this);
}

NS_END(tttoe)

