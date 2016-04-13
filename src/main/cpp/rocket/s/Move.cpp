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
  _rocket= _engine->getNodes("f/CGesture")[0];
  _drawing= _engine->getNodes("n/RPath")[0];
  _shared= _engine->getNodes("n/GVars")[0];
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
  auto rock=CC_GEC(Rocket,_rocket,"f/CPixie");
  auto dw=CC_GEC(RPath,_drawing,"n/RPath");
  auto ss=CC_GEC(GVars,_shared,"n/GVars");

  processRocket(dt);

  if (dw->lineType != LINE_NONE) {
    dw->tip= rock->pos();
  }

  if (!ss->jet->isActive()) {
    ss->jet->resetSystem();
  }
  ss->jet->setRotation(rock->getRotation());
  ss->jet->setPosition(rock->pos());

  dw->update(dt);
  rock->setOpacity(dw->energy * 255);
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
      rotatedAngle = atan2 (-clockwise.y, -clockwise.x);
    } else {
      rotatedAngle = atan2 (clockwise.y, clockwise.x);
    }

    //update rocket vector
    mv->vel.x = mv->speed.x * cos(rotatedAngle);
    mv->vel.y = mv->speed.y * sin(rotatedAngle);
    mv->setRotationFromVector();

    //wrap rotation values to 0-360 degrees
    if (rock->getRotation() > 0) {
      angle= 360;
    } else {
      angle= -360;
    }
    rock->setRotation(
        fmodf(rock->getRotation(), angle));
  }

  if (mv->targetRotation > rock->getRotation() + 180) {
    mv->targetRotation -= 360;
  }
  if (mv->targetRotation < rock->getRotation() - 180) {
    mv->targetRotation += 360;
  }

  CC_POS1(rock, pos);

  mv->dr = mv->targetRotation - rock->getRotation() ;
  mv->ar = mv->dr * mv->rotationSpring;
  mv->vr += mv->ar ;
  mv->vr *= mv->rotationDamping;

  auto rotationNow = rock->getRotation();
  rotationNow += mv->vr;
  rock->setRotation(rotationNow);
}

//////////////////////////////////////////////////////////////////////////////
//
bool Move::collidedWithSides() {

  auto mv=CC_GEC(RocketMotion,_rocket,"f/CMove");
  auto rock=CC_GEC(Rocket,_rocket,"f/CPixie");
  auto r= rock->radius();
  auto pos= rock->pos();
  auto wz = cx::visSize();
  auto wb=cx::visBox();

  if (pos.x > wb.right - r) {
    mv->vel = CCT_PT(-mv->vel.x, mv->vel.y);
    mv->rotationOrientation = ROTATE_NONE;
    rock->setPositionX(wb.right-r);
    mv->setRotationFromVector();
    return true;
  }

  if (pos.x < r) {
    mv->vel = CCT_PT(-mv->vel.x, mv->vel.y);
    mv->rotationOrientation = ROTATE_NONE;
    rock->setPositionX(r);
    mv->setRotationFromVector();
    return true;
  }

  if (pos.y < r) {
    mv->vel = CCT_PT(mv->vel.x, -mv->vel.y);
    mv->rotationOrientation = ROTATE_NONE;
    rock->setPositionY(r);
    mv->setRotationFromVector();
    return true;
  }

  if (pos.y > wb.top - r) {
    mv->vel = CCT_PT(mv->vel.x, -mv->vel.y );
    mv->rotationOrientation = ROTATE_NONE;
    rock->setPositionY(wb.top - r);
    mv->setRotationFromVector();
    return true;
  }

  return false;
}


NS_END




