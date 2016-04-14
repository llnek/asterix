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
NS_BEGIN(flappy)

//////////////////////////////////////////////////////////////////////////////
//
void MMenu::decoUI() {
  auto r= cx::reifyMenuText("btns", "Restart");
  auto q= cx::reifyMenuText("btns", "Quit");
  auto wz= cx::visSize();
  auto wb= cx::visBox();

  centerImage("game.bg");
  regoAtlas("game-pics");

  r->setCallback([=](c::Ref*) {
    cx::sfxPlay("button");
    cx::pop();
    if (XCFG()->hasAudio()) {
      cx::stopMusic();
    }
    cx::runEx(Game::reify(mc_new(GameCtx)));
  });

  q->setCallback([=](c::Ref*) {
    cx::sfxPlay("button");
    cx::pop();
    if (XCFG()->hasAudio()) {
      cx::stopMusic();
    }
    cx::prelude();
  });

  auto menu = cx::mkVMenu(s_vec<c::MenuItem*> {r,q},
      CC_CHT(r)/GOLDEN_RATIO);
  CC_POS2(menu, wb.cx,wb.cy);
  addItem(menu);

  auto back= cx::reifyMenuBtn("icon_back.png");
  auto gap= CC_CHT(back)/GOLDEN_RATIO;
  back->setAnchorPoint(cx::anchorBL());
  back->setCallback([=](c::Ref*) {
    cx::sfxPlay("button");
    cx::pop();
    if (XCFG()->hasAudio()) {
      cx::resumeMusic();
    }
  });
  CC_POS2(back, wb.left+gap, wb.bottom+gap);
  addAtlasItem("game-pics", back);

  // audio
  auto audios= cx::reifyAudioIcons();
  auto c= cx::white();

  audios[0]->setColor(c);
  audios[1]->setColor(c);

  gap= CC_CWH(audios[0]) /GOLDEN_RATIO;
  addAudioIcons(audios,
      cx::anchorBR(),
      CCT_PT(wb.right-gap, wb.bottom+gap));

  if (XCFG()->hasAudio()) {
    cx::pauseMusic();
  }
}


NS_END



