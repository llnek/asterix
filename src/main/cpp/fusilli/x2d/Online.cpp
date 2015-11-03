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

#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Online.h"
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////////
//
void OnlineLayer::Login(const stdstr& uid, const stdstr& pwd) {
  CC_PCAST(Online*)->OnPlayRequest(uid, pwd);
}

//////////////////////////////////////////////////////////////////////////////
//
XLayer* OnlineLayer::Realize() {
  CC_PCAST(Online*)->DecoUI(this);
  return this;
}

//////////////////////////////////////////////////////////////////////////////
//
Online* Online::Reify(not_null<Online*> box, VOIDFN yes, VOIDFN no) {
  box->SetActions(c::CallFunc::create(yes),
      c::CallFunc::create(no));
  box->Realize();
  return box;
}

//////////////////////////////////////////////////////////////////////////////
//
Online::Online() {
  SNPTR(odin)
  SNPTR(yes)
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
  auto y= (XLayer*) ReifyRefType<OnlineLayer>();
  AddLayer(y);
  y->Realize();
  return this;
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
void Online::OnPlayRequest(const stdstr& uid, const stdstr& pwd) {
  //TODO: fix url
  auto wsurl = XCFG()->GetWSUrl();
  auto game = XCFG()->GetGameId();

  if (uid.length() == 0 ||
      pwd.length() == 0) { return; }

  odin= ws::ReifyPlayRequest(game, uid, pwd);
  odin->Listen([=](const ws::Event& e) {
      this->OnOdinEvent(e);
      });
  ws::Connect(odin,wsurl);
}

//////////////////////////////////////////////////////////////////////////////
//
void Online::OnOdinEvent(const ws::Event& evt) {
  //CCLOG("odin event = %p", evt);
  switch (evt.type) {
    case ws::MType::NETWORK:
      OnNetworkEvent(evt);
    break;
    case ws::MType::SESSION:
      OnSessionEvent(evt);
    break;
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
      odin->CancelAll();
      OnStart(evt);
    break;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Online::OnSessionEvent(const ws::Event& evt) {
  switch (evt.code) {
    case ws::EType::PLAYREQ_OK:
      auto y= SCAST(OnlineLayer*, GetLayer(1));
      OnPlayReply(evt);
      y->RemoveAll();
      ShowWaitOthers(y);
    break;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Online::OnCancel(c::Ref* rr) {
  try {
    ws::Close(odin);
  } catch (...) {}
  SNPTR(odin)
  no->execute();
}





NS_END(fusii)

