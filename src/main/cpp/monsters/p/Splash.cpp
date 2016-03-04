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

  auto txt= cx::reifyBmfLabel("dft", "Play");
  auto btn= cx::reifyMenuBtn("button.png");
  auto menu= cx::mkMenu(btn);
  auto wb=cx::visBox();

  centerImage("game.bg");

  btn->setPosition(wb.cx, wb.cy);
  btn->setCallback([=](c::Ref*) {
    cx::sfxPlay("button");
      cx::runEx(Game::reify( new f::GCX()));
  });
  addItem(menu);

  txt->setPosition(wb.cx,wb.cy);
  txt->setScale(0.75);
  addItem(txt);
}


NS_END


