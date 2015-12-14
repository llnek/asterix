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
#include "Menu.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(tetris)

BEGIN_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL UILayer : public f::XLayer {
public:

  STATIC_REIFY_LAYER(UILayer)
  NOCPYASS(UILayer)

  virtual void decorate();
  virtual ~UILayer() {}
  UILayer() {}
};

//////////////////////////////////////////////////////////////////////////
//
void UILayer::decorate() {

  auto tt= cx::reifyBmfLabel("font.JellyBelly",
      XCFG()->getL10NStr("mmenu"));
  auto tile = MGMS()->TILE;
  auto cw= cx::center();
  auto wb= cx::visBox();
  c::Color3B c(246,177,127);

  centerImage("gui.mmenu.menu.bg");

  tt->setPosition(cw.x, wb.top * 0.9f);
  tt->setColor(c);
  tt->setScale(XCFG()->getScale());
  addItem(tt);

  auto b= cx::reifyMenuBtn("player1.png");
  auto menu= cx::mkMenu(b);
  b->setCallback([=]() {
        cx::runScene(
            Game::reify(mc_new(f::GContext)));
      });
  b->setPosition(cw);
  addItem(menu);

  // back-quit button
  auto back= cx::reifyMenuBtn("icon_back.png");
  auto sz= back->getContentSize();
  back->setCallback([=](c::Ref*) { ctx->back(); });
  back->setColor(c);

  // audio
  c::MenuItem* off;
  c::MenuItem* on;

  auto quit= cx::reifyMenuBtn("icon_quit.png");
  quit->setTarget(this,
      CC_MENU_SELECTOR(UILayer::onQuit));
  quit->setColor(c);

  auto m2= cx::mkMenu(s_vec<c::MenuItem*> {back, quit}, false, 10.0);
  m2->setPosition(wb.left + tile + sz.width * 1.1,
                  wb.bottom + tile + sz.height * 0.45);
  addItem(m2);

  cx::reifyAudioIcons(on, off);
  off->setColor(c);
  on->setColor(c);

  addAudioIcons((XLayer*) this,
    off, on,
    cx::anchorBR(),
    c::Vec2(wb.right - tile, wb.bottom + tile));

}

END_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////
//
void MainMenu::decorate() {
  UILayer::reify(this);
}



NS_END(tetris)

