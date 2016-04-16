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
NS_BEGIN(bazuka)

//////////////////////////////////////////////////////////////////////////////
//
void MMenu::decoUI() {

  _bgLayer = f::reifyRefType<ScrollingBgLayer>();
  _bgLayer->set(3);
  addItem(_bgLayer);

  auto nameLabel = cx::reifyBmfLabel("title", "Options Menu");
  XCFG()->scaleBmfont(nameLabel,64);
  auto wz= cx::visSize();
  auto wb= cx::visBox();

  CC_POS2(nameLabel, wb.cx, wb.top * 0.8);
  addItem(nameLabel);

  auto splash = cx::reifyMenuBtn("mainmenu.png");
  auto back = cx::reifyMenuBtn("resume.png");
  XCFG()->fit(splash);
  XCFG()->fit(back);
  splash->setCallback([=](c::Ref*) {
    cx::sfxPlay("pop");
    cx::pop();
    cx::prelude();
  });
  back->setCallback([=](c::Ref*) {
      cx::sfxPlay("pop");
      cx::pop();
      if (XCFG()->hasAudio()) { cx::resumeMusic(); }
  });
  auto menu = cx::mkHMenu( s_vec<c::MenuItem*>{splash,back},
      CC_CHT(splash)/GOLDEN_RATIO);
  CC_POS2(menu, wb.cx, wb.cy);
  addItem(menu, 10);

  // audio
  auto n3= "soundOFF.png";
  auto n2= "soundON.png";
  s_arr<c::MenuItem*,2> audios= { cx::reifyMenuBtn(n3,n3), cx::reifyMenuBtn(n2,n2) };
  XCFG()->fit(audios[0]);
  XCFG()->fit(audios[1]);
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

  this->scheduleUpdate();
}

//////////////////////////////////////////////////////////////////////////////
//
void MMenu::update(float ) {
  _bgLayer->sync();
}



NS_END



