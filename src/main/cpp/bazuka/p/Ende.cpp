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
#include "HUD.h"
#include "Ende.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(bazuka)

//////////////////////////////////////////////////////////////////////////////
//
void Ende::decoUI() {

  auto btn = cx::reifyMenuBtn("mainmenu.png");
  XCFG()->fit(btn);
  auto wz= cx::visSize();
  auto wb= cx::visBox();

  btn->setCallback([=](c::Ref*) {
      cx::sfxPlay("pop");
      cx::prelude();
      });
  CC_POS2(btn, wb.cx, wb.cy);
  addItem(cx::mkMenu(btn));

  auto text = cx::reifyBmfLabel("title", "Game Over!");
  XCFG()->scaleBmfont(text,64);
  CC_POS2(text, wb.cx, wb.top * 0.6);
  addItem(text, 10);

  auto high = CC_APPDB()->getIntegerForKey("GameHighScore");
  auto score= getHUD()->getScore();
  if (score > high) {
    CC_APPDB()->setIntegerForKey("GameHighScore", score);
    CC_APPDB()->flush();
    text = cx::reifyBmfLabel("pixel", "NEW HIGH SCORE");
    auto k= XCFG()->scaleBmfont(text,50);
    CC_POS2(text, wb.cx, wb.cy);
    text->setScale(k * 0.75);
    addItem(text, 10);

    text = cx::reifyBmfLabel("pixel", FTOS(score));
    k= XCFG()->scaleBmfont(text,50);
    CC_POS2(text, wb.cx, wb.top * 0.4);
    text->setScale(k * 0.75);
    addItem(text, 10);
  } else {
    text = cx::reifyBmfLabel("pixel", "BETTER LUCK NEXT TIME");
    k= XCFG()->scaleBmfont(text,50);
    CC_POS2(text, wb.cx, wb.cy);
    text->setScale(k * 0.75);
    addItem(text, 10);
  }

}





NS_END



