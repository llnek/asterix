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
NS_BEGIN(spacecraze)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Alien : public f::ComObj {
  MDECL_COMP_TPID("n/Alien")
  Alien(not_null<c::Sprite*> s)
  : ComObj(s) {

  }
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL AlienSquad : public a::Component {

  const s_vec<f::ComObj*>& list() { return aliens->list(); }

  AlienSquad(not_null<f::XPool*> aliens) {
    this->aliens=aliens;
  }

  int size() { return aliens->size(); }

  MDECL_COMP_TPID("n/AlienSquad")

  //not owner of pool
  DECL_PTR(f::XPool, aliens)
};



NS_END


