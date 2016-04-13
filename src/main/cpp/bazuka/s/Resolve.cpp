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
#include "Resolve.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(bazuka)


//////////////////////////////////////////////////////////////////////////////
//
void Resolve::preamble() {
  _player= _engine->getNodes("f/CGesture")[0];
  _shared= _engine->getNodes("n/GVars")[0];
}

//////////////////////////////////////////////////////////////////////////////
//
bool Resolve::update(float dt) {
  if (MGMS()->isLive()) {
    process(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::process(float dt) {
  auto hero= CC_GEC(Hero,_player,"f/CPixie");
  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  auto sz= hero->csize();
  auto pos= hero->pos();
  auto wb= cx::visBox();

  float maxY = wb.top - HHZ(sz);
  float minY = HHZ(sz);
  float newY = MIN(MAX(pos.y, minY), maxY);

  hero->setPos(pos.x, newY);

  if (hero->jump) {
    hero->jumpTimer = 10;
    hero->jump = false;
  }

  pos=hero->pos();

  if (hero->jumpTimer>0) {
    hero->state = kPlayerStateBoost;
    --hero->jumpTimer;
    hero->node->setPosition( c::ccpAdd(pos, c::Vec2(0,7)));
  } else {
    hero->state = kPLayerStateIdle;
    hero->jumpTimer = 0;
    hero->node->setPosition(c::ccpAdd(pos, ss->gravity));
  }

}


NS_END




