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
  DECL_TD(sstr, tileColor)
  DECL_BF(picked)
};

typedef f::FArrayPtr<GameTile> TilePtrArray;

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GVars : public ecs::Component {
  MDECL_COMP_TPID( "n/GVars" )

  DECL_PTR(c::DrawNode,arrowsLayer)
  DECL_PTR(c::Layer,globezLayer)
  DECL_IZ(tolerance)
  DECL_TD(sstr,startColor)
  DECL_IZ(fieldSize)
  DECL_IZ(tileSize)
  s_vec<TilePtrArray*> tiles;
  s_vec<f::Cell2D> visited;
  s_arr<sstr,5> tileTypes= {
    "red",
    "green",
    "blue",
    "grey",
    "yellow"
  };

};


NS_END




