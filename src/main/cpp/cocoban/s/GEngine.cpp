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

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Resolve.h"
#include "Collide.h"
#include "Move.h"
#include "AI.h"
#include "GEngine.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(cocoban)

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initLevels(GVars *ss) {
  ss->levels[0]= {1,1,1,1,1,1,1};
  ss->levels[1]= {1,1,0,0,0,0,1};
  ss->levels[2]= {1,1,3,0,2,0,1};
  ss->levels[3]= {1,0,0,4,0,0,1};
  ss->levels[4]= {1,0,3,1,2,0,1};
  ss->levels[5]= {1,0,0,1,1,1,1};
  ss->levels[6]= {1,1,1,1,1,1,1};
  ss->swipeTolerance=10;
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initEntities() {

  auto ent= this->reifyNode("Shared",true);
  auto ss=mc_new(GVars);
  initLevels(ss);
  ent->checkin(ss);

  auto has_player=false;
  for (auto i=0; i < ss->levels.size(); ++i) {
    auto &row= ss->levels[i];
    for (auto j=0; j < row.size(); ++j) {
      auto arr= new f::NodePtrArr( row.size());
      auto levelValue= row[j];
      arr->fill(CC_NIL);
      ss->crates.push_back(arr);
      switch (levelValue) {
        case 4:
        case 6: {
          auto ps = f::CPixie::reifyFrame("player.png");
          auto ent=this->reifyNode("Player", true);
          CC_POS2(ps, 165+25*j,185-25*i);
          XCFG()->fit(ps);
          MGML()->addAtlasItem("game-pics",ps);
          ent->checkin(ps);
          ent->checkin(mc_new(f::CGesture));
          ss->playerPos= f::Cell2I(j,i);
          assert(!has_player);
          has_player=true;
        }
        break;

        case 3:
        case 5: {
          auto cs = cx::reifySprite("crate.png");
          CC_POS2(cs, 165+25*j,185-25*i);
          XCFG()->fit(cs);
          MGML()->addAtlasItem("game-pics",cs);
          arr->set(j,cs);
        }
        break;
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initSystems() {
  regoSystem(mc_new1(Resolve,this));
  regoSystem(mc_new1(Collide,this));
  regoSystem(mc_new1(AI,this));
  regoSystem(mc_new1(Move,this));
}


NS_END

