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

#include "ui/UITextField.h"
#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "core/JSON.h"
#include "NetPlay.h"
#include "Game.h"
#include "MMenu.h"
#include "n/C.h"

NS_ALIAS(cx, fusii::ccsx)
NS_ALIAS(ws, fusii::odin)
NS_BEGIN(tttoe)

static int USER_TAG = (int) 'u';
static int PSWD_TAG = (int) 'p';

//////////////////////////////////////////////////////////////////////////////
//
void NetPlay::showWaitOthers() {

  auto qn= cx::reifyLabel("text", 32, gets("waitother"));
  auto wb = cx::visBox();

  //clear layer
  removeChildByTag(USER_TAG);
  removeChildByTag(PSWD_TAG);
  removeChildByTag(911);

  qn->setAlignment(c::TextHAlignment::CENTER);
  qn->setPosition(wb.cx, wb.top * 0.8);
  addItem(qn);

  auto b1= cx::reifyMenuBtn("cancel.png");
  auto menu = cx::mkMenu(b1);

  menu->setPosition(wb.cx, wb.top * 0.2);
  b1->setCallback(
      [=](c::Ref*) { this->onCancel(); });
  addItem(menu);
}

//////////////////////////////////////////////////////////////////////////////
//
void NetPlay::onStart(ws::OdinEvent *evt) {
  auto obj= fmtGameData( f::GMode::NET);
  auto io= _odin;

  obj["ppids"] = evt->getDoco()["source"]["ppids"];
  obj["pnum"]= _player; //j::json(player);

  io->cancelAll();
  S__NIL( _odin)
  SCAST(NPCX*, getCtx())->_yes(io,obj);
}

//////////////////////////////////////////////////////////////////////////////
//
void NetPlay::onCancel() {
  auto f= []() { cx::prelude(); };
  cx::runEx(
      MMenu::reify(
        mc_new1(MCX, f)));
}

//////////////////////////////////////////////////////////////////////////////
//
void NetPlay::onPlayReply(ws::OdinEvent *evt) {
  _player= JS_INT( evt->getDoco()["pnum"]);
  CCLOG("player %d: ok", _player);
  showWaitOthers();
}

//////////////////////////////////////////////////////////////////////////////
//
void NetPlay::networkEvent(ws::OdinEvent *evt) {
  switch (evt->getCode()) {
    case ws::EType::PLAYER_JOINED:
      //CCLOG("another player joined room: ", evt.source.puid);
    break;
    case ws::EType::START:
      CCLOG("play room is ready, game can start");
      onStart(evt);
    break;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void NetPlay::sessionEvent(ws::OdinEvent *evt) {
  switch (evt->getCode()) {
    case ws::EType::PLAYREQ_OK:
      onPlayReply(evt);
    break;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void NetPlay::odinEvent(ws::OdinEvent *evt) {
  //CCLOG("odin event = %p", evt);
  switch (evt->getType()) {
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
  auto u= (c::ui::TextField*) getChildByTag( USER_TAG);
  auto p= (c::ui::TextField*) getChildByTag( PSWD_TAG);
  auto uid = u->getString();
  auto pwd = p->getString();

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

  auto qn= cx::reifyLabel("text",
      32,
      gets("signinplay"));
  auto wb= cx::visBox();

  centerImage("game.bg");

  // text msg
  qn->setPosition(wb.cx, wb.top * 0.8);
  addItem(qn);

  // editbox for user
  auto uid = c::ui::TextField::create();
  auto bxz = cx::calcSize("ok.png");
  uid->setMaxLengthEnabled(true);
  uid->setMaxLength(16);
  uid->setTouchEnabled(true);
  uid->setFontName( "Arial");
  uid->setFontSize( 18);
  uid->setPlaceHolderColor(c::Color4B::WHITE);
  uid->setPlaceHolder(gets("userid"));
  uid->setPosition(CCT_PT(wb.cx, wb.cy+ HHZ(bxz)+2));
  uid->setTag(USER_TAG);
  uid->setTextColor(c::Color4B::WHITE);
  addItem(uid);

  // editbox for password
  auto pwd = c::ui::TextField::create();
  pwd->setPasswordEnabled(true);
  pwd->setPasswordStyleText("*");
  pwd->setTouchEnabled(true);
  pwd->setMaxLength(16);
  pwd->setFontName( "Arial");
  pwd->setFontSize( 18);
  pwd->setPlaceHolderColor(c::Color4B::WHITE);
  pwd->setPlaceHolder( gets("passwd"));
  pwd->setPosition(CCT_PT(wb.cx, wb.cy- HHZ(bxz)-2));
  pwd->setTag(PSWD_TAG);
  pwd->setTextColor(c::Color4B::WHITE);
  addItem(pwd);

  // btns
  auto b1= cx::reifyMenuBtn("continue.png");
  auto b2= cx::reifyMenuBtn("cancel.png");
  s_vec<c::MenuItem*> btns {b1, b2};
  auto menu= cx::mkVMenu(btns);

  menu->setPosition(wb.cx, wb.top * 0.2);
  b1->setCallback(
      [=](c::Ref*) { this->onLogin(); });

  b2->setCallback(
      [=](c::Ref*) { this->onCancel(); });

  menu->setTag(911);
  addItem(menu);
}

//////////////////////////////////////////////////////////////////////////////
//
NetPlay::~NetPlay() {
  ws::disconnect(_odin);
  mc_del_ptr(_odin);
}


NS_END


