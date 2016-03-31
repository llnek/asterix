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

#include "core/COMP.h"

NS_BEGIN(stoneage)

#define TYPE_GEM_BLUE  0
#define TYPE_GEM_YELLOW 1
#define TYPE_GEM_RED 2
#define TYPE_GEM_GREEN 3
#define TYPE_GEM_PINK  4
#define TYPE_GEM_WHITE 5
#define GEMSET_SIZE 6

#define TILE_SIZE  60
#define GRID_SIZE_X  8
#define GRID_SIZE_Y  11
#define GRID_SPACE  6
#define Z_GRID  1
#define Z_SWAP_1  2
#define Z_SWAP_2  3
#define Z_DIAMOND  4

#define POINTS  10
#define COMBO_POINTS  50
#define DIAMOND_POINTS  100

class GemInfo;
class Gem;
//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GVars : public ecs::Component {

  s_vec<f::FArrayPtr<Gem>*> gemsColMap;
  s_vec<f::FArrInt*> grid;
  s_vec<Gem*> allGems;
  s_vec<f::Cell2I> matchArray;

  __decl_comp_tpid( "n/GVars" )

  __decl_ptr(c::Node,gemsContainer)
  __decl_bf(enabled)
  __decl_ptr(Gem,selectedGem)
  __decl_ptr(Gem,targetGem)
  __decl_md(f::Cell2I,selectedIndex)
  __decl_md(f::Cell2I,targetIndex)
  __decl_md(c::Vec2,selectedGemPos)
  __decl_iz(combos)
  __decl_bf(addingCombos)
};

//////////////////////////////////////////////////////////////////////////////
//

bool isValidTarget(GVars*, int px, int py, const c::Vec2 &);
GemInfo findGemAtPos(GVars*, const c::Vec2&);
bool checkGridMatches(GVars*);
void selectStartGem(GVars*, const GemInfo&);
void selectTargetGem(GVars*, const GemInfo&);

const sstr getGemPng(int type);
int getGemType(int pos);
int getNewGem();

int getVertHorzUnique(GVars*, int col, int row);
int getVertUnique(GVars*, int col, int row);

void dropSelectedGem(GVars*);
void collapseGrid(GVars*);

void onGridCollapseComplete(GVars*);
void onNewSwapComplete(GVars*);

void swapGemsToNewPosition(GVars*);
void showMatchParticle(GVars*, const s_vec<f::Cell2I>&) ;

void animateMatches(GVars*, const s_vec<f::Cell2I>&, VOIDFN);
void dropColumn(GVars*, int col);
void animateIntro(GVars*);
void animateSelected(Gem*);
void resetSelectedGem(GVars*);
void swapGems(GVars*, Gem*, Gem*, VOIDFN);

void dropGemTo(Gem*, float y, float delay, VOIDFN );
void animateCollapse(GVars*, VOIDFN );
void collectDiamonds(const s_vec<ecs::Node*>&);


void dbgGemTypes(GVars*,int);
void dbgGems(GVars*,int);




NS_END




