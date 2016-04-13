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

  auto wz= cx::visRect();
  auto wb= cx::visBox();

  auto popup = c::LayerColor::create(
      c::Color4B(0, 0, 0, 196), CC_ZW(wz.size), CC_ZH(wz.size));
  //popup->setOpacity(0);
  //popup->runAction(c::FadeTo::create(0.25, 196));
  addItem(popup, 10);

  auto title = cx::reifyLabel("dft", 52,"Game Over");
  title->setPosition(wb.cx, wb.top * 0.75);
  addItem(title);

  auto score = getHUD()->getScore();
  auto scoreLabel = cx::reifyLabel("dft", 48, "Score: " + FTOS(score));
  scoreLabel->setPosition(wb.cx, wb.top * 0.6);
  scoreLabel->runAction(
      c::Sequence::create(
        c::DelayTime::create(0.5),
        c::EaseSineIn::create(c::ScaleTo::create(0.25, 1.1)),
        c::EaseSineOut::create(c::ScaleTo::create(0.25, 1)),
                          CC_NIL));
  addItem(scoreLabel);

  auto b1 = c::MenuItemLabel::create(
      cx::reifyLabel("dft", 32, "Replay"),
      [=](c::Ref*) {
        cx::runEx(Game::reify( new f::GCX() ));
      });
  auto b2 = c::MenuItemLabel::create(
      cx::reifyLabel("dft", 32, "Quit"),
      [=](c::Ref*) {
        cx::runEx(Splash::reify());
      });

  auto menu= cx::mkVMenu(s_vec<c::MenuItem*>{ b1, b2 }, CC_CSIZE(b1).width/4);
  menu->setPosition(wb.cx, wb.cy);
  addItem(menu);

}


NS_END



