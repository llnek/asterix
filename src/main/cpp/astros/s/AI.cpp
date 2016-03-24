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
#include "AI.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(astros)

//////////////////////////////////////////////////////////////////////////////
//
void AI::preamble() {
  _shared= _engine->getNodes("n/GVars")[0];
  spawnAstro(0);
}

//////////////////////////////////////////////////////////////////////////////
//
bool AI::update(float dt) {
  if (MGMS()->isLive()) {
    parallex(dt);
    process(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void AI::parallex(float dt) {
  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  auto bg= ss->background;
  auto wb= cx::visBox();

  bg->setPosition(bg->getPositionX() - ss->scrollSpeed, bg->getPositionY());
  if(bg->getPositionX() < wb.left) {
    bg->setPosition(bg->getPositionX()+480,bg->getPositionY());
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void AI::process(float dt) {

  if (!cx::timerDone(_astroTimer)) { return; }

  cx::undoTimer(_astroTimer);
  _astroTimer=CC_NIL;
  spawnAstro(dt);
}

//////////////////////////////////////////////////////////////////////////////
//
void AI::spawnAstro(float dt) {
  return;
  auto po= MGMS()->getPool("Astros");
    auto e= (ecs::Node*)po->take(true);
  auto wb= cx::visBox();
  auto a= CC_GEC(Asteroid,e,"f/CPixie");

  cx::resurrect(e,600, cx::randInt(wb.top));
  a->runAction(
      c::MoveTo::create(2.5,
        c::Vec2(wb.left -100, cx::randInt(wb.top))));

  _astroTimer=cx::reifyTimer(MGML(),800);
}



NS_END



