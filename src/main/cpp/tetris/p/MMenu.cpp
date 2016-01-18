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
#include "Menu.h"
#include "Game.h"
#include "x2d/XLib.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(tetris)

BEGIN_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL UILayer : public f::XLayer {

  STATIC_REIFY_LAYER(UILayer)
  MDECL_DECORATE()

protected:
  void onQuit(c::Ref*);
};

//////////////////////////////////////////////////////////////////////////
//
void UILayer::decorate() {

  auto tt= cx::reifyBmfLabel("font.JellyBelly", gets("mmenu"));
  auto tile = CC_CSV(c::Integer, "TILE");
  auto c= XCFG()->getColor("default");
  auto cw= cx::center();
  auto wb= cx::visBox();

  centerImage("gui.mmenu.menu.bg");
  incIndexZ();

  tt->setPosition(cw.x, wb.top * 0.9f);
  tt->setColor(c);
  tt->setScale(XCFG()->getScale());
  addItem(tt);

  auto b= cx::reifyMenuBtn("player1.png");
  auto menu= cx::mkMenu(b);
    b->setCallback([=](c::Ref*) {
        cx::runEx(
            Game::reify(mc_new(f::GCX)));
      });
  menu->setPosition(cw);
  addItem(menu);

  // back-quit button
  auto back= cx::reifyMenuBtn("icon_back.png");
  auto ctx= (MCX*) getSceneX()->getCtx();
  auto sz= back->getContentSize();
  back->setCallback([=](c::Ref*) { ctx->back(); });
  back->setColor(c);

  auto quit= cx::reifyMenuBtn("icon_quit.png");
  quit->setTarget(this,
      CC_MENU_SELECTOR(UILayer::onQuit));
  quit->setColor(c);

  auto m2= cx::mkHMenu(s_vec<c::MenuItem*> {back, quit});
  m2->setPosition(wb.left + tile + sz.width * 1.1,
                  wb.bottom + tile + sz.height * 0.45);
  addItem(m2);

  auto audios= cx::reifyAudioIcons();
  audios[0]->setColor(c);
  audios[1]->setColor(c);

  addAudioIcons(this, audios,
    cx::anchorBR(),
    c::Vec2(wb.right - tile, wb.bottom + tile));

}

//////////////////////////////////////////////////////////////////////////////
//
void UILayer::onQuit(c::Ref*) {
  cx::runEx(
      XCFG()->prelude());
}

END_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////
//
void MMenu::decorate() {
  UILayer::reify(this);
}



NS_END(tetris)

