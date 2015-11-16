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
j::Json evtToDoc(const OdinEvent& evt) {
  return j::Json::object {
    {"type", (int) evt.type },
    {"code", (int) evt.code },
    {"source", evt.doco }
  };
}

//////////////////////////////////////////////////////////////////////////////
//
OdinEvent mkPlayRequest(const stdstr& game,
    const stdstr& user,
    const stdstr& pwd) {

  return OdinEvent(MType::SESSION,
      EType::PLAYGAME_REQ,
      j::Json::array({ game, user, pwd }) );
}

//////////////////////////////////////////////////////////////////////////////
//
OdinEvent mkJoinRequest (const stdstr& room,
    const stdstr& user,
    const stdstr& pwd) {

  return OdinEvent(MType::SESSION,
      EType::JOINGAME_REQ,
      j::Json::array({ room, user, pwd }) );
}

//////////////////////////////////////////////////////////////////////////////
//
OdinEvent getPlayRequest(not_null<OdinIO*> wss) {
  return mkPlayRequest(wss->game, wss->user, wss->passwd);
}

//////////////////////////////////////////////////////////////////////////////
//
OdinEvent json_decode(const n::WebSocket::Data& e) {

  OdinEvent evt;
  stdstr err;

  assert(!e.isBinary);
  try {
    j::Json msg;
    msg.parse(e.bytes, err);
    evt= OdinEvent(msg);
  } catch (...) {
    CCLOGERROR("failed to parse json: %s", err.c_str());
  }

  return evt;
}
END_NS_UNAMED()


//////////////////////////////////////////////////////////////////////////////
//
OdinIO::~OdinIO() {
  mc_del_ptr(socket)
}

//////////////////////////////////////////////////////////////////////////////
//
owner<OdinIO*> ReifyPlayRequest(const stdstr& game,
    const stdstr& user, const stdstr& pwd) {
  auto w= new OdinIO();
  w->game= game;
  w->user= user;
  w->passwd= pwd;
  return w;
}

//////////////////////////////////////////////////////////////////////////////
//
owner<OdinIO*> ReifyJoinRequest(const stdstr& room,
    const stdstr& user, const stdstr& pwd) {
  auto w= new OdinIO();
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
void Send(not_null<OdinIO*> wss, const OdinEvent& evt) {
  if (wss->state == CType::S_CONNECTED) {
    auto d= evtToDoc(evt);
    wss->socket->send(d.dump());
  }
}

//////////////////////////////////////////////////////////////////////////////
// Listen to this message-type and event
void OdinIO::Listen( s::function<void (const OdinEvent&)> cb) {
  SNPTR(cbAll)
  Listen(MType::EVERYTHING , cb);
}

//////////////////////////////////////////////////////////////////////////////
// Listen to this message-type and event
void OdinIO::Listen(MType t, s::function<void (const OdinEvent&)> cb) {

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
void OdinIO::CancelAll() {
  SNPTR(cbNetwork)
  SNPTR(cbSession)
  SNPTR(cbAll)
}

//////////////////////////////////////////////////////////////////////////////
// Listen to this message-type and event
void OdinIO::Cancel(MType t) {
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
void OdinIO::Reset() {
  state= CType::S_NOT_CONNECTED;
  mc_del_ptr(socket);
}

//////////////////////////////////////////////////////////////////////////////
// Close the connection to the socket
//
void Close(not_null<OdinIO*> wss) {
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
void Disconnect(not_null<OdinIO*> wss) {
  Close(wss);
}

//////////////////////////////////////////////////////////////////////////////
//
void OdinIO::onOpen(n::WebSocket* ws) {
  // connection success
  // send the play game request
  state= CType::S_CONNECTED;
  socket=ws;
  ws->send(evtToDoc(getPlayRequest(this)).dump());
}

//////////////////////////////////////////////////////////////////////////////
//
void OdinIO::onMessage(n::WebSocket* ws, const n::WebSocket::Data& data) {
  auto evt= json_decode(data);
  switch (evt.type) {
    case MType::NETWORK:
    case MType::SESSION:
      OnEvent(evt);
    break;
    default:
      CCLOG("unhandled server event: %d, code: %d", (int) evt.type, (int) evt.code);
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
void OdinIO::OnEvent(const OdinEvent& evt) {
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
n::WebSocket* Connect(not_null<OdinIO*> wss, const stdstr& url) {
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

