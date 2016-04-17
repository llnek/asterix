// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Kenneth Leung. All rights reserved.

#include "base/ccUtils.h"
#include "Odin.h"
NS_BEGIN(fusii)
NS_BEGIN(odin)

BEGIN_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
//
j::json evtToDoc(MType type, EType code, j::json body) {
  return j::json({
    {"type", (int) type },
    {"code", (int) code },
    {"source", body }
  });
}

//////////////////////////////////////////////////////////////////////////////
//
j::json evtToDoc(not_null<OdinEvent*> evt) {
  return j::json({
    {"type", (int) evt->getType() },
    {"code", (int) evt->getCode() },
    {"source", evt->getDoco() }
  });
}

//////////////////////////////////////////////////////////////////////////////
//
owner<OdinEvent*> mkPlayRequest(
    const sstr &game,
    const sstr &user,
    const sstr &pwd) {

  return new OdinEvent(
      MType::SESSION,
      EType::PLAYGAME_REQ,
      j::json({
        {"game", game },
        {"user", user },
        {"pswd", pwd}
        }));
}

//////////////////////////////////////////////////////////////////////////////
//
owner<OdinEvent*> mkJoinRequest(
    const sstr &room,
    const sstr &user,
    const sstr &pwd) {

  return new OdinEvent(
      MType::SESSION,
      EType::JOINGAME_REQ,
      j::json({
        {"room", room },
        {"user", user },
        {"pswd", pwd}
        }));
}

//////////////////////////////////////////////////////////////////////////////
//
owner<OdinEvent*> getPlayRequest(not_null<OdinIO*> wss) {
  return mkPlayRequest(wss->getGameId(), wss->getUser(), wss->getPasswd());
}

//////////////////////////////////////////////////////////////////////////////
//
owner<OdinEvent*> json_decode(const n::WebSocket::Data &e) {

  OdinEvent *evt= mc_new(OdinEvent);
  assert(!e.isBinary);

  try {
    j::json msg;
    msg.parse(e.bytes);
    evt= mc_new1(OdinEvent,msg);
  } catch (...) {
    CCLOGERROR("failed to parse json: %s", "");
  }

  return evt;
}

END_NS_UNAMED
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
owner<OdinIO*> reifyPlayRequest(const sstr &game,
    const sstr &user, const sstr &pwd) {
  auto w= mc_new(OdinIO);
  w->setGameId(game);
  w->setUser(user);
  w->setPasswd(pwd);
  return w;
}

//////////////////////////////////////////////////////////////////////////////
//
owner<OdinIO*> reifyJoinRequest(const sstr &room,
    const sstr &user, const sstr &pwd) {
  auto w= mc_new(OdinIO);
  w->setRoomId(room);
  w->setUser(user);
  w->setPasswd(pwd);
  return w;
}

//////////////////////////////////////////////////////////////////////////////
// Send this event through the socket
void netSend(not_null<OdinIO*> wss, not_null<OdinEvent*> evt) {
  if (wss->getState() == CType::S_CONNECTED) {
    auto d= evtToDoc(evt);
    wss->getSocket()->send(d.dump());
  }
}

//////////////////////////////////////////////////////////////////////////////
// Send this msg through the socket
void netSend(not_null<OdinIO*> wss,
             MType m, EType e, const j::json &body) {

  if (wss->getState() == CType::S_CONNECTED) {
    auto d = evtToDoc(m,e,body);
    wss->getSocket()->send(d.dump());
  }
}

//////////////////////////////////////////////////////////////////////////////
// Send this msg through the socket
void netSendMsg(not_null<OdinIO*> wss, const j::json &msg) {

  if (wss->getState() == CType::S_CONNECTED) {
    wss->getSocket()->send(msg.dump());
  }
}

//////////////////////////////////////////////////////////////////////////////
// Listen to this message-type and event
void OdinIO::listen(OEventFN cb) {
  S__NIL(cbAll)
  listen(MType::ALL , cb);
}

//////////////////////////////////////////////////////////////////////////////
// Listen to this message-type and event
void OdinIO::listen(MType t, OEventFN cb) {
  switch (t) {
    case MType::SESSION:
      cbSession= cb;
    break;

    case MType::NETWORK:
      cbNetwork =cb;
    break;

    case MType::ALL:
      cbAll =cb;
    break;
  }
}

//////////////////////////////////////////////////////////////////////////////
// Cancel and remove all subscribers
void OdinIO::cancelAll() {
  S__NIL(cbNetwork)
  S__NIL(cbSession)
  S__NIL(cbAll)
}

//////////////////////////////////////////////////////////////////////////////
// Listen to this message-type and event
void OdinIO::cancel(MType t) {
  switch (t) {
    case MType::SESSION:
      S__NIL(cbSession)
    break;

    case MType::NETWORK:
      S__NIL(cbNetwork)
    break;

    case MType::ALL:
      S__NIL(cbAll)
    break;
  }
}

//////////////////////////////////////////////////////////////////////////////
// Reset and clear everything
void OdinIO::reset() {
  _state= CType::S_NOT_CONNECTED;
  mc_del_ptr(_socket);
}

//////////////////////////////////////////////////////////////////////////////
// Close the connection to the socket
void close(OdinIO *wss) {
  if (E_NIL(wss)) { return; }
  try {
    wss->cancelAll();
    wss->getSocket()->close();
  }
  catch (...)
  {}
  wss->reset();
}

//////////////////////////////////////////////////////////////////////////////
// Disconnect from the socket
void disconnect(OdinIO *wss) {
  close(wss);
}

//////////////////////////////////////////////////////////////////////////////
//
void OdinIO::onOpen(n::WebSocket *ws) {
  // send the play game request
  // connection success
  _state= CType::S_CONNECTED;
  _socket=ws;
  auto evt= getPlayRequest(this);
  c::RefPtr<OdinEvent> ref(evt);
  ws->send(evtToDoc(evt).dump());
}

//////////////////////////////////////////////////////////////////////////////
//
void OdinIO::onMessage(n::WebSocket *ws, const n::WebSocket::Data &data) {
  auto evt= json_decode(data);
  c::RefPtr<OdinEvent>
  ref(evt);

  switch (evt->getType()) {
    case MType::NETWORK:
    case MType::SESSION:
      onEvent(evt);
    break;
    default:
      CCLOG("unhandled server event: %d, code: %d",
          (int) evt->getType(), (int) evt->getCode());
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void OdinIO::onClose(n::WebSocket *ws) {
  CCLOG("websocket instance (%p) closed", ws);
}

//////////////////////////////////////////////////////////////////////////////
//
void OdinIO::onError(n::WebSocket *ws, const n::WebSocket::ErrorCode &error) {
  CCLOG("websocket instance (%p) has error, code: %d", ws, (int) error);
}

//////////////////////////////////////////////////////////////////////////////
//
void OdinIO::onEvent(OdinEvent *evt) {
  switch (evt->getType()) {
    case MType::NETWORK:
      if (cbNetwork) { cbNetwork(evt); }
      if (cbAll) { cbAll(evt); }
      break;
    case MType::SESSION:
      if (cbSession) { cbSession(evt); }
      if (cbAll) { cbAll(evt); }
      break;
  }
}

//////////////////////////////////////////////////////////////////////////////
// Connect to this url and request a websocket upgrade
//
n::WebSocket* connect(not_null<OdinIO*> wss, const sstr &url) {
  auto ws= mc_new(n::WebSocket);
  if (!ws->init(*wss, url)) {
    mc_del_ptr(ws);
  } else {
      wss->setSocket(ws);
  }
  return ws;
}


NS_END
NS_END


