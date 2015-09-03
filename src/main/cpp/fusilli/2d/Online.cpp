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

#include "Online.h"
USING_NS_CC;

//////////////////////////////////////////////////////////////////////////////
//
NS_FI_BEGIN

void Online::OnOnlineReq(const string& uid, const string& pwd) {
  auto wsurl = "ws://network/blah";

  if (uid.length == 0 ||
      pwd.length == 0) { return; }

  m_wss= ODIN::ReifySession(gameid, uid, pwd);
  m_wss->ListenAll(this.onOdinEvent, this);
  m_wss->Connect(wsurl);
}

/**
 * @method onOdinEvent
 * @private
 */
void Online::OnOdinEvent(const Event& evt) {
  //sjs.loggr.debug(evt);
  switch (evt.type) {
    case MSG_NETWORK: OnNetworkEvent(evt); break;
    case MSG_SESSION: OnSessionEvent(evt); break;
  }
}

/**
 * @method onNetworkEvent
 * @private
 */
void Online::OnNetworkEvent(const Event& evt) {
  switch (evt.code) {
    case PLAYER_JOINED:
      //TODO
      sjs.loggr.debug("another player joined room. " + evt.source.puid);
    break;
    case START:
      sjs.loggr.info("play room is ready, game can start.");
      m_wss->CancelAll();
      // flip to game scene
      this.options.yes(this.wss, this.player, evt.source || {});
    break;
  }
}

/**
 * @method onSessionEvent
 * @private
 */
void Online::OnSessionEvent(const Event& evt) {
  switch (evt.code) {
    case PLAYREQ_OK:
      sjs.loggr.debug("player " +
                      evt.source.pnum +
                      ": request to play game was successful.");
      this.player=evt.source.pnum;
      ShowWaitOthers();
    break;
  }
}

/**
 * @method onCancelPlay
 * @private
 */
void Online::OnCancelPlay() {
  try {
    m_wss->Close();
  } catch (Throwable e) {}
  m_wss=nullptr;
  this.options.onback();
}

/**
 * @method showWaitOthers
 * @private
 */
void Online::ShowWaitOthers() {
  auto qn= Label::createWithBMFont("font.OCR", "waiting...");
  auto cw= CCSX.::Center();
  auto wz= CCSX::VisRect();
  auto wb = CCSX::VisBox();

  RemoveAll();

  qn->setPosition(cw.x, wb.top * 0.75);
  qn->setScale(xcfg.game.scale * 0.3);
  qn->setOpacity(0.9*255);
  AddItem(qn);

  auto b1= CreateMenuButton("#cancel.png",
      "#cancel.png",
      "#cancel.png",
      CC_CALLBACK_1(Online::OnCancelPlay,this));
  auto menu= Menu::create();
  menu->addChild(b1);
  menu->setPosition(cw.x, wb.top * 0.1);
  AddItem(menu);
}

void Online::Setup() {
  auto qn= Label::createWithBMFont("font.OCR", "Sign in");
  auto cw= CCSX::Center();
  auto wz= CCSX::VisRect();
  auto wb= CCSX::VisBox();

  CenterImage("game.bg");
  qn->setPosition(cw.x, wb.top * 0.75);
  qn->setScale(xcfg.game.scale * 0.3);
  qn->setOpacity(0.9*255);
  AddItem(qn);

  auto bxz = Sprite::create("#ok.png")->getContentSize();
  // editbox for user
  auto uid = TextField::create();
  uid->setMaxLengthEnabled(true);
  uid->setMaxLength(16);
  uid->setTouchEnabled(true);
  uid->fontName = "Arial";
  uid->fontSize = 18;
  uid->placeHolder = "user id:";
  uid->setPosition(cw.x, cw.y + bxz.height * 0.5 + 2);
  AddItem(uid);

  // editbox for password
  auto pwd = TextField::create();
  pwd->setPasswordEnabled(true);
  pwd->setPasswordStyleText("*");
  pwd->setTouchEnabled(true);
  pwd->setMaxLength(16);
  pwd->fontName = "Arial";
  pwd->fontSize = 18;
  pwd->placeHolder = "password:";
  pwd->setPosition(cw.x, cw.y - bxz.height * 0.5 - 2);
  AddItem(pwd);

  auto b1= CreateMenuButton("#continue.png",
      "#continue.png",
      "#continue.png",
      CC_CALLBACK_1(Online::OnOnlineReq,this));

  auto b2= CreateMenuButton("#cancel.png",
      "#cancel.png",
      "#cancel.png",
      CC_CALLBACK_1(Online::OnBack, this));
  auto menu= Menu::create();
  menu->addChild(b1,1);
  menu->addChild(b2,2);
  menu->setPosition(cw.x, wb.top * 0.1);
  AddItem(menu);
}


















NS_FI_END
