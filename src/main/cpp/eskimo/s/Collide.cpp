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
#include "Collide.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(eskimo)


//////////////////////////////////////////////////////////////////////////////
//
void Collide::preamble() {
  shared=engine->getNodeList(SharedNode().typeId());
  players=engine->getNodeList(EskimoNode.typeId());
}

//////////////////////////////////////////////////////////////////////////////
//
bool Collide::update(float dt) {
  if (MGMS()->isLive()) {
    process(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Collide::process(float dt) {
  auto &sws = MGMS()->getPool("Switches")->list();
  auto py= CC_GNLF(Eskimo,players,"player");
  auto ss= CC_GNLF(GVars,shared,"slots");
  auto player= (EskimoSprite*) py->node;
  float dx,dy;
  float x,y;
  F__LOOP(it, sws) {
    auto gw= (*it)->node;
    if (!gw->isVisible())
    continue;
    dx = cx::deltaX(gw,player);
    dy = cx::deltaY(gw,player);
    if (pow(dx, 2) + pow (dy, 2) < PLAYER_SWITCH_RADII) {
      ss->gravity = gw->direction;
      CC_HIDE(gw);
      cx::sfxPlay("switch");
      //notify of collision
      EVENT_DISPATCHER->dispatchCustomEvent(NOTIFICATION_GRAVITY_SWITCH);
      x=0;
      y=0;
      //change world gravity
      switch (ss->gravity) {
        case kDirectionUp: y= FORCE_GRAVITY; break;
        case kDirectionDown: y= -FORCE_GRAVITY; break;
        case kDirectionLeft: x= -FORCE_GRAVITY; break;
        case kDirectionRight: x= FORCE_GRAVITY; break;
      }
      ss->world->SetGravity(b2Vec2(x,y));
      break;
    }
  }

  //check for level complete (collision with Igloo)
  dx = cx::deltaX(player, igloo);
  dy = cx::deltaY(player, igloo);
  if (pow(dx, 2) + pow(dy, 2) < IGLOO_SQ_RADIUS) {
    cx::sfxPlay("igloo");
    CC_HIDE(player);
    CC_HIDE(ss->btnPause);
    CC_HIDE(ss->btnReset);

    //notify of Level Completed. Igloo will listen to this and change its texture.
    EVENT_DISPATCHER->dispatchCustomEvent(NOTIFICATION_LEVEL_COMPLETED);

    //run smoke particle
    CC_SHOW(ss->smoke);
    ss->smoke->resetSystem();

    auto msg= j::json({
        {"msg", "well done!"}
        });
    SENDMSGEX("/game/hud/showmsg", &msg);
    //_messages->setString("well done!");
    //CC_SHOW(_messages);

    SENDMSG("/game/player/newlevel");
    //create delay until new level is loaded
    /*
    auto seq= c::Sequence::create(
        c::DelayTime::create(2.5f),
        c::CallFunc::create([=]() {
          this->newLevel();
          }),
        CC_NIL);
    this->runAction(seq);
    CC_HIDE(_tutorialLabel);
    MGMS()->stop();
    */
  }



}



NS_END




