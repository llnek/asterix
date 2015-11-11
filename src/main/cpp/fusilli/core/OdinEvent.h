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

#if !defined(__ODINEVENT_H__)
#define __ODINEVENT_H__

#include "base/CCEventCustom.h"
#include "dbox/json11.hpp"
#include "Primitives.h"
NS_ALIAS(j, json11)
NS_BEGIN(fusii)
NS_BEGIN(odin)

//////////////////////////////////////////////////////////////////////////////
//
enum class CC_DLL MType {
  NICHTS = -1,
  NETWORK = 0,
  SESSION,
  EVERYTHING
};

//////////////////////////////////////////////////////////////////////////////
//
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
class CC_DLL OdinEvent : public c::EventCustom {
protected:

  NO__CPYASS(OdinEvent)

  double tstamp;
  MType type;
  EType code;
  j::Json doco;

public:

  OdinEvent(MType, EType, j::Json& body);
  OdinEvent(MType t, EType c);
  OdinEvent(j::Json& msg);

  DECL_CTOR(OdinEvent)
};



NS_END(odin)
NS_END(fusii)
#endif
