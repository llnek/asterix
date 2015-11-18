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
class CC_DLL OdinIO : public n::WebSocket::Delegate {
protected:

  s::function<void (OdinEvent*)> cbSession;
  s::function<void (OdinEvent*)> cbNetwork;
  s::function<void (OdinEvent*)> cbAll;

  void onEvent(OdinEvent*);

  NO__CPYASS(OdinIO)

public:

  void listen(const MType, s::function<void (OdinEvent*)>);
  void listen(s::function<void (OdinEvent*)>);
  void reset();
  void cancelAll();
  void cancel(const MType);

  virtual void onError(n::WebSocket* , const n::WebSocket::ErrorCode& ) ;
  virtual void onMessage(n::WebSocket*, const n::WebSocket::Data& ) ;

  virtual void onClose(n::WebSocket* ) ;
  virtual void onOpen(n::WebSocket*) ;

  DECL_CTOR(OdinIO)

  n::WebSocket* socket;
  stdstr room;
  stdstr game;
  stdstr user;
  stdstr passwd;
  CType state;

};

//////////////////////////////////////////////////////////////////////////////
//
n::WebSocket* connect(not_null<OdinIO*>, const stdstr& url);

void disconnect(not_null<OdinIO*>);

void close(not_null<OdinIO*>);

void netSend(not_null<OdinIO*>, OdinEvent*);

owner<OdinIO*> reifyPlayRequest(const stdstr& game,
    const stdstr& user, const stdstr& pwd);

owner<OdinIO*> reifyJoinRequest(const stdstr& room,
    const stdstr& user, const stdstr& pwd);












NS_END(odin)
NS_END(fusii)
#endif


