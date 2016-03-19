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
#include "GEngine.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(blast)

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initEntities() {

  auto ent= this->reifyNode("Shared",true);
  auto ss= mc_new(GVars);
  ent->checkin(ss);
  // create & retain CCArray container lists
  ss->enemies = c::Array::createWithCapacity(MAX_ENEMIES);
  CC_KEEP(ss->enemies);
  ss->powerups = c::Array::createWithCapacity(MAX_POWERUPS);
  CC_KEEP(ss->powerups);
  ss->blasts = c::Array::createWithCapacity(MAX_BLASTS);
  CC_KEEP(ss->blasts);
  ss->missiles = c::Array::createWithCapacity(MAX_MISSILES);
  CC_KEEP(ss->missiles);
  ss->is_popup_active = false;
  ss->seconds = 0;
  ss->enemies_killed_total = 0;
  ss->enemies_killed_combo = 0;
  ss->combo_timer = 0;

  // create & add the player at the center of the screen
  auto sp = Player::create();
  sp->setPosition(wb.cx, wb.cy);
  MGML()->addItem(sp, E_LAYER_PLAYER);
  ent= this->reifyNode("Player",true);
  ent->checkin(mc_new(f::CGesture));
  ent->checkin(mc_new1(CPlayer,sp));
  ent->checkin(mc_new(f::CHealth));

  // create enemies
  addEnemyFormation(ss,ent);
}

//////////////////////////////////////////////////////////////////////////////
//
EEnemyFormation GEngine::getEnemyFormationType(GVars *ss) {
  // return a formation type from a list of formation types, based on time user has been playing
  // the longer the user has survived, the more difficult the formations will be
  int rc= E_FORMATION_RANDOM_EASY;

  if (ss->seconds > E_SKILL6) {
    auto i = cx::rand() * GVars::skill6_formations_size;
    rc= GVars::skill6_formations[i];
  }
  else if (ss->seconds > E_SKILL5) {
    auto i = cx::rand() * GVars::skill5_formations_size;
    rc= GVars::skill5_formations[i];
  }
  else if (ss->seconds > E_SKILL4) {
    auto i = cx::rand() * GVars::skill4_formations_size;
    rc= GVars::skill4_formations[i];
  }
  else if (ss->seconds > E_SKILL3) {
    auto i = cx::rand() * GVars::skill3_formations_size;
    rc= GVars::skill3_formations[i];
  }
  else if (ss->seconds > E_SKILL2) {
    auto i = cx::rand() * GVars::skill2_formations_size;
    rc= GVars::skill2_formations[i];
  }
  else if (ss->seconds > E_SKILL1) {
    auto i = cx::rand() * GVars::skill1_formations_size;
    rc= GVars::skill1_formations[i];
  }

  return (EEnemyFormation) rc;
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::addEnemyFormation(GVars *ss, ecs::Node *py) {
  // fetch an enemy formation formation
  auto player=CC_GEC(Player,py,"f/CPixie");
  auto type = getEnemyFormationType(ss);
  // fetch a list of positions for the given formation
  s_vec<c::Vec2> formation = getEnemyFormation(type, box, player->pos());
  auto num_enemies_on_screen = ss->enemies->count();
  auto num_enemies_to_create = formation.size();
  // limit the total number of enemies to MAX_ENEMIES
  if (num_enemies_on_screen + num_enemies_to_create >= MAX_ENEMIES) {
    num_enemies_to_create = MAX_ENEMIES - num_enemies_on_screen;
  }
  // create, add & position enemies based on the formation
  for (auto i = 0; i < num_enemies_to_create; ++i) {
    auto enemy = Enemy::create();
    enemy->setPosition(formation[i]);
    enemy->spawn(i * ENEMY_SPAWN_DELAY);
    MGML()->addItem(enemy, E_LAYER_ENEMIES);
    ss->enemies->addObject(enemy);
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

