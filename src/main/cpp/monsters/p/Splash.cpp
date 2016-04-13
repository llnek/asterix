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

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(monsters)

//////////////////////////////////////////////////////////////////////////////
//
void Splash::decoUI() {

  auto txt= cx::reifyBmfLabel("btns", "PLAY");
  auto btn= cx::reifyMenuBtn("button.png");
  auto sz= CC_CSIZE(btn);
  auto tz= CC_CSIZE(txt);
  auto wb= cx::visBox();

  centerImage("game.bg");

  CC_POS2(btn, wb.cx, wb.cy);
  txt->setColor(c::Color3B::ORANGE);
  XCFG()->scaleNode(txt,36);
  tz= CC_CSIZE(txt);
  btn->addChild(txt);
  btn->setCallback(
      [=](c::Ref*) {
        cx::sfxPlay("button");
        cx::runEx(Game::reify(mc_new(GameCtx)));
      });
  //need to do this to align txt to center, find better way
  CC_POS2(txt, sz.width/2, sz.height/2- tz.height/2/4);
  addItem(cx::mkMenu(btn));

}


NS_END


