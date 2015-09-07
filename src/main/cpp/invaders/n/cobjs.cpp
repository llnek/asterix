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

#include "cobjs.h"
NS_USING(fusilli)
NS_BEGIN(invaders)

//////////////////////////////////////////////////////////////////////////////
//
AlienSquad::AlienSquad(const vector<>& aliens, int stepx)
  : AlienSquad() {
  this->aliens=aliens;
  this->stepx=step;
}

//////////////////////////////////////////////////////////////////////////////
//
AlienSquad::AlienSquad() {
}

//////////////////////////////////////////////////////////////////////////////
//
AlienSquad::~AlienSquad() {
}

//////////////////////////////////////////////////////////////////////////////
//
Alien::Alien(Sprite* sprite, float value, int rank)
  : ComObj(sprite, 1, value) {
  this->rank=rank;
}

//////////////////////////////////////////////////////////////////////////////
//
Alien::~Alien() () {
}

//////////////////////////////////////////////////////////////////////////////
//
Alien::Alien() () {
}


//////////////////////////////////////////////////////////////////////////////
Bomb::Bomb(Sprite* s) : ComObj(s) {
  auto wz= CCSX::VisRect();
  vel= Vec2(0, -50 * wz.height / 480);
}

//////////////////////////////////////////////////////////////////////////////
Cannon::Cannon(float coolDownWindow) {
  this->coolDownWindow= coolDownWindow;
  hasAmmo = true;
};

//////////////////////////////////////////////////////////////////////////////
//
Explosion::Explosion(Sprite* s) : ComObj(s) {
  frameTime= 0.1;
}

//////////////////////////////////////////////////////////////////////////////
//
void Explosion::Inflate(options) {
  auto anim= Animation::create();
  anim->addSpriteFrame(CCSX::GetSpriteFrame("boom_0.png"));
  anim->addSpriteFrame(CCSX::GetSpriteFrame("boom_1.png"));
  anim->addSpriteFrame(CCSX::GetSpriteFrame("boom_2.png"));
  anim->addSpriteFrame(CCSX::GetSpriteFrame("boom_3.png"));
  anim->setDelayPerUnit(frameTime);

  sprite->setPosition(options.x, options.y);
  status=true;
  sprite->runAction(
    Sequence::createWithTwoActions(Animate::create(anim),
    CallFunc::create([this]() { this->Deflate(); })));
}

//////////////////////////////////////////////////////////////////////////////
Looper::Looper(int count) {
  timers= null; //sjs.makeArray(count,null);
}

//////////////////////////////////////////////////////////////////////////////
//
Missile::Missile(Sprite* s) : ComObj(s) {
  auto wz= CCSZ::VisRect();
  this->x = 0;
  this->y = 150 * wz.height / 480;
}

//////////////////////////////////////////////////////////////////////////////
//
Motion::Motion() {
  right = false;
  left = false;
}

//////////////////////////////////////////////////////////////////////////////
//
Ship::Ship(Sprite* s,frames) : ComObj(s) {
  this.frames=frames;
}

//////////////////////////////////////////////////////////////////////////////
//
Velocity::Velocity(float vx, float vy) {
  x = vx;
  y= vy;
}


NS_END(invaders)

