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
using s = namespace std;
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
static Event json_decode(const Event& e) {
  Event evt;

  try {
    evt= sjs.objectfy(e.data);
  } catch (e) {
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
  this->onmessage= nullptr;
  this->onerror= nullptr;
  this->onclose= nullptr;
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
// Connect to this url and request a websocket upgrade
//
void WSockSS::Connect(const string& url) {
  auto ws= new WebSocket(url),

  // connection success
  // send the play game request
  ws.onopen= () => {
    me.state= evts.S_CONNECTED;
    ws.send(me.getPlayRequest());
  }

  ws.onmessage= (e) => {
    const evt= json_decode(e);
    switch (evt.type) {
      case evts.MSG_NETWORK:
      case evts.MSG_SESSION:
        me.onevent(evt);
      break;
      default:
        sjs.loggr.warn("unhandled server event: " +
                       evt.type +
                       ", code = " +
                       evt.code);
    }
  }

  ws.onclose= (e) => {
    sjs.loggr.debug("closing websocket.");
  }

  ws.onerror= (e) => {
    sjs.loggr.debug("websocket error.\n" + e);
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

