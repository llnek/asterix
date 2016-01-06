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

//////////////////////////////////////////////////////////////////////////////
//
BEGIN_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL UILayer : public f::XLayer {

  STATIC_REIFY_LAYER(UILayer)
  MDECL_DECORATE()

  private:

  void onQuit(c::Ref*);

};

//////////////////////////////////////////////////////////////////////////////
//
void UILayer::onQuit(c::Ref*) {
  cx::runSceneEx(
      XCFG()->prelude());
}

//////////////////////////////////////////////////////////////////////////////
//
void UILayer::decorate() {

  auto ctx = (MCX*) getSceneX()->getCtx();
  auto tile = CC_CSV(c::Integer, "TILE");
  auto dfc= XCFG()->getColor("default");
  auto wb = cx::visBox();
  auto cw = cx::center();

  // background
  centerImage("mmenus.bg");

  // title
  auto lb= cx::reifyBmfLabel(cw.x, wb.top * 0.9f, "font.JellyBelly", gets("mmenu"));
  lb->setScale(XCFG()->getScale());
  lb->setColor(XCFG()->getColor("text"));
  addItem(lb);

  // play button
  auto b1= cx::reifyMenuBtn("play.png");
  auto menu= cx::mkMenu(b1);
  b1->setCallback([=](c::Ref*) {
      cx::runScene(Game::reify(mc_new(f::GCX)));
      });

  menu->setPosition(cw);
  addItem(menu);

  // back-quit buttons
  auto b= cx::reifyMenuBtn("icon_back.png");
  auto q= cx::reifyMenuBtn("icon_quit.png");
  auto sz= b->getContentSize();

  q->setTarget(this, CC_MENU_SELECTOR(UILayer::onQuit));
  b->setCallback([=](c::Ref*) { ctx->back(); });

  auto m2= cx::mkHMenu(s_vec<c::MenuItem*> {b, q} );
  m2->setPosition(wb.left + tile + sz.width * 1.1f,
                  wb.bottom + tile + sz.height * 0.45f);
  addItem(m2);

  auto audios = cx::reifyAudioIcons();
  audios[0]->setColor(dfc);
  audios[1]->setColor(dfc);

  addAudioIcons(this, audios,
      cx::anchorBR(),
      c::Vec2(wb.right - tile, wb.bottom + tile));
}

END_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
void MMenu::decorate() {
  UILayer::reify(this);
}


NS_END(invaders)

