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
NS_BEGIN(bunny)

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initEntities() {
  auto po= MGMS()->reifyPool("Bombs");
  auto wb= cx::visBox();

  po->preset([=]() -> f::Poolable* {
    auto bomb = f::CPixie::reifyFrame("bomb.png");
    auto ent= this->reifyNode("Bomb");
    auto sp= PCAST(c::Sprite,bomb);
    auto sz= CC_CSIZE(sp);
    setPhysicsBody(sp);
    MGML()->addAtlasItem("game-pics", sp,1);
    ent->checkin(bomb);
    sp->getPhysicsBody()->setEnabled(false);
    return ent;
  }, 16);

  auto py= f::CPixie::reifyFrame("player.png");
  auto ent= this->reifyNode("Player", true);
  auto sp= PCAST(c::Sprite,py);
  auto sz = CC_CSIZE(sp);
  sp->setPosition(wb.cx, wb.top * 0.23);
  setPhysicsBody(sp);
  MGML()->addAtlasItem("game-pics", sp, 0);

  auto rt= c::Rect(0, 0, CC_ZW(sz),CC_ZH(sz));
  auto anim= c::Animation::create();
  auto f1= cx::getSpriteFrame("player.png", rt);
  auto f2= cx::getSpriteFrame("player2.png", rt);
  anim->addSpriteFrame(f1);
  anim->addSpriteFrame(f2);
  anim->setDelayPerUnit(0.2);
  sp->runAction(
      c::RepeatForever::create( c::Animate::create(anim)));
  ent->checkin(py);
  ent->checkin(mc_new(f::CGesture));
  ent->checkin(mc_new(f::CHuman));
  ent->checkin(mc_new(f::CMove));
  ent->checkin(mc_new(f::CHealth));

  // global
  ent= this->reifyNode("Shared",true);
  ent->checkin(mc_new(GVars));
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::setPhysicsBody(c::Sprite *sprite) {
  auto body = c::PhysicsBody::createCircle(sprite->getContentSize().width/2);
  body->setContactTestBitmask(true);
  body->setDynamic(true);
  sprite->setPhysicsBody(body);
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

