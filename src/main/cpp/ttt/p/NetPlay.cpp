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
#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "core/JSON.h"
#include "NetPlay.h"
#include "Game.h"
#include "MMenu.h"
#include "s/utils.h"

NS_ALIAS(cx, fusii::ccsx)
NS_ALIAS(ws, fusii::odin)
NS_BEGIN(tttoe)

static int USER_TAG = (int) 'u';
static int PSWD_TAG = (int) 'p';

//////////////////////////////////////////////////////////////////////////////
//
void NetPlay::showWaitOthers() {

  auto qn= cx::reifyBmfLabel("font.OCR", gets("waitother"));
  auto wz= cx::visRect();
  auto cw= cx::center();
  auto wb = cx::visBox();

  removeAll();

  qn->setScale(XCFG()->getScale() * 0.3f);
  qn->setPosition(cw.x, wb.top * 0.75f);
  addItem(qn);

  auto f= []() { cx::runEx(XCFG()->prelude()); };
  auto b1= cx::reifyMenuBtn("cancel.png");
  auto menu = cx::mkMenu(b1);

  menu->setPosition(cw.x, wb.top * 0.1f);
  b1->setCallback(
      [=](c::Ref*) { this->onCancel(); });
  addItem(menu);
}

//////////////////////////////////////////////////////////////////////////////
//
void NetPlay::onStart(ws::OdinEvent *evt) {
  auto obj= fmtGameData( f::GMode::NET);
  auto ctx = SCAST(NPCX*, getCtx());
  auto io=odin;

  obj["ppids"] = evt->doco["source"]["ppids"];
  obj["pnum"]= j::json(player);
  SNPTR(odin)
  ctx->yes(io,obj);
}

//////////////////////////////////////////////////////////////////////////////
//
void NetPlay::onCancel() {
  auto f= []() { cx::runEx(XCFG()->prelude()); };
  auto m = MMenu::reify(mc_new_1(MCX, f));
  ws::disconnect(odin);
  mc_del_ptr(odin)
  cx::runEx(m);
}

//////////////////////////////////////////////////////////////////////////////
//
void NetPlay::onPlayReply(ws::OdinEvent *evt) {
  player= JS_INT(  evt->doco["pnum"]);
  CCLOG("player %d: ok", player);
  showWaitOthers();
}

//////////////////////////////////////////////////////////////////////////////
//
void NetPlay::networkEvent(ws::OdinEvent *evt) {
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
void NetPlay::sessionEvent(ws::OdinEvent *evt) {
  switch (evt->code) {
    case ws::EType::PLAYREQ_OK:
      onPlayReply(evt);
    break;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void NetPlay::odinEvent(ws::OdinEvent *evt) {
  //CCLOG("odin event = %p", evt);
  switch (evt->type) {
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
    odin= ws::reifyPlayRequest(
        XCFG()->getGameId(),
        uid, pwd);
    odin->listen([=](ws::OdinEvent *e) {
        this->odinEvent(e);
        });
    ws::connect(odin, XCFG()->getWSUrl());
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void NetPlay::decorate() {

  auto qn= cx::reifyBmfLabel("font.OCR", gets("signinplay"));
  auto wz= cx::visRect();
  auto cw= cx::center();
  auto wb= cx::visBox();

  centerImage(this,"game.bg");
  incIndexZ();

  // text msg
  qn->setScale(XCFG()->getScale() * 0.3f);
  qn->setPosition(cw.x, wb.top * 0.75f);
  qn->setOpacity(0.9f*255);
  addItem(this,qn);

  // editbox for user
  auto uid = c::ui::TextField::create();
  auto bxz = cx::calcSize("ok.png");
  uid->setMaxLengthEnabled(true);
  uid->setMaxLength(16);
  uid->setTouchEnabled(true);
  uid->setFontName( "Arial");
  uid->setFontSize( 18);
  uid->setPlaceHolder(gets("userid"));
  uid->setPosition(c::Vec2(cw.x, cw.y+bxz.height*0.5f+2));
  addItem(this,uid, lastZ, USER_TAG);

  // editbox for password
  auto pwd = c::ui::TextField::create();
  pwd->setPasswordEnabled(true);
  pwd->setPasswordStyleText("*");
  pwd->setTouchEnabled(true);
  pwd->setMaxLength(16);
  pwd->setFontName( "Arial");
  pwd->setFontSize( 18);
  pwd->setPlaceHolder( gets("passwd"));
  pwd->setPosition(c::Vec2(cw.x, cw.y-bxz.height*0.5f-2));
  addItem(this,pwd, lastZ, PSWD_TAG);

  // btns
  auto b1= cx::reifyMenuBtn("continue.png");
  auto b2= cx::reifyMenuBtn("cancel.png");
  auto menu= cx::mkVMenu(s_vec<c::MenuItem*> {b1, b2});

  menu->setPosition(cw.x, wb.top * 0.1f);
  b1->setCallback([=](c::Ref*) {
      this->onLogin();
      });

  b2->setCallback([=](c::Ref*) {
      this->onCancel();
      });

  addItem(this,menu);
}

//////////////////////////////////////////////////////////////////////////////
//
NetPlay::~NetPlay() {
  ws::disconnect(odin);
  mc_del_ptr(odin)
}


NS_END(tttoe)


