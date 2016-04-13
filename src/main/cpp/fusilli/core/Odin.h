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

#pragma once
//////////////////////////////////////////////////////////////////////////////

#include "network/WebSocket.h"
#include "OdinEvent.h"
NS_ALIAS(n, cocos2d::network)
NS_BEGIN(fusii)
NS_BEGIN(odin)

//////////////////////////////////////////////////////////////////////////////
//
enum class CC_DLL CType {
  S_NOT_CONNECTED = 0,
  S_CONNECTED
};

//////////////////////////////////////////////////////////////////////////////
//
typedef s::function<void (OdinEvent*)> OEventFN;

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL OdinIO : public n::WebSocket::Delegate {
protected:

  __decl_mv(CType, _state, CType::S_NOT_CONNECTED)
  __decl_ptr(n::WebSocket, _socket)
  __decl_nocpyass(OdinIO)

  __decl_md(OEventFN, cbNetwork)
  __decl_md(OEventFN, cbSession)
  __decl_md(OEventFN, cbAll)

  __decl_md(sstr, _passwd)
  __decl_md(sstr, _room)
  __decl_md(sstr, _game)
  __decl_md(sstr, _user)

  void onEvent(OdinEvent*);

public:

  void listen(MType, OEventFN );
  void listen(OEventFN );
  void reset();
  void cancelAll();
  void cancel(MType);

  virtual void onMessage(n::WebSocket*, const n::WebSocket::Data& ) ;
  virtual void onError(n::WebSocket* , const n::WebSocket::ErrorCode& ) ;

  virtual void onClose(n::WebSocket* ) ;
  virtual void onOpen(n::WebSocket*) ;

  __decl_gsms(n::WebSocket*, _socket, Socket)
  __decl_gsms(CType, _state, State)
  __decl_gsms(sstr, _room, RoomId)
  __decl_gsms(sstr, _game, GameId)
  __decl_gsms(sstr, _user, User)
  __decl_gsms(sstr, _passwd, Passwd)

  virtual ~OdinIO();
  OdinIO() {}
};

//////////////////////////////////////////////////////////////////////////////
//
void netSend(not_null<OdinIO*>, MType, EType, const j::json &body);
void netSend(not_null<OdinIO*>, not_null<OdinEvent*>);
void netSendMsg(not_null<OdinIO*>, const j::json &fullmsg);

n::WebSocket* connect(not_null<OdinIO*>, const sstr &url);
void disconnect(OdinIO*);
void close(OdinIO*);

owner<OdinIO*> reifyPlayRequest(const sstr &game,
    const sstr &user, const sstr &pwd);

owner<OdinIO*> reifyJoinRequest(const sstr &room,
    const sstr &user, const sstr &pwd);


NS_END
NS_END



