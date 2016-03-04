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

#include "Health.h"
NS_BEGIN(monsters)

//////////////////////////////////////////////////////////////////////////////
//
void HealthSystem::preamble() {
  shared=engine->getNodeList(SharedNode().typeId());
}

//////////////////////////////////////////////////////////////////////////////
//
bool HealthSystem::update(float dt) {
  if (MGMS()->isLive()) {
    process(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void HealthSystem::process(float dt) {

  auto ents = engine->getNodesWith("n/Health");
  F__LOOP(it, ents) {
    auto e= *it;
    if (!e->status()) {
    continue; }
    auto health = (Health*)e->get("n/Health");
    auto render = (Render*)e->get("n/Render");

    if (!health->alive) { return; }
    if (health->maxHp == 0) { return; }
    if (health->curHp <= 0) {
      health->alive = false;
      cx::sfxPlay("boom");
      if (render) {
        render->node->runAction(
            c::Sequence::create(
            c::FadeOut::create(0.5),
            c::RemoveSelf::create(true),
            c::CallFunc::create([=](){
              this->engine->deflate(e,true);
              }),
            CC_NIL));
      } else {
        this->engine->deflate(e,true);
      }
    }
  }
}

static int colorBuffer = 55;
static int maxColor = 200;
//////////////////////////////////////////////////////////////////////////////
//
void HealthSystem::draw() {
  auto ents = engine->getNodesWith("n/Health");
  F__LOOP(it,ents) {
    auto e= *it;
    if (!e->status()) {
    continue; }
    auto health = (Health*)e->get("n/Health");
    auto render = (Render*)e->get("n/Render");
    if (ENP(health) || ENP(render)) {
    continue; }
    auto sX = render->node->getPositionX() - HWZ(CC_CSIZE(render->node));
    auto eX = render->node->getPositionX() + HWZ(CC_CSIZE(render->node));
    auto actualY = render->node->getPositionY() + HHZ(CC_CSIZE(render->node));

    auto percentage =
      ((float) health->curHp) / ((float) health->maxHp);

    int actualX = ((eX-sX) * percentage) + sX;
    int amtRed = ((1.0f-percentage)*maxColor)+colorBuffer;
    int amtGreen = (percentage*maxColor)+colorBuffer;

    glLineWidth(7);
    ccDrawColor4B(amtRed,amtGreen,0,255);
    ccDrawLine(c::Vec2(sX, actualY), c::Vec2(actualX, actualY));
  }
}


NS_END



