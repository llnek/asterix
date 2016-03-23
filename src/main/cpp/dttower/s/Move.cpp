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
#include "n/Defense.h"
#include "n/Enemy.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(dttower)


//////////////////////////////////////////////////////////////////////////////
//
void Move::preamble() {
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

  auto po= MGMS()->getPool("Defense1");
  processDefense(po,dt);
  po= MGMS()->getPool("Defense2");
  processDefense(po,dt);

  po= MGMS()->getPool("Enemies");
  auto p1= po->ls();
  F__LOOP(it,p1) {
    auto e= *it;
    auto n= CC_GEC(Enemy,e,"f/CPixie");
    if (e->status()) {
      n->update(dt);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::processDefense(f::FPool *po, float dt) {
  auto p1= po->ls();
  F__LOOP(it,p1) {
    auto e= *it;
    auto d= CC_GEC(Defense,e,"f/CPixie");
    if (e->status()) {
      d->update(dt);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::onKeys(float dt) {
}


NS_END




