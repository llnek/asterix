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

#include "network/WebSocket.h"
#include "base/ccUtils.h"
#include "Odin.h"
NS_ALIAS(ws, cocos2d::network::WebSocket)
NS_ALIAS(n, cocos2d::network)

NS_BEGIN(fusilli)


//////////////////////////////////////////////////////////////////////////////
//
static Event mkEvent(Events eventType, int code,
    const s::string& payload) {
  return Event(
    utils::getTimeInMilliseconds(),
    eventType,
    code,
    payload);
}

//////////////////////////////////////////////////////////////////////////////
//
static Event mkPlayRequest (game,user,pwd) {
  return mkEvent(Events::PLAYGAME_REQ, -1,
      [game, user, pwd]);
}

//////////////////////////////////////////////////////////////////////////////
//
static Event mkJoinRequest (room,user,pwd) {
  return mkEvent(Events::JOINGAME_REQ, -1,
      [room, user, pwd]);
}

//////////////////////////////////////////////////////////////////////////////
//
static Event json_decode(const ws::Data& e) {
  assert(!e.isBinary);
  Event evt;

  try {
    evt= sjs.objectfy(e.data);
  } catch (...) {
  }

  if (! sjs.hasKey(evt, 'type')) {
    evt.type= -1;
  }
  if (! sjs.hasKey(evt, 'code')) {
    evt.code= -1;
  }

  if (sjs.hasKey(evt, 'source') &&
      sjs.isstr(evt.source)) {
    evt.source = sjs.objectfy(evt.source);
  }

  return evt;
}

//////////////////////////////////////////////////////////////////////////////
//
WSockSS::WSockSS(Dictionary* config)
  : WSockSS() {
  state= Events::S_NOT_CONNECTED;
  options= config;
  options->retain();
  wss = null;
}

//////////////////////////////////////////////////////////////////////////////
// Send this event through the socket
//
void WSockSS::Send(const Event& evt) {
  if (state == Events::S_CONNECTED &&
      NNP(wss)) {
    wss->send( Jsonfy(evt));
  }
}

//////////////////////////////////////////////////////////////////////////////
// Listen to this message-type and event
void WSockSS::Listen(callback) {
  handler= callback;
}

//////////////////////////////////////////////////////////////////////////////
// Cancel and remove all subscribers
//
void WSockSS::CancelAll() {
  handler= nullptr;
}

//////////////////////////////////////////////////////////////////////////////
// Reset and clear everything
//
void WSockSS::Reset() {
  handler= nullptr;
}

//////////////////////////////////////////////////////////////////////////////
// Close the connection to the socket
//
void WSockSS::Close() {
  state= Events::S_NOT_CONNECTED;
  Reset();
  if (NNP(wss)) {
    try {
      wss->close();
    }
    catch (...) {}
  }
  wss = nullptr;
}

//////////////////////////////////////////////////////////////////////////////
// Disconnect from the socket
//
void WSockSS::Disconnect() {
  Close();
}

//////////////////////////////////////////////////////////////////////////////
//
void WSockSS::onOpen(WebSocket* ws) {
  // connection success
  // send the play game request
  state= Events::S_CONNECTED;
  ws->send(GetPlayRequest());
}

//////////////////////////////////////////////////////////////////////////////
//
void WSockSS::onMessage(WebSocket* ws, const Data& data) {
  auto evt= json_decode(data);
  switch (evt.type) {
    case Events::MSG_NETWORK:
    case Events::MSG_SESSION:
      OnEvent(evt);
    break;
    default:
      CCLOGWARN("unhandled server event: %d, code: %d", evt.type, evt.code);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void WSockSS::onClose(WebSocket* ws) {
  CCLOG("websocket instance (%p) closed", ws);
}

//////////////////////////////////////////////////////////////////////////////
//
void WSockSS::onError(WebSocket* ws, const ws::ErrorCode& error) {
  CCLOG("websocket instance (%p) has error, code: %d", ws, error);
}

//////////////////////////////////////////////////////////////////////////////
//
void WSockSS::OnEvent(const Event& evt) {
  if (NNP(handler)) {
    handler(evt);
  }
}

//////////////////////////////////////////////////////////////////////////////
// Connect to this url and request a websocket upgrade
//
void WSockSS::Connect(const string& url) {
  auto ws= new n::WebSocket(url),
  if (!ws->init(*this, url)) {
    mc_del_ptr(ws);
  } else {
    wss=ws;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
WSockSS::GetPlayRequest() {
  return Jsonfy( mkPlayRequest(this.options.game,
                                   this.options.user,
                                   this.options.passwd));
}

NS_END(fusilli)

