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

#include "core/XConfig.h"
#include "core/CCSX.h"
#include "x2d/XLib.h"
#include "Game.h"
#include "MMenu.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(breakout)

BEGIN_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL UILayer : public f::XLayer {
  STATIC_REIFY_LAYER(UILayer)
  MDECL_DECORATE()
};

//////////////////////////////////////////////////////////////////////////////
//
void UILayer::decorate() {
    auto tile = CC_CSV(c::Integer,"TILE");
  auto wb=cx::visBox();
  auto cw= cx::center();

  centerImage("gui.mmenus.menu.bg");
  incIndexZ();

  auto tt= cx::reifyBmfLabel("font.JellyBelly", gets("mmenu"));
  tt->setPosition(cw.x, wb.top * 0.9f);
  tt->setColor(XCFG()->getColor("default"));
  tt->setScale(XCFG()->getScale());
  addItem(tt);

  auto btn= cx::reifyMenuBtn("play.png");
  auto menu= cx::mkMenu(btn);
  btn->setCallback([=](c::Ref*){
    cx::runSceneEx(Game::reify(mc_new(f::GCX)));
  });
  menu->setPosition(cw);
  addItem(menu);

  // back-quit buttons
  auto b= cx::reifyMenuBtn("icon_back.png");
  auto q= cx::reifyMenuBtn("icon_quit.png");
  auto sz= b->getContentSize();
    auto ctx = (MCX*) getSceneX()->getCtx();
  q->setTarget(this, CC_MENU_SELECTOR(UILayer::onQuit));
  b->setCallback([=](c::Ref*) { ctx->back(); });

  auto m2= cx::mkHMenu(s_vec<c::MenuItem*> {b, q} );
  m2->setPosition(wb.left + tile + sz.width * 1.1f,
                  wb.bottom + tile + sz.height * 0.45f);
  addItem(m2);

  auto audios = cx::reifyAudioIcons();
    auto dfc = XCFG()->getColor("default");
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






NS_END(breakout)



