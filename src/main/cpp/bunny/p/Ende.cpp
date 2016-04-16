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

#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Splash.h"
#include "Game.h"
#include "Ende.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(bunny)

//////////////////////////////////////////////////////////////////////////////
//
void Ende::decoUI() {
  auto label = cx::reifyBmfLabel("title", "Game Over");
  XCFG()->scaleBmfont(label,96);
  auto wz= cx::visSize();
  auto wb= cx::visBox();

  label->enableOutline(c::Color4B(255, 0, 0, 100),6);
  label->enableGlow(c::Color4B(255, 0, 0, 255));
  label->enableShadow();
  CC_POS2(label, wb.cx, wb.top * 0.6);
  addItem(label, 1);

  auto score = getHUD()->getScore();
  label = cx::reifyBmfLabel("dft", "Your score: " + FTOS(score));
  XCFG()->scaleBmfont(label,48);
  CC_POS2(label, wb.cx, wb.top * 0.5);
  addItem(label, 1);

  auto play = cx::reifyMenuBtn("play.png", "play_pressed.png");
  CC_POS2(play, wb.cx, wb.top * 0.2);
  play->setCallback([=](c::Ref*){
      cx::runEx(Game::reify(mc_new(GameCtx)));
      });
  addItem(cx::mkMenu(play), 1);
}


NS_END



