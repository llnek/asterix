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

#include "network/WebSocket.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "core/fusilli.h"

NS_ALIAS(n, cocos2d::network)
NS_ALIAS(js, rapidjson)
NS_ALIAS(cc, cocos2d)
NS_ALIAS(s, std)
NS_BEGIN(fusilli)

//////////////////////////////////////////////////////////////////////////////
//
enum class CC_DLL MType {
  NETWORK,
  SESSION
};

enum class CC_DLL CType {
  S_NOT_CONNECTED = 0,
  S_CONNECTED
};

enum class CC_DLL EType {

  NICHTS = -1,

  PLAYGAME_REQ = 0,
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
//
struct CC_DLL Event {
  Event(MType t, EType c, js::Document* payload) :
  Event() {
    doco=payload;
    type=t;
    code=c;
  }
  Event();
  ~Event();
  long long timeStamp;
  MType type;
  EType code;
  js::Document* doco;
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL WSockSS : public n::WebSocket::Delegate {
protected:

  const s::string GetPlayRequest();
  void OnEvent(const Event&);
  void Reset();
  WSockSS();

private:

  DISALLOW_COPYASSIGN(WSockSS)

  void (*cbSession)(const Event&);
  void (*cbNetwork)(const Event&);
  void (*cbAll)(const Event&);
  n::WebSocket* wss;
  s::string room;
  s::string game;
  s::string user;
  s::string passwd;
  CType state;

public:

  static WSockSS* CreatePlayRequest(const s::string& game,
      const s::string& user, const s::string& pwd);

  static WSockSS* CreateJoinRequest(const s::string& room,
      const s::string& user, const s::string& pwd);

  virtual ~WSockSS();

  void Connect(const s::string& url);
  void Disconnect();
  void Close();
  void Send(const Event&);

  void Listen(const MType, s::function<void (const Event&)>);
  void ListenAll(s::function<void (const Event&)>);
  void CancelAll();
  void Cancel(const MType);

  virtual void onOpen(n::WebSocket*) override;
  virtual void onMessage(n::WebSocket*, const n::WebSocket::Data& ) override;
  virtual void onClose(n::WebSocket* ) override;
  virtual void onError(n::WebSocket* , const n::WebSocket::ErrorCode& ) override;

};



NS_END(fusilli)
#endif
