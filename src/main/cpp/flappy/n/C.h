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
NS_BEGIN(flappy)

#define MAX_DRAGON_SPEED  -40
#define FLAP_FORCE  13
#define ANIMATION_ACTION_TAG  123
#define MOVEMENT_ACTION_TAG  121

#define SILHOUETTE_SPRITE_Y  100
#define MAX_SCROLLING_SPEED  6
#define CASTLE_SPRITE_Y  -50
#define MAX_STARS  15
#define HIGHSCORE_KEY  "DragonDashHighScore"
#define GRAVITY  -0.75

#define VERT_GAP_BWN_TOWERS  300

enum E_ZORDER {

  E_LAYER_BG=0,
  E_LAYER_STARS=2,
  E_LAYER_SILHOUETTE=4,
  E_LAYER_CASTLE=6,
  E_LAYER_TOWER=8,
  E_LAYER_PLAYER=10,
  E_LAYER_HUD=12,
  E_LAYER_POPUPS=14

};

//////////////////////////////////////////////////////////////////////////////
//
class Fairytale;
class Tower;
class Dragon;
struct CC_DLL GVars : public ecs::Component {
  __decl_comp_tpid( "n/GVars" )

  __decl_ptr(Fairytale, fairytale)
  __decl_ptr(Tower, towers)
  __decl_ptr(Dragon, dragon)
  __decl_bf(hasGameStarted)
  __decl_fz(castleRoof)


};

//////////////////////////////////////////////////////////////////////////////
//
void addDragon(f::XNode*);





NS_END




