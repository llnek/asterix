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

#include "core/XConfig.h"
#include "core/CCSX.h"
#include "lib.h"
#include "C.h"

NS_ALIAS(cx, fusii::ccsx)
NS_USING(fusii)
NS_BEGIN(pong)

//////////////////////////////////////////////////////////////////////////////
//
j::json fmtGameData(f::GMode m) {

  if (m == f::GMode::TWO) {
    return j::json({
        {"ppids", j::json::object_t {
          { XCFG()->getL10NStr("p1"), j::json::array_t {
            1, XCFG()->getL10NStr("player1") }
          },
          { XCFG()->getL10NStr("p2"), j::json::array_t {
            2, XCFG()->getL10NStr("player2") }
          },
      } },
      {"pnum", 1 }
    });
  }

  if (m == f::GMode::ONE) {
    return j::json({
        {"ppids", j::json:: object_t {
          { XCFG()->getL10NStr("cpu"), j::json::array_t {
            2, XCFG()->getL10NStr("player2") }
          },
          { XCFG()->getL10NStr("p1"), j::json::array_t {
            1, XCFG()->getL10NStr("player1") }
          },
      } },
      {"pnum", 1 }
    });
  }

  if (m == f::GMode::NET) {
    return j::json({
        {"ppids", j::json::object_t { } },
      {"pnum", 0 }
    });
  }

  throw "bad game mode!";
}


NS_END(pong)



