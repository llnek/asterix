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

BEGIN_NS_UNAMED()
class CC_DLL UILayer : f::XLayer {
protected:

  void onCancel(c::Ref* );

public:

  virtual f::XLayer* realize();
  ws::OdinIO* odin;

  NO__CPYASS(UILayer)
  DECL_CTOR(UILayer)
}

END_NS_UNAMED()

//////////////////////////////////////////////////////////////////////////////
//
void UILayer::onCancel(c::Ref* ) {
  try { ws::close(odin); } catch (...) {}
  SNPTR(odin)
  cx::runScene(MainMenu::reifyWithBackAction());
}

//////////////////////////////////////////////////////////////////////////////
//
void UILayer::odinEvent(ws::odinEvent* evt) {
  //CCLOG("odin event = %p", evt);
  switch (evt->type) {
    case ws::MType::NETWORK:
      networkEvent(evt);
    break;
    case ws::MType::SESSION:
      sessionEvent(evt);
    break;
  }
  evt->release();
}

//////////////////////////////////////////////////////////////////////////////
//
void UILayer::networkEvent(ws::OdinEvent* evt) {
  switch (evt->code) {
    case ws::EType::PLAYER_JOINED:
      //TODO
      //CCLOG("another player joined room: ", evt.source.puid);
    break;
    case ws::EType::START:
      CCLOG("play room is ready, game can start");
      odin->cancelAll();
      onStart(evt);
    break;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void UILayer::onPlayReply(ws::OdinEvent* evt) {
  player= evt->doco["pnum"].int_value();
  CCLOG("player %d: ok", player);
  showWaitOthers();
}

//////////////////////////////////////////////////////////////////////////////
//
void UILayer::sessionEvent(ws::OdinEvent* evt) {
  switch (evt->code) {
    case ws::EType::PLAYREQ_OK:
      onPlayReply(evt);
    break;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void UILayer::onLogin(c::Ref* ) {
  auto u= (c::ui::TextField*) getChildByTag( (int) 'u');
  auto p= (c::ui::TextField*) getChildByTag( (int) 'p');
  auto uid = u->getString();
  auto pwd= p->getString();

  //TODO: fix url
  auto wsurl = XCFG()->getWSUrl();
  auto game = XCFG()->getGameId();

  if (uid.length() > 0 && pwd.length() > 0) {
    odin= ws::reifyPlayRequest(game, uid, pwd);
    odin->listen([=](ws::OdinEvent* e) {
        this->odinEvent(e);
        });
    ws::connect(odin, wsurl);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
f::XLayer* UILayer::realize() {

  auto qn= cx::reifyBmfLabel("font.OCR", XCFG()->getL10NStr("signinplay"));
  auto wz= cx::visRect();
  auto cw= cx::center();
  auto wb= cx::visBox();
  int tag;

  centerImage("game.bg");

  // test msg
  qn->setScale(XCFG()->getScale() * 0.3);
  qn->setPosition(cw.x, wb.top * 0.75);
  qn->setOpacity(0.9*255);
  addItem(qn);

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
  tag= (int)'u';
  addItem(uid, Option<int>(), Option<int>(tag));

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
  tag= (int) 'p';
  addItem(pwd, Option<int>(), Option<int>(tag));

  // btns
  auto b1= cx::reifyMenuBtn("continue.png");
  b1->setTarget(this,
      CC_MENU_SELECTOR(UILayer::onLogin));

  auto b2= cx::reifyMenuBtn("cancel.png");
  b2->setTarget(this,
      CC_MENU_SELECTOR(UILayer::onCancel));

  auto menu= cx::mkMenu(s::vector<c::MenuItem*> {b1, b2}, true, 10.0);
  menu->setPosition(cw.x, wb.top * 0.1);
  tag= 117;
  addItem(menu, Option<int>(), Option<int>(tag));
}

//////////////////////////////////////////////////////////////////////////////
//
void UILayer::showWaitOthers() {

  auto qn= cx::reifyBmfLabel("font.OCR", XCFG()->getL10NStr("waitother"));
  auto wz= cx::visRect();
  auto cw= cx::center();
  auto wb = cx::visBox();

  removeAll();

  qn->setScale(XCFG()->getScale() * 0.3);
  qn->setPosition(cw.x, wb.top * 0.75);
  qn->setOpacity(0.9*255);
  addItem(qn);

  auto menu= f::ReifyRefType<cocos2d::Menu>();
  auto b1= cx::reifyMenuBtn("cancel.png");
  b1->setTarget(this,
      CC_MENU_SELECTOR(::onCancel));
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



NS_END(tttoe)

