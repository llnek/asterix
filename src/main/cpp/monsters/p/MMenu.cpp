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
#include "MMenu.h"
#include "Splash.h"
#include "Game.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(monsters)

//////////////////////////////////////////////////////////////////////////////
//
void MMenu::decoUI() {
  auto retry = cx::reifyMenuText("btns", "Restart");
  auto quit = cx::reifyMenuText("btns", "Quit");
  auto wz= cx::visSize();
  auto wb= cx::visBox();

  centerImage("game.bg");

  retry->setCallback([=](c::Ref*) {
    cx::sfxPlay("button");
    cx::pop();
    if (XCFG()->hasAudio()) {
      cx::stopMusic();
    }
    cx::runEx(Game::reify(mc_new(GameCtx)));
  });

  quit->setCallback([=](c::Ref*) {
    cx::sfxPlay("button");
    cx::pop();
    if (XCFG()->hasAudio()) {
      cx::stopMusic();
    }
    cx::prelude();
  });

  auto menu = cx::mkVMenu(s_vec<c::MenuItem*> {
      retry,quit
  }, CC_CHT(quit)/GOLDEN_RATIO);
  CC_POS2(menu, wb.cx,wb.cy);
  addItem(menu);

  // audio
  auto audios= cx::reifyAudioIcons();
  auto sz= CC_CSIZE(audios[0]);
  auto c= cx::white();

  audios[0]->setColor(c);
  audios[1]->setColor(c);

  auto gap= CC_CHT(audios[0])/GOLDEN_RATIO;
  addAudioIcons(audios,
      cx::anchorBR(),
      CCT_PT(wb.right-gap, wb.bottom+gap));

  if (XCFG()->hasAudio()) {
    cx::pauseMusic();
  }
}


NS_END



