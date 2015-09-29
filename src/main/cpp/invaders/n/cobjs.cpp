// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2015, Ken Leung. All rights reserved.

#include "core/CCSX.h"
#include "cobjs.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(invaders)


//////////////////////////////////////////////////////////////////////////////
//
AlienSquad::AlienSquad(not_null<f::XPool*> aliens, int stepx) {

  this->aliens=aliens;
  this->stepx=stepx;
}

//////////////////////////////////////////////////////////////////////////////
//
AlienSquad::~AlienSquad() {
  if (NNP(aliens)) { aliens->ClearAll(); }
}

//////////////////////////////////////////////////////////////////////////////
//
Alien::Alien(not_null<c::Sprite*> s, int value, int rank)
  : ComObj(s, 1, value) {
  this->rank=rank;
}

//////////////////////////////////////////////////////////////////////////////
//
Alien::~Alien() {
}

//////////////////////////////////////////////////////////////////////////////
Bomb::Bomb(not_null<c::Sprite*> s)
  : ComObj(s) {

  auto wz= cx::VisRect();
  x=0;
  y= -50 * wz.size.height / 480;
}

//////////////////////////////////////////////////////////////////////////////
//
Bomb::~Bomb() {
}

//////////////////////////////////////////////////////////////////////////////
//
Cannon::Cannon(float coolDownWindow) {
  this->coolDownWindow= coolDownWindow;
  hasAmmo = true;
};

//////////////////////////////////////////////////////////////////////////////
//
Cannon::~Cannon() {
}

//////////////////////////////////////////////////////////////////////////////
//
Explosion::Explosion(not_null<c::Sprite*> s)
  : ComObj(s) {
  frameTime= 0.1;
}

//////////////////////////////////////////////////////////////////////////
//
Explosion::~Explosion() {
}

//////////////////////////////////////////////////////////////////////////////
//
void Explosion::Inflate(float x, float y) {

  auto cache = c::AnimationCache::getInstance();
  auto anim= cache->getAnimation("boom!");
  if (ENP(anim)) {
    anim = c::Animation::create();
    anim->addSpriteFrame(cx::GetSpriteFrame("boom_0.png"));
    anim->addSpriteFrame(cx::GetSpriteFrame("boom_1.png"));
    anim->addSpriteFrame(cx::GetSpriteFrame("boom_2.png"));
    anim->addSpriteFrame(cx::GetSpriteFrame("boom_3.png"));
    anim->setDelayPerUnit(frameTime);
    anim->setRestoreOriginalFrame(true);
    cache->addAnimation(anim, "boom!");
    anim= cache->getAnimation("boom!");
  }

  sprite->setPosition(x,y);
  status=true;

  sprite->runAction(
    c::Sequence::createWithTwoActions(c::Animate::create(anim),
    c::CallFunc::create([=]() { this->Deflate(); })));
}

//////////////////////////////////////////////////////////////////////////////
Looper::Looper() {
  SNPTR(timer0)
  SNPTR(timer1)
}

//////////////////////////////////////////////////////////////////////////////
Looper::~Looper() {
  CC_DROP(timer0)
  CC_DROP(timer1)
}

//////////////////////////////////////////////////////////////////////////////
//
Missile::Missile(not_null<c::Sprite*> s)
  : ComObj(s) {

  auto wz= cx::VisRect();
  x= 0;
  y= 150 * wz.size.height / 480;
}

//////////////////////////////////////////////////////////////////////////////
//
Missile::~Missile() {
}

//////////////////////////////////////////////////////////////////////////////
//
Motion::Motion() {
  right = false;
  left = false;
}

//////////////////////////////////////////////////////////////////////////
//
Motion::~Motion() {
}

//////////////////////////////////////////////////////////////////////////////
//
Ship::Ship(not_null<c::Sprite*> s,
    const stdstr& f1, const stdstr& f2)

  : ComObj(s) {

  this->frame0 = f1;
  this->frame1= f2;
}

//////////////////////////////////////////////////////////////////////////
//
Ship::~Ship() {
}

//////////////////////////////////////////////////////////////////////////////
//
Velocity::Velocity(float vx, float vy) {
  x= vx;
  y= vy;
}

//////////////////////////////////////////////////////////////////////////////
//
Velocity::~Velocity() {
}



NS_END(invaders)


