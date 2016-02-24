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
#include "core/ComObj.h"
#include "lib.h"
NS_BEGIN(eskimo)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL GSwitch : public f::ComObj {

  GSwitch(GVars*, c::Node *n)
    : ComObj(n) {
  }

public:

  void initGSwitch(int direction, const c::Vec2& );

  static GSwitch* create(not_null<GVars*>);
  DECL_IZ(direction)

  MDECL_COMP_TPID("n/GSwitch")
};


NS_END


