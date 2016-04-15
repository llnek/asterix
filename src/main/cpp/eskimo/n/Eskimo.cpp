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

#include "core/XConfig.h"
#include "core/COMP.h"
#include "core/CCSX.h"
#include "Eskimo.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(eskimo)

//////////////////////////////////////////////////////////////////////////////
//
owner<Eskimo*> Eskimo::create(not_null<GVars*> ss) {
  auto sp = mc_new1(Eskimo, ss);
  sp->initWithSpriteFrameName("player_circle.png");
  sp->autorelease();
  MGML()->addAtlasItem("game-pics", sp, kMiddleground);
  return sp;
}

//////////////////////////////////////////////////////////////////////////////
//
bool Eskimo::initWithSpriteFrameName(const sstr &fn) {
  bool rc= c::Sprite::initWithSpriteFrameName(fn);
  if (!rc) { return false; }
  //create body
  b2BodyDef bodyDef; bodyDef.type = b2_dynamicBody;
  _body = ss->world->CreateBody(&bodyDef);
  _body->SetSleepingAllowed(false);
  _body->SetUserData(this);
  makeCircleShape();
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Eskimo::reset() {

  if (_shape != kStateCircle) {
    makeCircleShape();
  }

  _body->SetTransform(_body->GetPosition(),0.0);
  _body->SetLinearVelocity(b2Vec2_zero);

  setVisible(true);
  setRotation(0);
}

//////////////////////////////////////////////////////////////////////////////
//
void Eskimo::update() {

  if (_switchShape) {
    cx::sfxPlay("shape");
    if (_shape == kStateBox) {
      //switch to circle
      makeCircleShape();
    } else {
      //switch to box
      makeBoxShape();
    }
    _switchShape = false;
  }

  b2Sprite::update();
}

//////////////////////////////////////////////////////////////////////////////
//
void Eskimo::makeCircleShape() {

  //Define fixture
  //Define shape
  b2CircleShape  circle; circle.m_radius = PLAYER_RADIUS/PTM_RATIO;
  b2FixtureDef fixtureDef;
  fixtureDef.shape = &circle;
  fixtureDef.density = 1;
  fixtureDef.restitution = 0.4;
  fixtureDef.friction = 10;

  auto tmp= _body->GetFixtureList();
  if (tmp) {
    _body->DestroyFixture(tmp); }

  _body->CreateFixture(&fixtureDef);
  _shape = kStateCircle;

  this->setDisplayFrame(cx::getSpriteFrame("player_circle.png"));
}

//////////////////////////////////////////////////////////////////////////////
//
void Eskimo::makeBoxShape() {

  //Define fixture
  //Define shape
  b2PolygonShape box; box.SetAsBox(PLAYER_RADIUS /PTM_RATIO,
      PLAYER_RADIUS/ PTM_RATIO);
  b2FixtureDef fixtureDef;
  fixtureDef.shape = &box;
  fixtureDef.density = 10;
  fixtureDef.restitution = 0;
  fixtureDef.friction = 0;

  auto tmp= _body->GetFixtureList();
  if (tmp) {
    _body->DestroyFixture(tmp); }

  _body->CreateFixture(&fixtureDef);
  _shape = kStateBox;

  this->setDisplayFrame(cx::getSpriteFrame("player_box.png"));
}



NS_END



