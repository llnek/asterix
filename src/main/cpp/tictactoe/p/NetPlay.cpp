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

#include "ui/UITextField.h"
#include "dbox/json11.hpp"
#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "NetPlay.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
//
void NetPlay::decoUI(f::OnlineLayer* layer) {

  auto qn= cx::reifyBmfLabel("font.OCR", XCFG()->getL10NStr("signinplay"));
  auto wz= cx::visRect();
  auto cw= cx::center();
  auto wb= cx::visBox();

  layer->centerImage("game.bg");

  // test msg
  qn->setScale(XCFG()->getScale() * 0.3);
  qn->setPosition(cw.x, wb.top * 0.75);
  qn->setOpacity(0.9*255);
  layer->addItem(qn);

  // editbox for user
  auto uid = c::ui::TextField::create();
  auto bxz = cx::calcSize("ok.png");
  uid->setMaxLengthEnabled(true);
  uid->setMaxLength(16);
  uid->setTouchEnabled(true);
  uid->setFontName( "Arial");
  uid->setFontSize( 18);
  uid->setPlaceHolder(XCFG()->getL10NStr("userid"));
  uid->setPosition(c::Vec2(cw.x, cw.y + bxz.height * 0.5 + 2));
  layer->addItem(uid);

  // editbox for password
  auto pwd = c::ui::TextField::create();
  pwd->setPasswordEnabled(true);
  pwd->setPasswordStyleText("*");
  pwd->setTouchEnabled(true);
  pwd->setMaxLength(16);
  pwd->setFontName( "Arial");
  pwd->setFontSize( 18);
  pwd->setPlaceHolder( XCFG()->getL10NStr("passwd"));
  pwd->setPosition(c::Vec2(cw.x, cw.y - bxz.height * 0.5 - 2));
  layer->addItem(pwd);

  // btns
  auto b1= cx::reifyMenuBtn("continue.png");
  b1->setCallback([=](c::Ref*) {
        layer->login(uid->getString(), pwd->getString());
      });

  auto b2= cx::reifyMenuBtn("cancel.png");
  b2->setTarget(this,
      CC_MENU_SELECTOR(f::Online::onCancel));

  auto menu= cx::mkMenu(s::vector<c::MenuItem*> {b1, b2}, true, 10.0);
  menu->setPosition(cw.x, wb.top * 0.1);
  layer->addItem(menu);
}

//////////////////////////////////////////////////////////////////////////////
//
void NetPlay::showWaitOthers(f::OnlineLayer* layer) {

  auto qn= cx::reifyBmfLabel("font.OCR", XCFG()->getL10NStr("waitother"));
  auto wz= cx::visRect();
  auto cw= cx::center();
  auto wb = cx::visBox();

  qn->setScale(XCFG()->getScale() * 0.3);
  qn->setPosition(cw.x, wb.top * 0.75);
  qn->setOpacity(0.9*255);
  layer->addItem(qn);

  auto menu= f::ReifyRefType<cocos2d::Menu>();
  auto b1= cx::reifyMenuBtn("cancel.png");
  b1->setTarget(this,
      CC_MENU_SELECTOR(f::Online::onCancel));
  menu->addChild(b1);
  menu->setPosition(cw.x, wb.top * 0.1);
  layer->addItem(menu);
}

//////////////////////////////////////////////////////////////////////////////
//
void NetPlay::onStart(ws::OdinEvent* evt) {
  auto s= XCFG()->getSeedData();
  s["ppids"] = evt->doco["source"]["ppids"];
  s["pnum"]= player;

  MGMS()->setOnlineChannel(odin);
  SNPTR(odin)
  yes->execute();
}

//////////////////////////////////////////////////////////////////////////////
//
void NetPlay::onPlayReply(ws::OdinEvent* evt) {
  player= evt->doco["pnum"].int_value();
  CCLOG("player %d: ok", player);
}



NS_END(tttoe)

