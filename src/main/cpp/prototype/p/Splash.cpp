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
NS_BEGIN(prototype)

//////////////////////////////////////////////////////////////////////////////
//
void Splash::decoUI() {

  auto title= cx::createSprite("title");
  auto wb= cx::visBox();

  centerImage("gui.bg");

  title->setPosition(wb.cx, wb.top * 0.8);
  addItem(title);

  auto play= cx::createMenuBtn("play-std.btn", "play-sel.btn");
  auto menu= cx::mkMenu(play);
  play->setPosition(wb.cx, wb.top * 0.2);
  play->setCallback([=](c::Ref*){
      cx::sfxPlay("button");
      cx::runEx(Game::reify(new GameCtx() ));
  });
  addItem(menu);

}


NS_END


