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

#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Splash.h"
#include "Game.h"
#include "Ende.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(blast)

//////////////////////////////////////////////////////////////////////////////
//
void Ende::decoUI() {
  auto splash = cx::reifyMenuBtn("splash-std.png", "splash-sel.png");
  auto retry = cx::reifyMenuBtn("replay-std.png", "replay-sel.png");
  auto wz= cx::visRect();
  auto wb= cx::visBox();

  auto menu = cx::mkVMenu(s_vec<c::MenuItem*> {
      retry,splash
  }, wz.size.height/4);

  menu->setPosition(wb.cx,wb.cy);

  retry->setCallback([=](c::Ref*) {
    cx::sfxPlay("button");
    if (XCFG()->hasAudio()) {
      cx::stopMusic();
    }
    cx::runEx(Game::reify( new GameCtx() ));
  });

  splash->setCallback([=](c::Ref*) {
    cx::sfxPlay("button");
    if (XCFG()->hasAudio()) {
      cx::stopMusic();
    }
    cx::runEx(Splash::reify());
  });

  auto title= cx::reifySprite("game-over.png");
  title->setPosition(wb.cx, wb.top * 0.8);

  centerImage("gui.bg");

  addItem(title);
  addItem(menu);
}


NS_END



