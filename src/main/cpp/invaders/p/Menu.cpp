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

#include "core/Primitives.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "2d/CCMenuItem.h"
#include "2d/CCMenu.h"
#include "Menu.h"
#include "Game.h"
NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(invaders)

BEGIN_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL UILayer : public f::XLayer {
protected:

  virtual f::XLayer* realizeEx(VOIDFN );
  void onPlay(c::Ref*);
  void onBack(c::Ref*);

  DECL_CTOR(UILayer)
  NOCPYASS(UILayer)
  VOIDFN backAction;
public:
  STATIC_REIFY_LAYER(UILayer)
};

//////////////////////////////////////////////////////////////////////////////
//
f::XLayer* UILayer::realizeEx(VOIDFN cb) {

  auto tile = CC_CSV(c::Integer, "TILE");
  auto color= c::Color3B(94,49,120);
  auto wb = cx::visBox();
  auto cw = cx::center();

  // background
  centerImage("mmenus.bg");

  // title
  auto lb= cx::reifyBmfLabel(cw.x, wb.top * 0.9f,
                          "font.JellyBelly",
                          XCFG()->getL10NStr("mmenu"));
  lb->setScale(XCFG()->getScale());
  lb->setColor(cx::white());
  addItem(lb);

  // play button
  auto b1= cx::reifyMenuBtn("play.png");
  auto menu= cx::mkMenu(b1);
  b1->setTarget(this,
      CC_MENU_SELECTOR(UILayer::onPlay));
  menu->setPosition(cw);
  addItem(menu);

  // back-quit buttons
  auto b= cx::reifyMenuBtn("icon_back.png");
  auto q= cx::reifyMenuBtn("icon_quit.png");
  auto sz= b->getContentSize();
  b->setTarget(this,
      CC_MENU_SELECTOR(UILayer::onBack));
  q->setTarget(this,
      CC_MENU_SELECTOR(XLayer::onQuit));
  auto m2= cx::mkMenu(s::vector<c::MenuItem*> {b, q}, false, 10.0f);
  m2->setPosition(wb.left + tile + sz.width * 1.1f,
                  wb.bottom + tile + sz.height * 0.45f);
  addItem(m2);

  // audio
  c::MenuItem* off;
  c::MenuItem* on;
  cx::reifyAudioIcons(off, on);
  off->setColor(color);
  on->setColor(color);

  addAudioIcons(this, off, on,
      cx::anchorBR(),
      c::Vec2(wb.right - tile, wb.bottom + tile));

  this->backAction= cb;

  return this;
}

//////////////////////////////////////////////////////////////////////////////
//
void UILayer::onPlay(c::Ref*) {
  auto x= mc_new(f::GContext);
  auto g = Game::reify();
  g->realizeWithCtx(x);
  cx::runScene(g);
}

//////////////////////////////////////////////////////////////////////////////
//
void UILayer::onBack(c::Ref*) {
  this->backAction();
}

//////////////////////////////////////////////////////////////////////////////
//
UILayer::~UILayer() {
}

//////////////////////////////////////////////////////////////////////////////
//
UILayer::UILayer() {
  SNPTR(backAction)
}

END_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
MainMenu* MainMenu::reifyWithBackAction(VOIDFN cb) {
  auto m = MainMenu::reify();
  m->realizeEx(cb);
  return m;
}

//////////////////////////////////////////////////////////////////////////////
//
f::XScene* MainMenu::realizeEx(VOIDFN cb) {
  auto y = UILayer::reify();
  addLayer(y);
  y->realizeEx(cb);
  return this;
}




NS_END(invaders)

