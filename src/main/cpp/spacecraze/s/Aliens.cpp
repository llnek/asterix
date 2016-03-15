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

#pragma once

//////////////////////////////////////////////////////////////////////////////
#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "GEngine.h"
#include "Aliens.h"
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(spacecraze)


//////////////////////////////////////////////////////////////////////////////
//
void Aliens::preamble() {
  _aliens = _engine->getNodes("n/AlienSquad")[0];
  startEnemies();
}

//////////////////////////////////////////////////////////////////////////////
//
void Aliens::startEnemies() {

  auto squad= CC_GEC(AlienSquad,aliens,"n/AlienSquad");
  auto maxSz= cx::calcSize("sfenmy3");
  auto stepx = HWZ(maxSz);
  auto wb= cx::visBox();
  auto deltaRt = wb.right - squad->rightEdge;
  auto deltaLf= squad->leftEdge - wb.left;

  auto max_moves_right = (int) floor(deltaRt/stepx);
  auto max_moves_left = (int) floor(deltaLf/stepx);

  auto move_left = c::Sequence::create(
      c::DelayTime::create(squad->duration),
      c::EaseSineOut::create(
        c::MoveBy::create(0.25, c::Vec2(stepx*-1, 0))),
      CC_NIL);

  auto move_right = c::Sequence::create(
      c::DelayTime::create(squad->duration),
      c::EaseSineOut::create(
        c::MoveBy::create(0.25, c::Vec2(stepx, 0))),
      CC_NIL);

  auto move_left_to_start = c::Repeat::create(move_right, max_moves_left);
  auto move_start_to_left = c::Repeat::create(move_left, max_moves_left);

  auto move_start_to_right = c::Repeat::create(move_right, max_moves_right);
  auto move_right_to_start = c::Repeat::create(move_left, max_moves_right);

  auto movement_sequence = c::Sequence::create(move_start_to_left, move_left_to_start, move_start_to_right, move_right_to_start, CC_NIL);

  auto pool = MGMS()->getPool("Aliens");
  pool->foreach([=](f::Poolable *p) {
    auto ui=CC_GEC(Alien,p,"f/CPixie");
    ui->node->runAction(
        c::RepeatForever::create(
          (c::ActionInterval*)
          movement_sequence->clone() ));
  });
}

//////////////////////////////////////////////////////////////////////////////
//
bool Aliens::update(float dt) {
  return true;
}


NS_END





