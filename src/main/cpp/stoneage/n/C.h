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
#include "Gem.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(stoneage)


enum GemTypes {
  TYPE_GEM_BLUE=  0,
  TYPE_GEM_YELLOW ,
  TYPE_GEM_RED ,
  TYPE_GEM_GREEN ,
  TYPE_GEM_PINK  ,
  TYPE_GEM_WHITE ,
  GEMSET_SIZE
};

enum Points {
  POINTS=  10,
  COMBO_POINTS=  50,
  DIAMOND_POINTS=  100
};

#define TILE_SIZE  60
#define GRID_SIZE_X  8
#define GRID_SIZE_Y  11
#define GRID_SPACE  6
#define Z_GRID  1
#define Z_SWAP_1  2
#define Z_SWAP_2  3
#define Z_DIAMOND  4

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Particle : public f::CPixie {
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Diamond : public f::CPixie {
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Gem : public f::CPixie {

  Gem(int t) {
    _type=t;
  }

  __decl_mv(int, _type,-1)
  __decl_bf(_selected)

public:

  static owner<Gem*> create(int t=TYPE_GEM_BLUE) {
    auto z= mc_new1(Gem, t);
    auto png= getGemPng(t);
    z->initWithSpriteFrameName(png);
    z->autorelease();
    return z;
  }

  void deselect() {selected=false; }
  void select() { selected=true; }

  void reset() {
    this->setRotation(0);
    this->setScale(1);
  }

  __decl_ismx(_selected,Selected)
  __decl_getr(int,_type,Type)

  void setType(int t) {
    this->setSpriteFrame(getGemPng(t));
    _type = t;
  }

};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GemInfo {
  GemInfo(int x, int y, not_null<Gem*> g) {
    this->y = y;
    this->x =x;
    this->gem=g;
  }
  __decl_ptr(Gem,gem)
  __decl_iz(y)
  __decl_iz(x)
};

typedef f::FArrayPtr<Gem> GemPtrArray;

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GVars : public ecs::Component {

  __decl_vec(GemPtrArray*, gemsColMap)
  __decl_vec(f::FArrInt*, grid)
  __decl_vec(Gem*, allGems)
  __decl_vec(f::Cell2I, matchArray)

  __decl_comp_tpid( "n/GVars" )

  __decl_ptr(c::Node, gemsContainer)
  __decl_bf(enabled)
  __decl_ptr(Gem, selectedGem)
  __decl_ptr(Gem, targetGem)
  __decl_md(f::Cell2I, selectedIndex)
  __decl_md(f::Cell2I, targetIndex)
  __decl_md(CCT_PT, selectedGemPos)
  __decl_iz(combos)
  __decl_bf(addingCombos)
};

//////////////////////////////////////////////////////////////////////////////
//

bool isValidTarget(GVars*, int px, int py, const CCT_PT&);
GemInfo findGemAtPos(GVars*, const CCT_PT&);
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




