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

#include "base/ccUtils.h"
#include "JSON.h"
#include "Odin.h"

NS_ALIAS(n, cocos2d::network)
NS_ALIAS(j, fusilli::json)
NS_ALIAS(c, cocos2d)
NS_ALIAS(s, std)
NS_BEGIN(fusilli)

//////////////////////////////////////////////////////////////////////////////
//
static Event mkEvent(MType eventType, EType code, js::Document* d) {
  return Event(eventType, code, d);
}

//////////////////////////////////////////////////////////////////////////////
//
static Event mkPlayRequest(const stdstr& game,
    const stdstr& user,
    const stdstr& pwd) {
  auto d = new js::Document();
      //[game, user, pwd]);
  return mkEvent(MType::SESSION, EType::PLAYGAME_REQ, d);
}

//////////////////////////////////////////////////////////////////////////////
//
/*
static Event mkJoinRequest (const stdstr& room,
    const stdstr& user,
    const stdstr& pwd) {
  auto d = new js::Document();
      //[room, user, pwd]);
  return mkEvent(MType::SESSION, EType::JOINGAME_REQ, d);
}
*/


//////////////////////////////////////////////////////////////////////////////
//
static owner<js::Document*>  evtToDoc(const Event& evt) {
  return nullptr;
}

//////////////////////////////////////////////////////////////////////////////
//
static Event json_decode(const n::WebSocket::Data& e) {
  js::Document doc;
  Event evt;

  assert(!e.isBinary);
  try {
    doc.Parse(e.bytes);
  } catch (...) {
    CCLOGERROR("failed to parse json: %s", e.bytes);
  }

  if (doc.IsObject()) {
    if (doc.HasMember("type")) {
      //evt.type = doc["type"];
    }
    if (doc.HasMember("code")) {
      //evt.code = SCAST(EType, doc["code"]);
    }
    if (doc.HasMember("source")) {
      auto v= doc["source"].GetString();
      auto s = new js::Document();
      try {
        s->Parse(v);
        evt.doco=s;
      } catch (...) {
        mc_del_ptr(s);
      }
    }
  }

  return evt;
}

//////////////////////////////////////////////////////////////////////////////
//
Event::Event() {
  timeStamp = c::utils::getTimeInMilliseconds();
  type= MType::SESSION;
  code= EType::NICHTS;
  doco= nullptr;
}

//////////////////////////////////////////////////////////////////////////////
//
Event::~Event() {
  mc_del_ptr(doco);
}

//////////////////////////////////////////////////////////////////////////////
//
WSockSS::~WSockSS() {
}

//////////////////////////////////////////////////////////////////////////////
//
owner<WSockSS*> WSockSS::CreatePlayRequest(const stdstr& game,
    const stdstr& user, const stdstr& pwd) {
  auto w= new WSockSS();
  w->game= game;
  w->user= user;
  w->passwd= pwd;
  return w;
}

//////////////////////////////////////////////////////////////////////////////
//
WSockSS* WSockSS::CreateJoinRequest(const stdstr& room,
    const stdstr& user, const stdstr& pwd) {
  auto w= new WSockSS();
  w->room= room;
  w->user= user;
  w->passwd= pwd;
  return w;
}

//////////////////////////////////////////////////////////////////////////////
//
WSockSS::WSockSS() {
  state= CType::S_NOT_CONNECTED;
  cbNetwork = cbSession = cbAll = nullptr;
  wss = nullptr;
}

//////////////////////////////////////////////////////////////////////////////
// Send this event through the socket
//
void WSockSS::Send(const Event& evt) {
  if (state == CType::S_CONNECTED &&
      NNP(wss)) {
    auto d=  j::Stringify( evtToDoc(evt));
    wss->send(d);
  }
}

//////////////////////////////////////////////////////////////////////////////
// Listen to this message-type and event
void WSockSS::ListenAll(s::function<void (const Event&)> cb) {
  cbAll= cb;
}

//////////////////////////////////////////////////////////////////////////////
// Listen to this message-type and event
void WSockSS::Listen(MType t, s::function<void (const Event&)> cb) {
  if (MType::SESSION == t) {
    cbSession= cb;
  }
  if (MType::NETWORK == t) {
    cbNetwork =cb;
  }
}

//////////////////////////////////////////////////////////////////////////////
// Cancel and remove all subscribers
//
void WSockSS::CancelAll() {
  cbAll= nullptr;
}

//////////////////////////////////////////////////////////////////////////////
// Listen to this message-type and event
void WSockSS::Cancel(MType t) {
  if (MType::SESSION == t) {
    cbSession= nullptr;
  }
  if (MType::NETWORK == t) {
    cbNetwork =nullptr;
  }
}

//////////////////////////////////////////////////////////////////////////////
// Reset and clear everything
//
void WSockSS::Reset() {
  cbNetwork = nullptr;
  cbSession = nullptr;
  cbAll = nullptr;
}

//////////////////////////////////////////////////////////////////////////////
// Close the connection to the socket
//
void WSockSS::Close() {
  state= CType::S_NOT_CONNECTED;
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
void WSockSS::onOpen(n::WebSocket* ws) {
  // connection success
  // send the play game request
  state= CType::S_CONNECTED;
  ws->send(GetPlayRequest());
}

//////////////////////////////////////////////////////////////////////////////
//
void WSockSS::onMessage(n::WebSocket* ws, const n::WebSocket::Data& data) {
  auto evt= json_decode(data);
  switch (evt.type) {
    case MType::NETWORK:
    case MType::SESSION:
      OnEvent(evt);
    break;
    default:
      CCLOGWARN("unhandled server event: %d, code: %d", evt.type, evt.code);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void WSockSS::onClose(n::WebSocket* ws) {
  CCLOG("websocket instance (%p) closed", ws);
}

//////////////////////////////////////////////////////////////////////////////
//
void WSockSS::onError(n::WebSocket* ws, const n::WebSocket::ErrorCode& error) {
  CCLOG("websocket instance (%p) has error, code: %d", ws, error);
}

//////////////////////////////////////////////////////////////////////////////
//
void WSockSS::OnEvent(const Event& evt) {
  switch (evt.type) {
    case MType::NETWORK:
      if (NNP(cbNetwork)) { cbNetwork(evt); }
      if (NNP(cbAll)) { cbAll(evt); }
      break;
    case MType::SESSION:
      if (NNP(cbSession)) { cbSession(evt); }
      if (NNP(cbAll)) { cbAll(evt); }
      break;
  }
}

//////////////////////////////////////////////////////////////////////////////
// Connect to this url and request a websocket upgrade
//
void WSockSS::Connect(const stdstr& url) {
  auto ws= new n::WebSocket();
  if (!ws->init(*this, url)) {
    mc_del_ptr(ws);
  } else {
    wss=ws;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
const stdstr WSockSS::GetPlayRequest() {
  return j::Stringify( evtToDoc(mkPlayRequest(game, user, passwd)));
}


NS_END(fusilli)

