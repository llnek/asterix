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
#include "aeon/fusilli.h"

NS_ALIAS(n, cocos2d::network)
NS_ALIAS(js, rapidjson)
NS_ALIAS(c, cocos2d)
NS_ALIAS(s, std)
NS_BEGIN(fusii)



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

  const stdstr GetPlayRequest();
  void OnEvent(const Event&);
  void Reset();
  WSockSS();

private:

  s::function<void (const Event&)> cbSession;
  s::function<void (const Event&)> cbNetwork;
  s::function<void (const Event&)> cbAll;

  NO__CPYASS(WSockSS)

  n::WebSocket* wss;
  stdstr room;
  stdstr game;
  stdstr user;
  stdstr passwd;
  CType state;

public:

  static owner<WSockSS*> CreatePlayRequest(const stdstr& game,
      const stdstr& user, const stdstr& pwd);

  static owner<WSockSS*> CreateJoinRequest(const stdstr& room,
      const stdstr& user, const stdstr& pwd);

  virtual ~WSockSS();

  void Connect(const stdstr& url);
  void Disconnect();
  void Close();
  void Send(const Event&);

  void Listen(const MType, s::function<void (const Event&)>);
  void ListenAll(s::function<void (const Event&)>);
  void CancelAll();
  void Cancel(const MType);

  virtual void onClose(n::WebSocket* ) ;
  virtual void onOpen(n::WebSocket*) ;

  virtual void onMessage(n::WebSocket*,
      const n::WebSocket::Data& ) ;

  virtual void onError(n::WebSocket* ,
      const n::WebSocket::ErrorCode& ) ;

};



NS_END(fusii)
#endif


