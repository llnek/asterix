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
#include "MMenu.h"
#include "Splash.h"
#include "Game.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(blast)

//////////////////////////////////////////////////////////////////////////////
//
void MMenu::decoUI() {
  auto resume = cx::reifyMenuBtn("resume-std.png", "resume-sel.png");
  auto retry = cx::reifyMenuBtn("replay-std.png", "replay-sel.png");
  auto splash = cx::reifyMenuBtn("splash-std.png", "splash-sel.png");
  auto wz= cx::visRect();
  auto wb= cx::visBox();

  auto menu = cx::mkVMenu(s_vec<c::MenuItem*> {
      resume,retry,splash
  }, wz.size.height/4);

  resume->setCallback([=](c::Ref*) {
    cx::sfxPlay("button");
    cx::pop();
    if (XCFG()->hasAudio()) {
      cx::resumeMusic();
    }
  });

  retry->setCallback([=](c::Ref*) {
    cx::sfxPlay("button");
    cx::pop();
    if (XCFG()->hasAudio()) {
      cx::stopMusic();
    }
    cx::runEx(Game::reify( new GameCtx() ));
  });

  splash->setCallback([=](c::Ref*) {
    cx::sfxPlay("button");
    cx::pop();
    if (XCFG()->hasAudio()) {
      cx::stopMusic();
    }
    cx::runEx(Splash::reify());
  });

  menu->setPosition(wb.cx,wb.cy);
  centerImage("gui.bg");
  addItem(menu);

  // audio
  auto audios= cx::reifyAudioIcons();
  auto sz= CC_CSIZE(audios[0]);
  auto gap= sz.width /4;
  auto c= cx::white();

  audios[0]->setColor(c);
  audios[1]->setColor(c);

  addAudioIcons(audios,
      cx::anchorBR(),
      c::Vec2(wb.right-gap, wb.bottom+gap));

  if (XCFG()->hasAudio()) {
    cx::pauseMusic();
  }
}


NS_END



