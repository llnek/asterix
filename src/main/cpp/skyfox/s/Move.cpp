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
NS_BEGIN(skyfox)

//////////////////////////////////////////////////////////////////////////////
//
void Move::preamble() {

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
  auto po= MGMS()->getPool("Clouds");
  auto wb= cx::visBox();
  auto c= po->ls();

  F__LOOP(it,c) {
    auto e= *it;
    auto co=CC_GEC(Cloud,e,"f/CPixie");
    auto z= co->csize();
    co->node->setPositionX(co->node->getPositionX() + dt * 20);
    if (co->node->getPositionX() > wb.right + HWZ(z)) {
      co->node->setPositionX(- HWZ(z));
    }
  }
}



NS_END




