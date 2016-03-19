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
#include "Move.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(monsters)


//////////////////////////////////////////////////////////////////////////////
//
void MoveLogic::preamble() {

}

//////////////////////////////////////////////////////////////////////////////
//
bool MoveLogic::update(float dt) {
  if (MGMS()->isLive()) {
    process(dt);
  }
  return true;
}

void MoveLogic::onKeys(float dt) {
}

//////////////////////////////////////////////////////////////////////////////
//
void MoveLogic::process(float dt) {

  auto ents = _engine->getNodes(
      s_vec<ecs::COMType>{"f/CMove","f/CPixie","n/Team"});
  auto wz= cx::visRect();

  F__LOOP(it,ents) {
    auto e= *it;
    auto render = CC_GEC(f::CPixie,e,"f/CPixie");
    auto move = CC_GEC(f::CMove,e,"f/CMove");
    auto team = CC_GEC(Team,e,"n/Team");

    auto arrivePt = arrive(e,move,render);
    auto sepPt = separate(e,move,render,team);
    auto newAccel= c::ccpAdd(arrivePt, sepPt);

    // Update current acceleration based on the above, and clamp
    move->acc = c::ccpAdd(move->acc, newAccel);
    if (c::ccpLength(move->acc) > move->maxAccel.x) {
      move->acc = c::ccpMult(c::ccpNormalize(move->acc), move->maxAccel.x);
    }

    // Update current velocity based on acceleration and dt, and clamp
    move->vel= c::ccpAdd(move->vel, c::ccpMult(move->acc, dt));
    if (c::ccpLength(move->vel) > move->maxSpeed.x) {
      move->vel= c::ccpMult(c::ccpNormalize(move->vel), move->maxSpeed.x);
    }

    // Update position based on velocity
    auto newPos= c::ccpAdd(render->pos(), c::ccpMult(move->vel, dt));
    newPos.y = MAX(MIN(newPos.y, wz.size.height), 0);
    newPos.x = MAX(MIN(newPos.x, wz.size.width), 0);
    render->node->setPosition(newPos);
  }
}

static float timeToTarget = 0.1;
//////////////////////////////////////////////////////////////////////////////
//
c::Vec2 MoveLogic::arrive(ecs::Node *entity, f::CMove *move, f::CPixie *render) {

  auto vector = c::ccpSub(move->moveTarget, render->pos());
  auto dist = c::ccpLength(vector);

  float targetRadius = 5;
  float slowRadius = targetRadius + 25;

  if (dist < targetRadius) {
    return c::Vec2(0,0);
  }

  float targetSpeed;
  if (dist > slowRadius) {
    targetSpeed = move->maxSpeed.x;
  } else {
    targetSpeed = move->maxSpeed.x * dist / slowRadius;
  }

  auto targetVelocity = c::ccpMult(c::ccpNormalize(vector), targetSpeed);

  auto acceleration = c::ccpMult(c::ccpSub(targetVelocity, move->vel), 1/timeToTarget);
  if (c::ccpLength(acceleration) > move->maxAccel.x) {
    acceleration = c::ccpMult(c::ccpNormalize(acceleration), move->maxAccel.x);
  }
  return acceleration;
}

static float SEPARATE_THRESHHOLD = 20;
//////////////////////////////////////////////////////////////////////////////
//
c::Vec2 MoveLogic::separate(ecs::Node *entity, f::CMove *move, f::CPixie *render, Team *team) {

  auto ents = getEntsOnTeam(_engine,team->team,"f/CMove");
  auto steering = c::Vec2(0,0);
  F__LOOP(it,ents) {
    auto otherEntity = *it;

    if (otherEntity->getEid() == entity->getEid()) {
    continue; }

    auto otherRender = CC_GEC(f::CPixie,otherEntity,"f/CPixie");
    if (!otherRender) { continue; }

    auto direction = c::ccpSub(render->pos(), otherRender->pos());
    auto dist = c::ccpLength(direction);

    if (dist < SEPARATE_THRESHHOLD) {
      direction = c::ccpNormalize(direction);
      steering = c::ccpAdd(steering, c::ccpMult(direction, move->maxAccel.x));
    }
  }

  return steering;
}


NS_END




