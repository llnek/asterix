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

#include "core/XConfig.h"
#include "core/COMP.h"
#include "core/CCSX.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(globes)


//////////////////////////////////////////////////////////////////////////////
//
class GVars;
struct CC_DLL GameTile : public f::CPixie {
  static owner<GameTile*> create(GVars *ss, int tile);
  __decl_md(sstr, tileColor)
  __decl_bf(picked)
};

typedef f::FArrayPtr<GameTile> TilePtrArray;

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GVars : public ecs::Component {
  __decl_comp_tpid( "n/GVars" )

  __decl_ptr(c::DrawNode,arrowsLayer)
  __decl_ptr(c::Layer,globezLayer)
  __decl_iz(tolerance)
  __decl_md(sstr,startColor)
  __decl_iz(fieldSize)
  __decl_iz(tileSize)
  __decl_md(f::Box4, grid);
  __decl_vec(TilePtrArray*, tiles)
  __decl_vec(f::Cell2D, visited)
  s_arr<sstr,5> tileTypes= {
    "red",
    "green",
    "blue",
    "grey",
    "yellow"
  };

};

//////////////////////////////////////////////////////////////////////////////
//
const CCT_PT translatePt(GVars*, const CCT_PT &pos);




NS_END




