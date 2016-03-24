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
#include "Resolve.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(astros)


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

  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  auto po= MGMS()->getPool("Astros");
  auto ps= po->ls();
  auto wb= cx::visBox();

  F__LOOP(it,ps) {
      auto e= (ecs::Node*)*it;
    auto h=CC_GEC(f::CHealth,e,"f/CHealth");
    auto a=CC_GEC(Asteroid,e,"f/CPixie");
    if(!h->alive() ||
       a->getPositionX() < wb.left -50) {
      cx::hibernate(e);
    }
  }
}

NS_END




