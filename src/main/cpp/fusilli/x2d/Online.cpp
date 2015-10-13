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
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Online.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(fusii)


//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL OnlineLayer : public XLayer {
private:

  NO__CPYASS(OnlineLayer)
  OnlineLayer() {}

public:

  virtual int GetIID() { return 1; }
  virtual XLayer* Realize();
  virtual ~OnlineLayer() {}

  void OnReq(const stdstr&, const stdstr&);
  void ShowWaitOthers();

  CREATE_FUNC(OnlineLayer)
};

//////////////////////////////////////////////////////////////////////////////
//
void OnlineLayer::OnReq(const stdstr& uid, const stdstr& pwd) {
  auto par = SCAST(Online*, getParent());
  par->OnPlayReq(uid, pwd);
}

//////////////////////////////////////////////////////////////////////////////
//
void OnlineLayer::ShowWaitOthers() {

  auto qn= cx::ReifyBmfLabel("font.OCR", "waiting...");
  auto wz= cx::VisRect();
  auto cw= cx::Center();
  auto wb = cx::VisBox();

  RemoveAll();

  qn->setPosition(cw.x, wb.top * 0.75);
  qn->setScale(XCFG()->GetScale() * 0.3);
  qn->setOpacity(0.9*255);
  AddItem(qn);

  auto b1= cx::ReifyMenuBtn("cancel.png");
  b1->setTarget(getParent(),
      CC_MENU_SELECTOR(Online::OnCancel));
  auto menu= c::Menu::create();
  menu->addChild(b1);
  menu->setPosition(cw.x, wb.top * 0.1);
  AddItem(menu);
}

//////////////////////////////////////////////////////////////////////////////
//
XLayer* OnlineLayer::Realize() {
  SCAST(Online*, getParent())->Decorate(this);
  return this;
}

//////////////////////////////////////////////////////////////////////////////
//
Online* Online::Reify(not_null<Online*> box, c::CallFunc* yes, c::CallFunc* no) {
  box->SetActions(yes,no);
  box->Realize();
  return box;
}

//////////////////////////////////////////////////////////////////////////////
//
void Online::Decorate(OnlineLayer* layer) {

  auto qn= cx::ReifyBmfLabel("font.OCR", "Sign in");
  auto wz= cx::VisRect();
  auto cw= cx::Center();
  auto wb= cx::VisBox();

  layer->CenterImage("game.bg");
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
  uid->setPlaceHolder( "user id:");
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
  pwd->setPlaceHolder( "password:");
  pwd->setPosition(c::Vec2(cw.x, cw.y - bxz.height * 0.5 - 2));
  layer->AddItem(pwd);

  auto b1= cx::ReifyMenuBtn("continue.png");
  b1->setCallback([=](c::Ref* rr) {
        layer->OnReq(uid->getString(), pwd->getString());
      });

  auto b2= cx::ReifyMenuBtn("cancel.png");
  b2->setTarget(this, CC_MENU_SELECTOR(Online::OnCancel));
  auto menu= c::Menu::create();
  menu->addChild(b1,1);
  menu->addChild(b2,2);
  menu->setPosition(cw.x, wb.top * 0.1);
  layer->AddItem(menu);
}

//////////////////////////////////////////////////////////////////////////////
//
void Online::SetActions(c::CallFunc* yes, c::CallFunc* no) {
  CC_KEEP(yes)
  CC_KEEP(no)
  this->yes= yes;
  this->no = no;
}

//////////////////////////////////////////////////////////////////////////////
//
Online::Online() {
  SNPTR(yes)
  SNPTR(wss)
  SNPTR(no)
}

//////////////////////////////////////////////////////////////////////////////
//
Online::~Online() {
  CC_DROP(yes)
  CC_DROP(no)
}

//////////////////////////////////////////////////////////////////////////////
//
XScene* Online::Realize() {
  auto y= OnlineLayer::create();
  AddLayer(y);
  y->Realize();
  return this;
}

//////////////////////////////////////////////////////////////////////////////
//
void Online::OnPlayReq(const stdstr& uid, const stdstr& pwd) {
  auto wsurl = XCFG()->GetWSUrl();
  auto game = XCFG()->GetGameId();

  if (uid.length() == 0 ||
      pwd.length() == 0) { return; }

  wss= ws::ReifyPlayRequest(game, uid, pwd);
  wss->Listen(ws::MType::EVERYTHING, [=](const ws::Event& e) {
      this->OnOdinEvent(e);
      });
  ws::Connect(wss,wsurl);
}

//////////////////////////////////////////////////////////////////////////////
//
void Online::OnOdinEvent(const ws::Event& evt) {
  //CCLOG("odin event = %p", evt);
  switch (evt.type) {
    case ws::MType::NETWORK: OnNetworkEvent(evt); break;
    case ws::MType::SESSION: OnSessionEvent(evt); break;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Online::OnNetworkEvent(const ws::Event& evt) {
  switch (evt.code) {
    case ws::EType::PLAYER_JOINED:
      //TODO
      //CCLOG("another player joined room: ", evt.source.puid);
    break;
    case ws::EType::START:
      CCLOG("play room is ready, game can start");
      wss->CancelAll();
      OnContinue();
    break;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Online::OnContinue() {
      // flip to game scene
   //OnContinue(this.wss, this.player, evt.source);
}

//////////////////////////////////////////////////////////////////////////////
//
void Online::OnSessionEvent(const ws::Event& evt) {
  switch (evt.code) {
    case ws::EType::PLAYREQ_OK:
      //CCLOG("player %d: request to play game was successful",evt.source.pnum);
      //player=evt.source.pnum;
      SCAST(OnlineLayer*, GetLayer(1))->ShowWaitOthers();
    break;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Online::OnCancel(c::Ref* rr) {
  try {
    ws::Close(wss);
  } catch (...) {}
  SNPTR(wss)
  no->execute();
}


NS_END(fusii)

