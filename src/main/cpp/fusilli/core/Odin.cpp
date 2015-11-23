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
#include "Odin.h"
NS_BEGIN(fusii)
NS_BEGIN(odin)


BEGIN_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
j::Json evtToDoc(not_null<OdinEvent*> evt) {
  return j::Json::object {
    {"type", (int) evt->type },
    {"code", (int) evt->code },
    {"source", evt->doco }
  };
}

//////////////////////////////////////////////////////////////////////////////
//
owner<OdinEvent*> mkPlayRequest(const stdstr& game,
    const stdstr& user,
    const stdstr& pwd) {

    auto a=  j::Json::array({ game, user, pwd });
    auto p= j::Json(a);
  return new OdinEvent(MType::SESSION,
      EType::PLAYGAME_REQ, p
      );
}

//////////////////////////////////////////////////////////////////////////////
//
owner<OdinEvent*> mkJoinRequest (const stdstr& room,
    const stdstr& user,
    const stdstr& pwd) {

    auto a=  j::Json::array({ room, user, pwd }) ;
    auto p= j::Json(a);
  return new OdinEvent(MType::SESSION,
      EType::JOINGAME_REQ,p
     );
}

//////////////////////////////////////////////////////////////////////////////
//
owner<OdinEvent*> getPlayRequest(not_null<OdinIO*> wss) {
  return mkPlayRequest(wss->game, wss->user, wss->passwd);
}

//////////////////////////////////////////////////////////////////////////////
//
owner<OdinEvent*> json_decode(const n::WebSocket::Data& e) {

  OdinEvent* evt= new OdinEvent();
  stdstr err;

  assert(!e.isBinary);
  try {
    j::Json msg;
    msg.parse(e.bytes, err);
    evt= new OdinEvent(msg);
  } catch (...) {
    CCLOGERROR("failed to parse json: %s", err.c_str());
  }

  return evt;
}
END_NS_UNAMED()


//////////////////////////////////////////////////////////////////////////////
//
OdinIO::~OdinIO() {
  try {
    disconnect(this);
  }
  catch (...) {}
}

//////////////////////////////////////////////////////////////////////////////
//
owner<OdinIO*> reifyPlayRequest(const stdstr& game,
    const stdstr& user, const stdstr& pwd) {
  auto w= mc_new( OdinIO);
  w->game= game;
  w->user= user;
  w->passwd= pwd;
  return w;
}

//////////////////////////////////////////////////////////////////////////////
//
owner<OdinIO*> reifyJoinRequest(const stdstr& room,
    const stdstr& user, const stdstr& pwd) {
  auto w= mc_new( OdinIO);
  w->room= room;
  w->user= user;
  w->passwd= pwd;
  return w;
}

//////////////////////////////////////////////////////////////////////////////
//
OdinIO::OdinIO() {
  cbNetwork = cbSession = cbAll = nullptr;
  state= CType::S_NOT_CONNECTED;
  SNPTR(socket)
}

//////////////////////////////////////////////////////////////////////////////
// Send this event through the socket
//
void netSend(not_null<OdinIO*> wss, not_null<OdinEvent*> evt) {
	c::RefPtr<OdinEvent> ref(evt.get());
	if (wss->state == CType::S_CONNECTED) {
      auto d= evtToDoc(evt);
      wss->socket->send(d.dump());
    }
}

//////////////////////////////////////////////////////////////////////////////
// Listen to this message-type and event
void OdinIO::listen( s::function<void (OdinEvent*)> cb) {
  SNPTR(cbAll)
  listen(MType::ALL , cb);
}

//////////////////////////////////////////////////////////////////////////////
// Listen to this message-type and event
void OdinIO::listen(MType t, s::function<void (OdinEvent*)> cb) {

  if (MType::ALL == t) {
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
void OdinIO::cancelAll() {
  SNPTR(cbNetwork)
  SNPTR(cbSession)
  SNPTR(cbAll)
}

//////////////////////////////////////////////////////////////////////////////
// Listen to this message-type and event
void OdinIO::cancel(MType t) {
  if (MType::ALL == t) {
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
void OdinIO::reset() {
  state= CType::S_NOT_CONNECTED;
  mc_del_ptr(socket);
}

//////////////////////////////////////////////////////////////////////////////
// Close the connection to the socket
//
void close(OdinIO* wss) {
  if (ENP(wss)) { return; }
  try {
    wss->cancelAll();
    wss->socket->close();
  }
  catch (...)
  {}
  wss->reset();
}

//////////////////////////////////////////////////////////////////////////////
// Disconnect from the socket
//
void disconnect(OdinIO* wss) {
  close(wss);
}

//////////////////////////////////////////////////////////////////////////////
//
void OdinIO::onOpen(n::WebSocket* ws) {
  // connection success
  // send the play game request
  state= CType::S_CONNECTED;
  socket=ws;
  auto evt= getPlayRequest(this);
  c::RefPtr<OdinEvent> ref(evt);
  ws->send(evtToDoc(evt).dump());
}

//////////////////////////////////////////////////////////////////////////////
//
void OdinIO::onMessage(n::WebSocket* ws, const n::WebSocket::Data& data) {
  auto evt= json_decode(data);
  c::RefPtr<OdinEvent> ref(evt);
  switch (evt->type) {
    case MType::NETWORK:
    case MType::SESSION:
      onEvent(evt);
    break;
    default:
      CCLOG("unhandled server event: %d, code: %d", (int) evt->type, (int) evt->code);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void OdinIO::onClose(n::WebSocket* ws) {
  CCLOG("websocket instance (%p) closed", ws);
}

//////////////////////////////////////////////////////////////////////////////
//
void OdinIO::onError(n::WebSocket* ws, const n::WebSocket::ErrorCode& error) {
  CCLOG("websocket instance (%p) has error, code: %d", ws, (int) error);
}

//////////////////////////////////////////////////////////////////////////////
//
void OdinIO::onEvent(OdinEvent* evt) {
  switch (evt->type) {
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
n::WebSocket* connect(not_null<OdinIO*> wss, const stdstr& url) {
  auto ws= new n::WebSocket();
  if (!ws->init(*wss, url)) {
    mc_del_ptr(ws);
  } else {
    wss->socket=ws;
  }
  return ws;
}

















NS_END(odin)
NS_END(fusii)

