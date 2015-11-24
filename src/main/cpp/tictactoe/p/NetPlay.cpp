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
#include "nlohmann/json.hpp"
#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "NetPlay.h"
#include "Game.h"
#include "Menu.h"
#include "s/utils.h"
NS_ALIAS(cx, fusii::ccsx)
NS_ALIAS(ws, fusii::odin)
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
//
BEGIN_NS_UNAMED()
class CC_DLL UILayer : public f::XLayer {
public:

  void networkEvent(ws::OdinEvent*);
  void sessionEvent(ws::OdinEvent*);
  void odinEvent(ws::OdinEvent*);

  void onCancel(c::Ref* );
  void onLogin(c::Ref*);

  void onPlayReply(ws::OdinEvent*);
  void showWaitOthers();
  void onStart(ws::OdinEvent*);

  virtual f::XLayer* realize();
  ws::OdinIO* odin;
  int player;

  NO__CPYASS(UILayer)
  DECL_CTOR(UILayer)

};

//////////////////////////////////////////////////////////////////////////////
//
UILayer::~UILayer() {
  ws::disconnect(odin);
}

//////////////////////////////////////////////////////////////////////////////
//
UILayer::UILayer() {
  player=0;
  SNPTR(odin);
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

  auto b1= cx::reifyMenuBtn("cancel.png");
  auto menu = cx::mkMenu(b1);
  b1->setTarget(this, CC_MENU_SELECTOR(UILayer::onCancel));
  menu->setPosition(cw.x, wb.top * 0.1);
  addItem(menu);
}

//////////////////////////////////////////////////////////////////////////////
//
void UILayer::onStart(ws::OdinEvent* evt) {

  auto s= XCFG()->getSeedData();
  auto mode = f::GMode::NET;

    auto p = s["ppids"] ; p = evt->doco["source"]["ppids"];
    p = s["pnum"]; p= j::json(player);

  auto g = f::reifyRefType<Game>();
  auto io= odin;

  prepareSeedData(mode);
  SNPTR(odin)
  cx::runScene( Game::reify(g, mode, io));
}

//////////////////////////////////////////////////////////////////////////////
//
void UILayer::onCancel(c::Ref* ) {
  ws::disconnect(odin);
  SNPTR(odin)
  auto f= [=]() { cx::runScene(XCFG()->startWith()); };
  auto m = MainMenu::reifyWithBackAction(f);
  cx::runScene( m);
}

//////////////////////////////////////////////////////////////////////////////
//
void UILayer::onPlayReply(ws::OdinEvent* evt) {
  player= evt->doco["pnum"].get<j::json::number_integer_t>();
  CCLOG("player %d: ok", player);
  showWaitOthers();
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
void UILayer::sessionEvent(ws::OdinEvent* evt) {
  switch (evt->code) {
    case ws::EType::PLAYREQ_OK:
      onPlayReply(evt);
    break;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void UILayer::odinEvent(ws::OdinEvent* evt) {
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
  addItem(uid, f::Maybe<int>(), f::Maybe<int>(tag));

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
  addItem(pwd, f::Maybe<int>(), f::Maybe<int>(tag));

  // btns
  auto b1= cx::reifyMenuBtn("continue.png");
  auto b2= cx::reifyMenuBtn("cancel.png");
  auto menu= cx::mkMenu(s::vector<c::MenuItem*> {b1, b2}, true, 10.0);

  b1->setTarget(this,
      CC_MENU_SELECTOR(UILayer::onLogin));

  b2->setTarget(this,
      CC_MENU_SELECTOR(UILayer::onCancel));

  menu->setPosition(cw.x, wb.top * 0.1);
  tag= 117;
  addItem(menu, f::Maybe<int>(), f::Maybe<int>(tag));

  return this;
}

END_NS_UNAMED()

//////////////////////////////////////////////////////////////////////////////
//
f::XScene* NetPlay::realize() {
  auto y = f::reifyRefType<UILayer>();
  addLayer(y);
  y->realize();
  return this;
}

//////////////////////////////////////////////////////////////////////////////
//
NetPlay::~NetPlay() {

}

//////////////////////////////////////////////////////////////////////////////
//
NetPlay::NetPlay() {

}


NS_END(tttoe)

