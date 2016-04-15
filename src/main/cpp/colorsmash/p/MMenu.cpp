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
NS_BEGIN(colorsmash)

//////////////////////////////////////////////////////////////////////////////
//
void MMenu::decoUI() {

  auto wz= cx::visSize();
  auto wb= cx::visBox();

  auto bg = c::LayerColor::create(
      c::Color4B(25, 0, 51, 255),
      CC_ZW(wz),
      CC_ZH(wz));
  addItem(bg, -1);

  auto title = cx::reifyBmfLabel("title", "Options");
  XCFG()->scaleBmfont(title, 64);
  CC_POS2(title, wb.cx, wb.top * 0.75);
  addItem(title);

  auto b1 = cx::reifyMenuText("btns", "Restart");
  auto b3 = cx::reifyMenuText("btns", "Quit");
  XCFG()->scaleBmfont(b1, 32);
  XCFG()->scaleBmfont(b3, 32);
  b1->setCallback([=](c::Ref*) {
      cx::pop();
      cx::runEx(Game::reify( new f::GCX() ));
      });
  b3->setCallback([=](c::Ref*) {
        cx::pop();
        cx::prelude();
      });
  auto menu= cx::mkVMenu(s_vec<c::MenuItem*>{ b1, b3 },
      CC_CHT(b1)/GOLDEN_RATIO);
  CC_POS2(menu, wb.cx, wb.cy);
  addItem(menu);

  auto b2= cx::reifyMenuBtn("icon_back.png");
  auto gap= CC_CHT(b2)/GOLDEN_RATIO;
  b2->setAnchorPoint(cx::anchorBL());
  b2->setCallback([=](c::Ref*) {
        if (XCFG()->hasAudio()) {
          cx::resumeMusic();
        }
        cx::pop();
      });
  CC_POS2(b2, wb.left + gap, wb.bottom + gap);
  addItem(cx::mkMenu(b2));

  // audio
  auto audios= cx::reifyAudioIcons();
  auto c= cx::white();

  audios[0]->setColor(c);
  audios[1]->setColor(c);

  gap= CC_CHT(audios[0])/GOLDEN_RATIO;
  addAudioIcons(audios,
      cx::anchorBR(),
      CCT_PT(wb.right-gap, wb.bottom+gap));

  if (XCFG()->hasAudio()) {
    cx::pauseMusic();
  }
}


NS_END



