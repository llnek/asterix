// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Ken Leung. All rights reserved.

#include "2d/CCMenuItem.h"
#include "2d/CCMenu.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "MMenu.h"
#include "Game.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(invaders)

//////////////////////////////////////////////////////////////////////////////
//
void MMenu::decorate() {

  auto tile = CC_CSV(c::Float, "TILE");
  auto dfc= XCFG()->getColor("dft");
  auto ctx = (MCX*) getCtx();
  auto wb = cx::visBox();

  centerImage("mmenus.bg");

  // title
  auto lb= cx::reifyBmfLabel(wb.cx, wb.top * 0.9f, "JellyBelly", gets("mmenu"));
  lb->setColor(XCFG()->getColor("text"));
  lb->setScale(XCFG()->getScale());
  addItem(lb);

  // play button
  auto b1= cx::reifyMenuBtn("play.png");
  auto menu= cx::mkMenu(b1);
  auto x= mc_new(f::GCX);

  b1->setCallback(
      [=](c::Ref*) { cx::runEx(Game::reify(x)); });

  menu->setPosition(wb.cx, wb.cy);
  addItem(menu);

  // back-quit buttons
  auto b= cx::reifyMenuBtn("icon_back.png");
  auto q= cx::reifyMenuBtn("icon_quit.png");
  auto sz= b->getContentSize();

  q->setCallback([](c::Ref*) { cx::prelude();  });
  b->setCallback([=](c::Ref*) { ctx->back(); });

  s_vec<c::MenuItem*> btns {b, q};
  auto m2= cx::mkHMenu(btns);
  m2->setPosition(wb.left + tile + sz.width * 1.1f,
                  wb.bottom + tile + sz.height * 0.45f);
  addItem(m2);

  auto audios = cx::reifyAudioIcons();
  audios[0]->setColor(dfc);
  audios[1]->setColor(dfc);

  addAudioIcons(audios,
      cx::anchorBR(),
      c::Vec2(wb.right - tile, wb.bottom + tile));
}


NS_END(invaders)

