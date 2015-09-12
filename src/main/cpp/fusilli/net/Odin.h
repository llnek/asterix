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

#if !defined(__ODIN_H__)
#define __ODIN_H__

#include "core/fusilli.h"

//////////////////////////////////////////////////////////////////////////////
//
enum class Events {

  S_NOT_CONNECTED,
  S_CONNECTED,

  // Msg types

  MSG_NETWORK,
  MSG_SESSION,

  PLAYGAME_REQ,
  JOINGAME_REQ,

  PLAYREQ_NOK,
  JOINREQ_NOK,
  USER_NOK,
  GAME_NOK,
  ROOM_NOK,
  ROOM_FILLED,
  ROOMS_FULL,

  PLAYREQ_OK,
  JOINREQ_OK,

  AWAIT_START,
  SYNC_ARENA,
  POKE_RUMBLE,

  RESTART,
  START,
  STOP,
  POKE_MOVE,
  POKE_WAIT,
  PLAY_MOVE,
  REPLAY,

  QUIT_GAME,

  PLAYER_JOINED,
  STARTED,
  CONNECTED,
  ERROR,
  CLOSED

};


//////////////////////////////////////////////////////////////////////////////
let mkEvent = (eventType, code, payload) => {
  return {
    timeStamp: sjs.now(),
    type: eventType,
    code: code,
    source: payload
  };
}

//////////////////////////////////////////////////////////////////////////////
let mkPlayRequest = (game,user,pwd) => {
  return mkEvent(evts.PLAYGAME_REQ, -1, [game, user, pwd]);
}

//////////////////////////////////////////////////////////////////////////////
let mkJoinRequest = (room,user,pwd) => {
  return mkEvent(evts.JOINGAME_REQ, -1, [room, user, pwd]);
}

//////////////////////////////////////////////////////////////////////////////
let json_decode = e => {
  let evt = {},
  src;

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
class CC_DLL Session {

public:

  void Connect(url);

  Session(Dictionary* config);
  virtual ~Session();

  void Send(const Event& evt);

  void Listen(messageType, event, callback, target);
  void ListenAll(callback,target);
  void CancelAll();
  void Cancel(subid);
  void Reset();

  void Disconnect();
  void Close();

  void OnNetworkMsg(evt);

  void OnSessionMsg(evt);

  void WinSock(url);

  const string GetPlayRequest();

  void OnEvent(evt);

};



NS_END(fusilli)
#endif
