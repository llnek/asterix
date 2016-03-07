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
NS_BEGIN(monsters)

//////////////////////////////////////////////////////////////////////////////
//
void Ende::decoUI() {

  auto label= cx::reifyBmfLabel("dft", "Game Over");
  auto wb= cx::visBox();

  label->setPosition(wb.cx, wb.top * 0.6);
  addItem(label);

  auto replay = c::MenuItemLabel::create(
      cx::reifyBmfLabel("dft", "Replay"),
      [=](c::Ref*) {
      });
  auto quit = c::MenuItemLabel::create(
      cx::reifyBmfLabel("dft", "Quit"),
      [=](c::Ref*) {
      });

  auto menu= cx::mkVMenu(s_vec<c::MenuItem*>{replay, quit}, CC_CSIZE(replay).height/4);
  menu->setPosition(wb.cx, wb.cy);
  addItem(menu, 10);

    //[restartItem runAction:[CCScaleTo actionWithDuration:0.5 scale:1.0]];
    //[label runAction:[CCScaleTo actionWithDuration:0.5 scale:1.0]];
}


NS_END



