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
NS_BEGIN(rocket)


//////////////////////////////////////////////////////////////////////////////
//
void Move::preamble() {
  drawings=engine->getNodeList(LineDrawingNode().typeId());
  shared=engine->getNodeList(SharedNode().typeId());
  rockets=engine->getNodeList(RocketNode().typeId());
}

//////////////////////////////////////////////////////////////////////////////
//
bool Move::update(float dt) {
  if (MGMS()->isLive()) {
    process(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::process(float dt) {
  auto dw=CC_GNLF(LineDrawing,drawings,"drawing");
  auto ss=CC_GNLF(GVars,shared,"slots");
  auto rock=CC_GNLF(Rocket,rockets,"rocket");

  processRocket(dt);

  if (dw->lines->lineType != LINE_NONE) {
    dw->lines->tip= rock->pos();
  }

  if (!ss->jet->isActive()) {
    ss->jet->resetSystem();
  }
  ss->jet->setRotation(rock->node->getRotation());
  ss->jet->setPosition(rock->pos());

  dw->lines->update(dt);
  rock->node->setOpacity(dw->lines->energy * 255);
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::processRocket(float dt) {
  auto mv=CC_GEC(RocketMotion,_rocket,"f/CMove");
  auto rock=CC_GEC(Rocket,_rocket,"f/CPixie");
  auto pos= rock->pos();

  collidedWithSides();

  if (mv->rotationOrientation == ROTATE_NONE) {
    pos.x += mv->vel.x * dt;
    pos.y += mv->vel.y * dt;
  } else {
    //rotate point around a pivot by a certain amount (rotation angle)
    auto angle = mv->angularSpeed * dt;
    auto rotatedPoint = pos.rotateByAngle(mv->pivot, angle);

    pos.x = rotatedPoint.x;
    pos.y = rotatedPoint.y;

    auto diff = pos;
    diff.subtract(mv->pivot);

    auto clockwise = diff.getRPerp();
    float rotatedAngle;
    if (mv->rotationOrientation == ROTATE_COUNTER) {
      rotatedAngle = atan2 (-1 * clockwise.y, -1 * clockwise.x);
    } else {
      rotatedAngle = atan2 (clockwise.y, clockwise.x);
    }

    //update rocket vector
    mv->vel.x = mv->speed.x * cos(rotatedAngle);
    mv->vel.y = mv->speed.y * sin(rotatedAngle);
    mv->setRotationFromVector();

    //wrap rotation values to 0-360 degrees
    if (rock->node->getRotation() > 0) {
      angle= 360;
    } else {
      angle= -360;
    }
    rock->node->setRotation(
        fmodf(rock->node->getRotation(), angle));
  }

  if (mv->targetRotation > rock->node->getRotation() + 180) {
    mv->targetRotation -= 360;
  }
  if (mv->targetRotation < rock->node->getRotation() - 180) {
    mv->targetRotation += 360;
  }

  rock->node->setPosition(pos);

  mv->dr = mv->targetRotation - rock->node->getRotation() ;
  mv->ar = mv->dr * mv->rotationSpring;
  mv->vr += mv->ar ;
  mv->vr *= mv->rotationDamping;

  auto rotationNow = rock->node->getRotation();
  rotationNow += mv->vr;
  rock->node->setRotation(rotationNow);
}

//////////////////////////////////////////////////////////////////////////////
//
bool Move::collidedWithSides() {

  auto mv=CC_GEC(RocketMotion,_rocket,"f/CMove");
  auto rock=CC_GEC(Rocket,_rocket,"f/CPixie");
  auto pos= rock->pos();
  auto r= rock->radius();
  auto wz = cx::visRect();
  auto wb=cx::visBox();

  if (pos.x > wb.right - r) {
    mv->vel = c::Vec2(mv->vel.x * -1, mv->vel.y);
    mv->rotationOrientation = ROTATE_NONE;
    rock->node->setPositionX(wb.right-r);
    mv->setRotationFromVector();
    return true;
  }

  if (pos.x < r) {
    mv->vel = c::Vec2(mv->vel.x * -1, mv->vel.y);
    mv->rotationOrientation = ROTATE_NONE;
    rock->node->setPositionX(r);
    mv->setRotationFromVector();
    return true;
  }

  if (pos.y < r) {
    mv->vel = c::Vec2(mv->vel.x, mv->vel.y * -1);
    mv->rotationOrientation = ROTATE_NONE;
    rock->node->setPositionY(r);
    mv->setRotationFromVector();
    return true;
  }

  if (pos.y > wb.top - r) {
    mv->vel = c::Vec2(mv->vel.x, mv->vel.y * -1);
    mv->rotationOrientation = ROTATE_NONE;
    rock->node->setPositionY(wb.top - r);
    mv->setRotationFromVector();
    return true;
  }

  return false;
}

NS_END




