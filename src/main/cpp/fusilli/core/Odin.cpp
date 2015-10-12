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
NS_ALIAS(j, fusii::json)
NS_ALIAS(c, cocos2d)
NS_ALIAS(s, std)
NS_BEGIN(fusii)
NS_BEGIN(wsock)
//////////////////////////////////////////////////////////////////////////////
//
static Event mkEvent(MType eventType,
    EType code,
    not_null<js::Document*> d) {

  return Event(eventType, code, d);
}

//////////////////////////////////////////////////////////////////////////////
//
static Event mkPlayRequest(const stdstr& game,
    const stdstr& user,
    const stdstr& pwd) {

  auto d = new js::Document();
  //TODO:
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
  mc_del_ptr(socket)
}

//////////////////////////////////////////////////////////////////////////////
//
owner<WSockSS*> CreatePlayRequest(const stdstr& game,
    const stdstr& user, const stdstr& pwd) {
  auto w= new WSockSS();
  w->game= game;
  w->user= user;
  w->passwd= pwd;
  return w;
}

//////////////////////////////////////////////////////////////////////////////
//
owner<WSockSS*> CreateJoinRequest(const stdstr& room,
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
  cbNetwork = cbSession = cbAll = nullptr;
  state= CType::S_NOT_CONNECTED;
  SNPTR(socket)
}

//////////////////////////////////////////////////////////////////////////////
// Send this event through the socket
//
void Send(not_null<WSockSS*> wss, const Event& evt) {
  if (wss->state == CType::S_CONNECTED) {
    auto d= j::Stringify( evtToDoc(evt));
    wss->socket->send(d);
  }
}

//////////////////////////////////////////////////////////////////////////////
// Listen to this message-type and event
void WSockSS::Listen(MType t, s::function<void (const Event&)> cb) {

  if (MType::EVERYTHING == t) {
    cbAll =cb;
  }
  else
  if (MType::SESSION == t) {
    cbSession= cb;
  }
  else
  if (MType::NETWORK == t) {
    cbNetwork =cb;
  }
}

//////////////////////////////////////////////////////////////////////////////
// Cancel and remove all subscribers
//
void WSockSS::CancelAll() {
  SNPTR(cbNetwork)
  SNPTR(cbSession)
  SNPTR(cbAll)
}

//////////////////////////////////////////////////////////////////////////////
// Listen to this message-type and event
void WSockSS::Cancel(MType t) {
  if (MType::EVERYTHING == t) {
    SNPTR(cbAll)
  }
  else
  if (MType::SESSION == t) {
    SNPTR(cbSession)
  }
  else
  if (MType::NETWORK == t) {
    SNPTR(cbNetwork)
  }
}

//////////////////////////////////////////////////////////////////////////////
// Reset and clear everything
//
void WSockSS::Reset() {
  state= CType::S_NOT_CONNECTED;
  mc_del_ptr(socket);
}

//////////////////////////////////////////////////////////////////////////////
// Close the connection to the socket
//
void Close(not_null<WSockSS*> wss) {
  wss->CancelAll();
  try {
    wss->socket->close();
  }
  catch (...)
  {}
  wss->Reset();
}

//////////////////////////////////////////////////////////////////////////////
// Disconnect from the socket
//
void Disconnect(not_null<WSockSS*> wss) {
  Close(wss);
}

//////////////////////////////////////////////////////////////////////////////
//
void WSockSS::onOpen(n::WebSocket* ws) {
  // connection success
  // send the play game request
  state= CType::S_CONNECTED;
  socket=ws;
  ws->send(GetPlayRequest(this));
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
      CCLOG("unhandled server event: %d, code: %d", evt.type, evt.code);
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
n::WebSocket* Connect(not_null<WSockSS*> wss, const stdstr& url) {
  auto ws= new n::WebSocket();
  if (!ws->init(*wss, url)) {
    mc_del_ptr(ws);
  } else {
    wss->socket=ws;
  }
  return ws;
}

//////////////////////////////////////////////////////////////////////////////
//
const stdstr GetPlayRequest(not_null<WSockSS*> wss) {
  return j::Stringify( evtToDoc(mkPlayRequest(wss->game, wss->user, wss->passwd)));
}

NS_END(wsock)
NS_END(fusii)

