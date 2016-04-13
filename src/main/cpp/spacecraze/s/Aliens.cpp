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

  auto squad= CC_GEC(AlienSquad,_aliens,"n/AlienSquad");
  auto maxSz= cx::calcSize("sfenmy3");
  auto stepx = HWZ(maxSz);
  auto wb= cx::visBox();
  auto deltaRt = wb.right - squad->getRightEdge();
  auto deltaLf= squad->getLeftEdge() - wb.left;
  auto maxright = (int) floor(deltaRt/stepx);
  auto maxleft = (int) floor(deltaLf/stepx);

  auto moveleft = c::Sequence::create(
      c::DelayTime::create(squad->getDuration()),
      c::EaseSineOut::create(
        c::MoveBy::create(
          0.25, CCT_PT(stepx*-1, 0))),
      CC_NIL);

  auto moveright = c::Sequence::create(
      c::DelayTime::create(squad->getDuration()),
      c::EaseSineOut::create(
        c::MoveBy::create(
          0.25, CCT_PT(stepx, 0))),
      CC_NIL);

  auto moveseq= c::Sequence::create(
    c::Repeat::create(moveleft, maxleft),
    c::Repeat::create(moveright, maxleft),
    c::Repeat::create(moveright, maxright),
    c::Repeat::create(moveleft, maxright),
    CC_NIL);

  auto po= MGMS()->getPool("Aliens");
  po->foreach([=](f::Poolable *p) {
    auto ui=CC_GEC(Alien,p,"f/CPixie");
    ui->runAction(
        c::RepeatForever::create(
          (c::ActionInterval*) moveseq->clone() ));
  });
}

//////////////////////////////////////////////////////////////////////////////
//
bool Aliens::update(float dt) {
  return true;
}


NS_END





