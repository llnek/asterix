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

#include "2d/CCMenuItem.h"
#include "2d/CCMenu.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Menu.h"
#include "Game.h"
#include "x2d/XLib.h"
NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(invaders)

BEGIN_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL UILayer : public f::XLayer {
public:

  STATIC_REIFY_LAYER(UILayer)
  NOCPYASS(UILayer)
  IMPLCZ(UILayer)

  virtual void decorate();
};

//////////////////////////////////////////////////////////////////////////////
//
void UILayer::decorate() {

  auto ctx= (MContext*) getSceneX()->getCtx();
  auto tile = CC_CSV(c::Integer, "TILE");
  c::Color3B color(94,49,120);
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
  b1->setCallback([=](c::Ref*) {
      cx::runScene(Game::reify(mc_new(f::GContext)));
      });

  menu->setPosition(cw);
  addItem(menu);

  // back-quit buttons
  auto b= cx::reifyMenuBtn("icon_back.png");
  auto q= cx::reifyMenuBtn("icon_quit.png");
  auto sz= b->getContentSize();

  // audio
  c::MenuItem* off;
  c::MenuItem* on;
  c::Menu* m2;

  q->setTarget(this, CC_MENU_SELECTOR(XLayer::onQuit));
  b->setCallback([=](c::Ref*) { ctx->back(); });

  m2= cx::mkMenu(s::vector<c::MenuItem*> {b, q}, false, 10.0f);
  m2->setPosition(wb.left + tile + sz.width * 1.1f,
                  wb.bottom + tile + sz.height * 0.45f);
  addItem(m2);

  cx::reifyAudioIcons(off, on);
  off->setColor(color);
  on->setColor(color);

  addAudioIcons(this, off, on,
      cx::anchorBR(),
      c::Vec2(wb.right - tile, wb.bottom + tile));

}

END_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
void MainMenu::decorate() {
  UILayer::reify(this);
}


NS_END(invaders)

