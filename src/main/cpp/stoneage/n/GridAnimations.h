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
NS_BEGIN(stoneage)
class GVars;
//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GridAnimations {

  static GridAnimations* create(GVars*);

  GridAnimations(GVars *s) {
    ss=s;
  }

  void animateIntro();

  void dropColumn(int col);

  void animateSelected(Gem*);

  void resetSelectedGem();

  void swapGems(Gem* gemOrigin, Gem* gemTarget, VOIDFN );

  void animateMatches(const s_vec<f::Cell2P>&, VOIDFN );

  void animateCollapse( VOIDFN);

  void dropGemTo(Gem*, float y, float delay, VOIDFN);

  void collectDiamonds(const s_vec<Gem*>&);

  DECL_IZ(animatedGems)
  DECL_PTR(GVars,ss)
};



NS_END



