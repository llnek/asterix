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

#include "ui/UITextField.h"
#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "core/JSON.h"
#include "NetPlay.h"
#include "Game.h"
#include "MMenu.h"
#include "n/lib.h"

NS_ALIAS(cx, fusii::ccsx)
NS_ALIAS(ws, fusii::odin)
NS_BEGIN(pong)

static int USERTAG= (int) 'u';
static int PASSTAG= (int) 'p';

//////////////////////////////////////////////////////////////////////////////
//
void NetPlay::showWaitOthers() {

  auto qn= cx::reifyBmfLabel("OCR", gets("waitother"));
  auto b1= cx::reifyMenuBtn("cancel.png");
  auto menu = cx::mkMenu(b1);
  auto wb = cx::visBox();

  removeAll();

  qn->setScale(XCFG()->getScale() * 0.3);
  qn->setPosition(wb.cx, wb.top * 0.75);
  addItem(qn);

  b1->setCallback(
      [=](c::Ref*) { this->onCancel(); });

  b1->setPosition(wb.cx, wb.top * 0.1);
  addItem(menu);
}

//////////////////////////////////////////////////////////////////////////////
//
void NetPlay::onStart(ws::OdinEvent *evt) {

  auto obj= fmtGameData(f::GMode::NET);
  auto ctx = getCtx();

  obj["ppids"] = evt->_doco["source"]["ppids"];
  obj["pnum"]= _player;

  SCAST(NPCX*, ctx)->yes(_odin,obj);
  S__NIL(_odin)
}

//////////////////////////////////////////////////////////////////////////////
//
void NetPlay::onCancel() {
  auto f= []() { cx::prelude(); };
  ws::disconnect(_odin);
  mc_del_ptr(_odin);
  cx::runEx( MMenu::reify(mc_new1(MCX, f)));
}

//////////////////////////////////////////////////////////////////////////////
//
void NetPlay::onPlayReply(ws::OdinEvent *evt) {
  _player= JS_INT( evt->_doco["pnum"]);
  assert(_player > 0);
  CCLOG("player %d: ok", _player);
  showWaitOthers();
}

//////////////////////////////////////////////////////////////////////////////
//
void NetPlay::networkEvent(ws::OdinEvent *evt) {
  switch (evt->_code) {
    case ws::EType::PLAYER_JOINED:
      //TODO
      //CCLOG("another player joined room: ", evt.source.puid);
    break;
    case ws::EType::START:
      CCLOG("play room is ready, game can start");
      _odin->cancelAll();
      onStart(evt);
    break;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void NetPlay::sessionEvent(ws::OdinEvent *evt) {
  switch (evt->_code) {
    case ws::EType::PLAYREQ_OK:
      onPlayReply(evt);
    break;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void NetPlay::odinEvent(ws::OdinEvent *evt) {
  CCLOG("odin event = %s", evt->_doco.dump(2).c_str());
  switch (evt->_type) {
    case ws::MType::NETWORK:
      networkEvent(evt);
    break;
    case ws::MType::SESSION:
      sessionEvent(evt);
    break;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void NetPlay::onLogin() {
  auto u= (c::ui::TextField*) getChildByTag( USERTAG);
  auto p= (c::ui::TextField*) getChildByTag( PASSTAG);
  auto uid= u->getString();
  auto pwd= p->getString();

  if (uid.length() > 0 && pwd.length() > 0) {
    _odin= ws::reifyPlayRequest(
        XCFG()->getGameId(),
        uid, pwd);
    _odin->listen([=](ws::OdinEvent *e) {
        this->odinEvent(e);
        });
    ws::connect(_odin, XCFG()->getWSUrl());
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void NetPlay::decoUI() {
  auto qn= cx::reifyBmfLabel("OCR", gets("signinplay"));
  auto wb= cx::visBox();

  centerImage("gui.mmenu.menu.bg");

  // text msg
  qn->setScale(XCFG()->getScale() * 0.3);
  qn->setPosition(wb.cx, wb.top * 0.75);
  addItem(qn);

  // editbox for user
  auto uid = c::ui::TextField::create();
  auto bxz = cx::calcSize("ok.png");
  uid->setMaxLengthEnabled(true);
  uid->setMaxLength(16);
  uid->setTouchEnabled(true);
  uid->setFontName( "Arial");
  uid->setFontSize( 18);
  uid->setPlaceHolder(gets("userid"));
  uid->setPosition(c::Vec2(wb.cx, wb.cy+HHZ(bxz)+2));
  addItem(uid, 0, USERTAG);

  // editbox for password
  auto pwd = c::ui::TextField::create();
  pwd->setPasswordEnabled(true);
  pwd->setPasswordStyleText("*");
  pwd->setTouchEnabled(true);
  pwd->setMaxLength(16);
  pwd->setFontName( "Arial");
  pwd->setFontSize( 18);
  pwd->setPlaceHolder( gets("passwd"));
  pwd->setPosition(c::Vec2(wb.cx, wb.cy-HHZ(bxz)-2));
  addItem(pwd, 0, PASSTAG);

  // btns
  auto b1= cx::reifyMenuBtn("continue.png");
  auto b2= cx::reifyMenuBtn("cancel.png");
  s::vector<c::MenuItem*> btns {b1, b2};
  auto menu= cx::mkVMenu(btns);

  b1->setCallback(
      [=](c::Ref*) { this->onLogin(); });

  b2->setCallback(
      [=](c::Ref*) { this->onCancel(); });

  menu->setPosition(wb.cx, wb.top * 0.1);
  addItem(menu);
}



NS_END


