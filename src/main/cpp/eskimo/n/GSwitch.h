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

#include "core/COMP.h"
#include "C.h"
NS_BEGIN(eskimo)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL GSwitch : public f::CPixie {

  __decl_iz(_direction)

  GSwitch(GVars *ss) {
    this->ss=ss;
  }

public:

  static owner<GSwitch*> create(not_null<GVars*>);

  void initGSwitch(int dir, const CCT_PT&);

};


NS_END


