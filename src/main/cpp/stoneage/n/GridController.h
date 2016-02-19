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

NS_BEGIN(stoneage)
class GemInfo;
//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GridController : public f::ComObj {

  MDECL_COMP_TPID("n/GridController")

  DECL_PTR(GridAnimations,anim)
  DECL_PTR(GVars,ss)
  DECL_BF(enabled)
  DECL_BF(touchDown)

  GridController(GVars *v, GridAnimations* a) {
    ss=v;
    anim=a;
  }

  void addMatches(const s_vec<f::Cell2I>&);

  void checkTypeMatch(int c, int r);
  bool checkGridMatches();

  bool find(const f::Cell2I&, const s_vec<f::Cell2I>& );

  GemInfo findGemAtPosition(const c::Vec2&);

  void selectStartGem(const GemInfo& );

  void selectTargetGem(const GemInfo& );

  void onTouchUp(const c::Vec2&);

  void onTouchMove(const c::Vec2&);
  void onTouchDown(const c::Vec2&);

  bool isValidTarget(int px, int py, const c::Vec2&);

};



NS_END



