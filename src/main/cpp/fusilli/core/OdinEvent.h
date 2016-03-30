// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Ken Leung. All rights reserved.

#pragma once

//////////////////////////////////////////////////////////////////////////////
//
#include "base/CCEventCustom.h"
#include "JSON.h"
#include "Primitives.h"
NS_BEGIN(fusii)
NS_BEGIN(odin)

//////////////////////////////////////////////////////////////////////////////
//
enum class CC_DLL MType {
  NETWORK = 0,
  SESSION,
  ALL,

  NICHTS = -1
};

//////////////////////////////////////////////////////////////////////////////
//
enum class CC_DLL EType {

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
  CLOSED,

  NICHTS = -1
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL OdinEvent : public c::EventCustom {

  OdinEvent(MType, EType, j::json body);
  OdinEvent(MType t, EType c);
  OdinEvent(j::json msg);

  __decl_tv(MType, _type, MType::NICHTS)
  __decl_tv(EType, _code, EType::NICHTS)
  __decl_td(j::json, _doco)
  long long _tstamp=0;

  virtual ~OdinEvent();
  OdinEvent();
  __decl_nocpyass(OdinEvent)
};



NS_END
NS_END




