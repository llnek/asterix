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

#include "dropbox/json11.hpp"
#include "x2d/MainGame.h"
#include "ui/UITextField.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "NetPlay.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
//
void NetPlay::DecoUI(f::OnlineLayer* layer) {

  auto qn= cx::ReifyBmfLabel("font.OCR", XCFG()->GetL10NStr("signinplay"));
  auto wz= cx::VisRect();
  auto cw= cx::Center();
  auto wb= cx::VisBox();

  layer->CenterImage("game.bg");

  // test msg
  qn->setPosition(cw.x, wb.top * 0.75);
  qn->setScale(XCFG()->GetScale() * 0.3);
  qn->setOpacity(0.9*255);
  layer->AddItem(qn);

  // editbox for user
  auto uid = c::ui::TextField::create();
  auto bxz = cx::CalcSize("ok.png");
  uid->setMaxLengthEnabled(true);
  uid->setMaxLength(16);
  uid->setTouchEnabled(true);
  uid->setFontName( "Arial");
  uid->setFontSize( 18);
  uid->setPlaceHolder(XCFG()->GetL10NStr("userid"));
  uid->setPosition(c::Vec2(cw.x, cw.y + bxz.height * 0.5 + 2));
  layer->AddItem(uid);

  // editbox for password
  auto pwd = c::ui::TextField::create();
  pwd->setPasswordEnabled(true);
  pwd->setPasswordStyleText("*");
  pwd->setTouchEnabled(true);
  pwd->setMaxLength(16);
  pwd->setFontName( "Arial");
  pwd->setFontSize( 18);
  pwd->setPlaceHolder( XCFG()->GetL10NStr("passwd"));
  pwd->setPosition(c::Vec2(cw.x, cw.y - bxz.height * 0.5 - 2));
  layer->AddItem(pwd);

  // btns
  auto b1= cx::ReifyMenuBtn("continue.png");
  auto yy= (f::OnlineLayer*) layer;
  b1->setCallback([=](c::Ref*) {
        yy->Login(uid->getString(), pwd->getString());
      });

  auto b2= cx::ReifyMenuBtn("cancel.png");
  b2->setTarget(this,
      CC_MENU_SELECTOR(Online::OnCancel));

  auto menu= f::ReifyRefType<cocos2d::Menu>();
  menu->addChild(b1);
  menu->addChild(b2);
  menu->setPosition(cw.x, wb.top * 0.1);
  layer->AddItem(menu);
}

//////////////////////////////////////////////////////////////////////////////
//
void NetPlay::ShowWaitOthers(f::OnlineLayer* layer) {

  auto qn= cx::ReifyBmfLabel("font.OCR", XCFG()->GetL10NStr("waitother"));
  auto wz= cx::VisRect();
  auto cw= cx::Center();
  auto wb = cx::VisBox();

  qn->setScale(XCFG()->GetScale() * 0.3);
  qn->setPosition(cw.x, wb.top * 0.75);
  qn->setOpacity(0.9*255);
  layer->AddItem(qn);

  auto menu= f::ReifyRefType<cocos2d::Menu>();
  auto b1= cx::ReifyMenuBtn("cancel.png");

  b1->setTarget(getParent(),
      CC_MENU_SELECTOR(f::Online::OnCancel));
  menu->addChild(b1);
  menu->setPosition(cw.x, wb.top * 0.1);
  layer->AddItem(menu);
}

//////////////////////////////////////////////////////////////////////////////
//
void NetPlay::OnStart(const ws::Event& evt) {
  auto s= XCFG()->GetSeedData();
  s["ppids"] = evt.doco["source"]["ppids"];
  s["pnum"]= player;

  MGMS()->SetOnlineChannel(odin);
  SNPTR(odin)
  yes->execute();
}

//////////////////////////////////////////////////////////////////////////////
//
void NetPlay::OnPlayReply(const ws::Event& evt) {
  player= evt.doco["pnum"].int_value();
  CCLOG("player %d: ok", player);
}



NS_END(tttoe)

