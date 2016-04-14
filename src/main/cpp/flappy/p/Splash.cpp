// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Kenneth Leung. All rights reserved.

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Splash.h"
#include "Game.h"
#include "n/Fairytale.h"
#include "n/Dragon.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(flappy)

//////////////////////////////////////////////////////////////////////////////
//
void Splash::decoUI() {

  auto wz= cx::visSize();
  auto wb= cx::visBox();

  regoAtlas("game-pics", 1+E_LAYER_BG);
  regoAtlas("dhtex", 1+E_LAYER_BG);

  fairytale= mc_new1(Fairytale, this);
  fairytale->init();

  Dragon::create(CC_NIL, this);

  auto title= cx::reifySprite("dhtitle");
  XCFG()->fit(title);
  CC_POS2(title, wb.cx, wb.top * 0.75);
  addAtlasItem("dhtex", title,1);

  auto btn = cx::reifyMenuBtn("dhplay");
  XCFG()->fit(btn);
  btn->setCallback([=](c::Ref*) {
      cx::runEx(Game::reify(mc_new(GameCtx)));
  });
  CC_POS2(btn, wb.cx, wb.top * 0.25);
  addItem(cx::mkMenu(btn), 1);

  scheduleUpdate();
}

//////////////////////////////////////////////////////////////////////////////
//
void Splash::update(float dt) {
  fairytale->update(dt);
}


NS_END


