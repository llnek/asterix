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
#include "HUD.h"
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(colorsmash)

//////////////////////////////////////////////////////////////////////////////
//
void Ende::decoUI() {

  auto wz= cx::visSize();
  auto wb= cx::visBox();

  auto popup = c::LayerColor::create(
      c::Color4B(0, 0, 0, 196), CC_ZW(wz), CC_ZH(wz));
  //popup->setOpacity(0);
  //popup->runAction(c::FadeTo::create(0.25, 196));
  addItem(popup, 10);

  auto title = cx::reifyBmfLabel("title", "Game Over");
  XCFG()->scaleBmfont(title,64);
  CC_POS2(title, wb.cx, wb.top * 0.8);
  addItem(title);

  auto score = getHUD()->getScore();
  auto scoreLabel = cx::reifyBmfLabel("dft", "Score: " + FTOS(score));
  auto k= XCFG()->scaleBmfont(scoreLabel, 48);
  CC_POS2(scoreLabel, wb.cx, wb.top * 0.6);
  scoreLabel->runAction(
      c::Sequence::create(
        c::DelayTime::create(0.5),
        c::EaseSineIn::create(c::ScaleTo::create(0.25, 1.1*k)),
        c::EaseSineOut::create(c::ScaleTo::create(0.25, k)),
                          CC_NIL));
  addItem(scoreLabel);

  auto t = cx::reifyMenuText("btns", "Try Again?");
  auto q = cx::reifyMenuText("btns", "Quit");
  XCFG()->scaleBmfont(t,"36");
  XCFG()->scaleBmfont(q,"36");
  t->setCallbacl([=](c::Ref*) {
        cx::runEx(Game::reify(mc_new(GameCtx)));
      });
  q->setCallback([=](c::Ref*) {
        cx::prelude();
      });
  auto menu= cx::mkVMenu(s_vec<c::MenuItem*>{ t, q},
      CC_CHT(t)/GOLDEN_RATIO);
  CC_POS2(menu, wb.cx, wb.cy);
  addItem(menu);

}


NS_END



