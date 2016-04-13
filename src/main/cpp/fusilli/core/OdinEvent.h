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
class CC_DLL OdinEvent : public c::EventCustom {

  __decl_mv(MType, _type, MType::NICHTS)
  __decl_mv(EType, _code, EType::NICHTS)
  __decl_md(j::json, _doco)
  __decl_nocpyass(OdinEvent)
  long long _tstamp=0;

public:

  OdinEvent(MType, EType, const j::json &body);
  OdinEvent(MType, EType);
  OdinEvent(const j::json &msg);

  __decl_gsms(j::json&,_doco, Doco)
  __decl_gsms(MType,_type, Type)
  __decl_gsms(EType,_code, Code)

  virtual ~OdinEvent();
  OdinEvent();
};



NS_END
NS_END




