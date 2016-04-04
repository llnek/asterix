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

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Resolve.h"
#include "Collide.h"
#include "Move.h"
#include "AI.h"
#include "n/Terrain.h"
#include "n/Player.h"
#include "GEngine.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(victorian)

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initEntities() {

  auto pool= MGMS()->reifyPool("Clouds");
  auto t= Terrain::create();
  auto p= Player::create();
  auto wb=cx::visBox();

  //add clouds
  for (auto i = 0; i < 4; ++i) {
    auto cy = wb.top * (i % 2 == 0 ? 0.7 : 0.8);
    auto ui= f::CPixie::reifyFrame("cloud.png");
    auto ent= this->reifyNode("Cloud", true);
    ent->checkin(ui);
    ui->inflate(wb.right * 0.15 + i * wb.right * 0.25, cy);
    MGML()->addAtlasItem("game-pics",ui, kBackground);
    pool->checkin(ent);
  }

  auto ent=this->reifyNode("Arena", true);
  ent->checkin(mc_new(GVars));

  ent=this->reifyNode("Terrain", true);
  ent->checkin(mc_new(f::CAutoma));
  ent->checkin(t);

  ent=this->reifyNode("Player", true);
  ent->checkin(mc_new(PlayerMotion));
  ent->checkin(mc_new(PlayerStats));
  ent->checkin(mc_new(f::CGesture));
  ent->checkin(mc_new(f::CHealth));
  ent->checkin(p);

  MGML()->addAtlasItem("game-pics",t, kMiddleground);
  MGML()->addAtlasItem("game-pics",p, kBackground);

}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initSystems() {
  regoSystem(mc_new1(Resolve,this));
  regoSystem(mc_new1(Move,this));
  regoSystem(mc_new1(AI,this));
  regoSystem(mc_new1(Collide,this));
}


NS_END

