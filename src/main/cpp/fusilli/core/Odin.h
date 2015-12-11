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
public:

  typedef s::function<void (OdinEvent*)> OEventFN;

protected:

  void onEvent(OdinEvent*);
  OEventFN cbSession;
  OEventFN cbNetwork;
  OEventFN cbAll;

public:

  void listen(const MType, OEventFN );
  void listen(OEventFN );
  void reset();
  void cancelAll();
  void cancel(const MType);

  virtual void onError(n::WebSocket* , const n::WebSocket::ErrorCode& ) ;
  virtual void onMessage(n::WebSocket*, const n::WebSocket::Data& ) ;

  virtual void onClose(n::WebSocket* ) ;
  virtual void onOpen(n::WebSocket*) ;

  CType state= CType::S_NOT_CONNECTED;
  n::WebSocket* socket=nullptr;
  sstr room;
  sstr game;
  sstr user;
  sstr passwd;

  virtual ~OdinIO();
  OdinIO() {}

  NOCPYASS(OdinIO)
};

//////////////////////////////////////////////////////////////////////////////
//
n::WebSocket* connect(not_null<OdinIO*>, const sstr& url);

void netSend(not_null<OdinIO*>, not_null<OdinEvent*>);

void disconnect(OdinIO*);

void close(OdinIO*);


owner<OdinIO*> reifyPlayRequest(const sstr& game,
    const sstr& user, const sstr& pwd);

owner<OdinIO*> reifyJoinRequest(const sstr& room,
    const sstr& user, const sstr& pwd);


NS_END(odin)
NS_END(fusii)
#endif


