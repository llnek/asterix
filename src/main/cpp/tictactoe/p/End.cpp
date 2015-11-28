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
public:

  virtual f::XLayer* realize();
  NO__CPYASS(UILayer)
  DECL_CTOR(UILayer)

  void onReplay(c::Ref*);
  void onQuit(c::Ref*);

  f::GMode mode;
};

//////////////////////////////////////////////////////////////////////////////
//
UILayer::~UILayer() {
}

//////////////////////////////////////////////////////////////////////////////
//
UILayer::UILayer() {
}

//////////////////////////////////////////////////////////////////////////////
//
void UILayer::onReplay(c::Ref*) {
  auto g = f::reifyRefType<Game>();
  prepareSeedData(mode);
  cx::runScene( Game::reify(g, mode) );
}

//////////////////////////////////////////////////////////////////////////////
//
void UILayer::onQuit(c::Ref*) {
  cx::runScene( XCFG()->startWith() );
}

//////////////////////////////////////////////////////////////////////////////
//
f::XLayer* UILayer::realize() {

  auto qn= cx::reifyBmfLabel("font.OCR", XCFG()->getL10NStr("gameover"));
  auto wz= cx::visRect();
  auto cw= cx::center();
  auto wb= cx::visBox();
  int tag;

  centerImage("game.bg");

  // test msg
  qn->setScale(XCFG()->getScale() * 0.3f);
  qn->setPosition(cw.x, wb.top * 0.75f);
  qn->setColor(cx::white());
  qn->setOpacity(0.9*255);
  addItem(qn);

  // btns
  auto b1= cx::reifyMenuBtn("play.png");
  auto b2= cx::reifyMenuBtn("quit.png");
  auto menu= cx::mkMenu(s::vector<c::MenuItem*> {b1, b2}, true, 10.0);

  b1->setTarget(this,
      CC_MENU_SELECTOR(UILayer::onReplay));

  b2->setTarget(this,
      CC_MENU_SELECTOR(UILayer::onQuit));

  menu->setPosition(cw.x, wb.top * 0.5);
  addItem(menu);

  return this;
}

END_NS_UNAMED()


//////////////////////////////////////////////////////////////////////////////
//
f::XScene* EndGame::realize() {
  auto y = f::reifyRefType<UILayer>();
  addLayer(y);
  y->realize();
  return this;
}

//////////////////////////////////////////////////////////////////////////
//
EndGame* EndGame::reify(f::GMode m) {
  auto g = f::reifyRefType<EndGame>();
  g->mode= m;
  g->realize();
  return g;
}

//////////////////////////////////////////////////////////////////////////
//
EndGame::~EndGame() {

}

//////////////////////////////////////////////////////////////////////////
//
EndGame::EndGame() {
  mode=f::GMode::ONE;
}

NS_END(tttoe)

