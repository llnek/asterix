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
NS_ALIAS(cx, fusilli::ccsx)
NS_BEGIN(fusilli)


//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL OnlineLayer : public XLayer {
private:

  void OnReq(const stdstr&, const stdstr&);
  void ShowWaitOthers();
  OnlineLayer() {}

  NO__COPYASSIGN(OnlineLayer)

public:
  virtual int GetIID() { return 1; }
  virtual XLayer* Realize();
  virtual ~OnlineLayer() {}

  CREATE_FUNC(OnlineLayer)
};

//////////////////////////////////////////////////////////////////////////////
//
Online* Online::Create(c::CallFunc* yes, c::CallFunc* no) {
  auto s= Online::create();
  s->SetActions(yes,no);
  s->Realize();
  return s;
}

//////////////////////////////////////////////////////////////////////////////
//
void Online::SetActions(c::CallFunc* yes, c::CallFunc* no) {
  yes->retain();
  no->retain();
  this->yes= yes;
  this->no = no;
}

//////////////////////////////////////////////////////////////////////////////
//
Online::Online()
  : yes(nullptr)
  , wss(nullptr)
  , no(nullptr) {
}

//////////////////////////////////////////////////////////////////////////////
//
Online::~Online() {
   yes->release();
   no->release();
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
  auto cfg = XConfig::GetInstance();
  auto wsurl = cfg->GetWSUrl();
  auto game = cfg->GetGameId();

  if (uid.length() == 0 ||
      pwd.length() == 0) { return; }

  wss= WSockSS::CreatePlayRequest(game, uid, pwd);
  wss->ListenAll([=](const Event& e) {
      this->OnOdinEvent(e);
      });
  wss->Connect(wsurl);
}

//////////////////////////////////////////////////////////////////////////////
//
void Online::OnOdinEvent(const Event& evt) {
  CCLOG("odin event = %p", evt);
  switch (evt.type) {
    case MType::NETWORK: OnNetworkEvent(evt); break;
    case MType::SESSION: OnSessionEvent(evt); break;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Online::OnNetworkEvent(const Event& evt) {
  switch (evt.code) {
    case EType::PLAYER_JOINED:
      //TODO
      //CCLOG("another player joined room: ", evt.source.puid);
    break;
    case EType::START:
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
void Online::OnSessionEvent(const Event& evt) {
  switch (evt.code) {
    case EType::PLAYREQ_OK:
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
    wss->Close();
  } catch (...) {}
  wss=nullptr;
  this->no->execute();
}

//////////////////////////////////////////////////////////////////////////////
//
void OnlineLayer::OnReq(const stdstr& uid, const stdstr& pwd) {
  auto par = SCAST(Online*, getParent());
  par->OnPlayReq(uid, pwd);
}

//////////////////////////////////////////////////////////////////////////////
//
void OnlineLayer::ShowWaitOthers() {
  auto cfg= XConfig::GetInstance();
  auto fnt= cfg->GetFont("font.OCR");
  auto qn= c::Label::createWithBMFont(fnt, "waiting...");
  auto cw= cx::Center();
  auto wz= cx::VisRect();
  auto wb = cx::VisBox();

  RemoveAll();

  qn->setPosition(cw.x, wb.top * 0.75);
  qn->setScale(cfg->GetScale() * 0.3);
  qn->setOpacity(0.9*255);
  AddItem(qn);

  auto b1= cx::CreateMenuBtn("#cancel.png");
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
  auto cfg = XConfig::GetInstance();
  auto fnt= cfg->GetFont("font.OCR");
  auto qn= c::Label::createWithBMFont(fnt, "Sign in");
  auto wz= cx::VisRect();
  auto cw= cx::Center();
  auto wb= cx::VisBox();
  auto par= SCAST(Online*,getParent());

  CenterImage("game.bg");
  qn->setPosition(cw.x, wb.top * 0.75);
  qn->setScale(cfg->GetScale() * 0.3);
  qn->setOpacity(0.9*255);
  AddItem(qn);

  auto bxz = cx::CalcSize("#ok.png");
  // editbox for user
  auto uid = c::ui::TextField::create();
  uid->setMaxLengthEnabled(true);
  uid->setMaxLength(16);
  uid->setTouchEnabled(true);
  uid->setFontName( "Arial");
  uid->setFontSize( 18);
  uid->setPlaceHolder( "user id:");
  uid->setPosition(c::Vec2(cw.x, cw.y + bxz.height * 0.5 + 2));
  AddItem(uid);

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
  AddItem(pwd);

  auto b1= cx::CreateMenuBtn("#continue.png");
  b1->setCallback([=](c::Ref* rr) {
        this->OnReq(uid->getString(), pwd->getString());
      });

  auto b2= cx::CreateMenuBtn("#cancel.png");
  b2->setTarget(par, CC_MENU_SELECTOR(Online::OnCancel));
  auto menu= c::Menu::create();
  menu->addChild(b1,1);
  menu->addChild(b2,2);
  menu->setPosition(cw.x, wb.top * 0.1);
  AddItem(menu);
  return this;
}



NS_END(fusilli)

