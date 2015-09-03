// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2015, Ken Leung. All rights reserved.

#include "MsgBox.h"
USING_NS_CC;

//////////////////////////////////////////////////////////////////////////////
//
NS_FI_BEGIN


MsgBox::Setup() {
  auto qn= Label::createWithBMFont("font.OCR", "some-message");
  auto cw= CCSX::Center();
  auto wz= CCSX::VisRect();
  auto wb = CCSX::VisBox();

  CenterImage("game.bg");
  qn->setPosition(cw.x, wb.top * 0.75);
  qn->setScale(xcfg.game.scale * 0.25);
  qn->setOpacity(0.9*255);
  AddItem(qn);

  auto b1= CreateMenuButton("#ok.png",
      "#ok.png",
      "#ok.png",
      CC_CALLBACK_1(MsgBox::OnYes, this));
  auto menu= Menu::create();
  menu->addChild(b1);
  menu->setPosition(cw.x, wb.top * 0.1);
  AddItem(menu);
}

























NS_FI_END
