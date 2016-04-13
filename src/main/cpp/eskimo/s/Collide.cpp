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
#include "Box2D/Box2D.h"
#include "core/CCSX.h"
#include "n/GSwitch.h"
#include "n/Eskimo.h"
#include "n/Igloo.h"
#include "Collide.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(eskimo)


//////////////////////////////////////////////////////////////////////////////
//
void Collide::preamble() {
  _player= _engine->getNodes("f/CGesture")[0];
  _shared= _engine->getNodes("n/GVars")[0];
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
  auto sws = MGMS()->getPool("Switches")->ls();
  auto py= CC_GEC(Eskimo,_player,"f/CPixie");
  auto ss= CC_GEC(GVars,_shared,"n/GVars");
  auto player= (EskimoSprite*) py->node;
  float dx,dy;
  float x,y;
  F__LOOP(it, sws) {
    auto e= *it;
    auto gw= CC_GEC(GSwitch,e,"f/CPixie");
    if (!gw->isOvert()) {
    continue; }
    dx = cx::deltaX(gw->node,player);
    dy = cx::deltaY(gw->node,player);
    if (pow(dx,2) + pow (dy,2) < PLAYER_SWITCH_RADII) {
      ss->gravity = gw->direction;
      gw->hide();
      cx::sfxPlay("switch");
      //notify of collision
      CC_DISPEVENT1(NOTIFY_GSWITCH);
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
  dx = cx::deltaX(player, ss->igloo);
  dy = cx::deltaY(player, ss->igloo);
  if (pow(dx,2) + pow(dy,2) < IGLOO_SQ_RADIUS) {
    cx::sfxPlay("igloo");
    CC_HIDE(player);
    auto m2=j::json({ {"tag", kSpriteBtnReset } });
    auto m1=j::json({ {"tag", kSpriteBtnPause } });
    SENDMSGEX("/game/hud/togglebutton->off", &m2);
    SENDMSGEX("/game/hud/togglebutton->off", &m1);

    //notify of Level Completed. Igloo will listen to this and change its texture.
    CC_DISPEVENT1(NOTIFY_LEVEL_DONE);

    //run smoke particle
    CC_SHOW(ss->smoke);
    ss->smoke->resetSystem();

    auto msg= j::json({ {"msg", "well done!"} });
    SENDMSGEX("/game/hud/showmsg", &msg);

    SENDMSG("/game/player/goto/newlevel");
  }

}



NS_END




