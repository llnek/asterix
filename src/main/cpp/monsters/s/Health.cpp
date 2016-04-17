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

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Health.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(monsters)

//////////////////////////////////////////////////////////////////////////////
//
void HealthLogic::preamble() {

}

//////////////////////////////////////////////////////////////////////////////
//
bool HealthLogic::update(float dt) {
  if (MGMS()->isLive()) {
    process(dt);
    //draw();
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void HealthLogic::process(float dt) {
  auto ents = _engine->getNodes("f/CHealth");
  F__LOOP(it,ents) {
    auto e= *it;
    auto health = CC_GEC(f::CHealth,e,"f/CHealth");
    auto render = CC_GEC(f::CPixie,e,"f/CPixie");

    if (!health->alive()) {

      cx::sfxPlay("boom");

      if (render) {
        render->runAction(
           c::Sequence::create(
            c::FadeOut::create(0.5),
            c::RemoveSelf::create(),
            CC_NIL));
      }

      _engine->purgeNode(e);
    }
  }
}

static int colorBuffer = 55;
static int maxColor = 200;
//////////////////////////////////////////////////////////////////////////////
//
void HealthLogic::draw() {

  auto ents = _engine->getNodes(
      s_vec<ecs::COMType>{ "f/CHealth","f/CPixie"});
  F__LOOP(it,ents) {
    auto e= *it;
    auto health = CC_GEC(f::CHealth,e,"f/CHealth");
    auto render = CC_GEC(f::CPixie,e,"f/CPixie");
    auto sX = render->getPositionX() - HWZ(CC_CSIZE(render));
    auto eX = render->getPositionX() + HWZ(CC_CSIZE(render));
    auto actualY = render->getPositionY() + HHZ(CC_CSIZE(render));

    auto percentage = (float)health->curHP / (float)health->origHP;
    auto actualX = ((eX-sX) * percentage) + sX;
    auto amtRed = ((1.0f-percentage)*maxColor)+colorBuffer;
    auto amtGreen = (percentage*maxColor)+colorBuffer;

    //glLineWidth(7);
    //c::ccDrawColor4B(amtRed,amtGreen,0,255);
    //c::ccDrawLine(CCT_PT(sX, actualY), CCT_PT(actualX, actualY));
  }
}


NS_END



