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

//////////////////////////////////////////////////////////////////////////////

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Collide.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(cuteness)


//////////////////////////////////////////////////////////////////////////////
//
void Collide::preamble() {
  _enemies= _engine->getNodes("n/ESquad")[0];
  _planet= _engine->getNodes("f/CHuman")[0];
  _shared= _engine->getNodes("n/GVars")[0];
}

//////////////////////////////////////////////////////////////////////////////
//
bool Collide::update(float dt) {
  if (MGMS()->isLive()) {
    process(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Collide::process(float dt) {
  auto sq= CC_GEC(EnemySquad, _enemies, "n/ESquad");
  auto pt= CC_GEC(Planet, _planet, "f/CPixie");
  auto ph= CC_GEC(f::CHealth, _planet, "f/CHealth");
  auto &ps= sq->getPools();
  F__LOOP(it, ps) {
    auto &p= *it;
    auto &objs= p->ls();
    F__LOOP(it2, objs) {
      auto &e = *it2;
      if (!e->status()) { continue; }
      auto h=CC_GEC(f::CHealth,e,"f/CHealth");
      auto sp=CC_GEC(Enemy,e,"f/CPixie");
      if (cx::collide(pt,sp)) {
        cx::sfxPlay("sndBlast");
        ph->hurt();
        h->hurt();
        cx::hibernate((ecs::Node*)e);
        planetDamage(pt, ph);
      }
      if (!ph->alive()) { return; }
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Collide::planetDamage(Planet *pt, f::CHealth *ph) {
  pt->setOpacity(255 * ph->ratio());
  auto co= pt->getOpacity();
  if (!ph->alive()) {
    cx::sfxPlay("sndLose");
    pt->setOpacity(0);
    SENDMSG("/game/player/lose");
  } else {
    pt->runAction(c::Spawn::create(
          c::FadeTo::create(200,1),
          c::Sequence::create(
            c::ScaleTo::create(200, 1.2),
            c::CallFunc::create([=](){
              pt->setOpacity(co);
              pt->setScale(1);
              }),
            CC_NIL
            ),
          CC_NIL
          ));
  }
}


NS_END




