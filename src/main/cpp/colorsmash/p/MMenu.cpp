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
NS_BEGIN(colorsmash)

//////////////////////////////////////////////////////////////////////////////
//
void MMenu::decoUI() {

  auto wz= cx::visRect();
  auto wb= cx::visBox();

  auto bg = c::LayerColor::create(
      c::Color4B(25, 0, 51, 255),
      CC_ZW(wz.size),
      CC_ZH(wz.size));
  addItem(bg);

  auto title = cx::reifyLabel("dft",52, "Options");
  title->setPosition(wb.cx, wb.top * 0.75);
  addItem(title);

  auto b1 = c::MenuItemLabel::create(
      cx::reifyLabel("dft",32,"Restart"),
      [=](c::Ref*) {
      cx::pop();
      cx::runEx(Game::reify( new f::GCX() ));
      });
  auto b2 = c::MenuItemLabel::create(
      cx::reifyLabel("dft",32,"Back"),
      [=](c::Ref*) {
        if (XCFG()->hasAudio()) {
          cx::resumeMusic();
        }
        cx::pop();
      });
  auto b3 = c::MenuItemLabel::create(
      cx::reifyLabel("dft",32,"Quit"),
      [=](c::Ref*) {
        cx::pop();
        cx::runEx(Splash::reify());
      });
  auto menu= cx::mkVMenu(s_vec<c::MenuItem*>{
      b1,b2,b3
      }, CC_ZW(CC_CSIZE(b1))/4);
  menu->setPosition(wb.cx, wb.cy);
  addItem(menu);

  // audio
  auto audios= cx::reifyAudioIcons();
  auto sz= CC_CSIZE(audios[0]);
  auto gap= CC_ZW(sz)/4;
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



