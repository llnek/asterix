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

#include "core/XConfig.h"
#include "core/CCSX.h"
#include "s/GEngine.h"
#include "HUD.h"
#include "Game.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(hockey)
BEGIN_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
struct CC_DLL GLayer : public f::GameLayer {

  HUDLayer* getHUD() {
    return (HUDLayer*)getSceneX()->getLayer(3); }

  STATIC_REIFY_LAYER(GLayer)
  MDECL_DECORATE()
  MDECL_GET_IID(2)

  virtual bool onTouchStart(const s_vec<c::Touch*>& );
  virtual void onTouchMotion(const s_vec<c::Touch*>& );
  virtual void onTouchEnd(const s_vec<c::Touch*>& );

  void updateScore(int,int);
  virtual void onInited();

  DECL_PTR(ecs::Node, _shared)
  DECL_PTR(ecs::Node, _puck)
  s_vec<ecs::Node*> _mallets;

};

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onTouchStart(const s_vec<c::Touch*> &touches) {
  F__LOOP(it2, _mallets) {
    auto e2= *it2;
    auto m=CC_GEC(Mallet,e2,"f/CPixie");
    F__LOOP(it,touches) {
      auto t = *it;
      if (m->bbox().containsPoint(t->getLocation())) {
        m->tap= t;
      }
    }
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchMotion(const s_vec<c::Touch*> &touches) {
  auto wb= cx::visBox();
  F__LOOP(it2, _mallets) {
    auto e2= *it2;
    auto mv=CC_GEC(f::CMove,e2,"f/CMove");
    auto m=CC_GEC(Mallet,e2,"f/CPixie");
    F__LOOP(it,touches) {
      auto t = *it;
      auto tap = t->getLocation();
      if (m->tap == t) {
        auto r= m->radius();
        auto cp= m->pos();
        auto npos= tap;
        //clamp
        if (npos.y < r) { npos.y  = r; }
        if (npos.x < r) { npos.x = r; }
        if (npos.x > wb.right - r) { npos.x = wb.right - r; }
        if (npos.y > wb.top - r) { npos.y = wb.top - r; }
        //keep player inside its court
        if (cp.y < wb.cy) {
          if (npos.y > wb.cy - r) {
            npos.y = wb.cy - r;
          }
        } else {
          if (npos.y < wb.cy + r) {
            npos.y = wb.cy + r;
          }
        }

        mv->moveTarget= npos;
        mv->vel= c::Vec2(tap.x - cp.x, tap.y - cp.y);
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchEnd(const s_vec<c::Touch*> &touches) {
  F__LOOP(it2,_mallets) {
    auto e2= *it2;
    auto mv=CC_GEC(f::CMove,e2,"f/CMove");
    auto m=CC_GEC(Mallet,e2,"f/CPixie");
    F__LOOP(it,touches) {
      auto t= *it;
      if (m->tap == t) {
        mv->vel= CC_ZPT;
        m->tap= CC_NIL;
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::updateScore(int player, int score) {
  auto bv= CC_GEC(f::CMove,_puck,"f/CMove");
  auto ball= CC_GEC(Puck,_puck,"f/CPixie");
  auto bc= ball->circum();
  auto wb= cx::visBox();

  getHUD()->updateScore(player, score);
  bv->vel= CC_ZPT;

  if (player == 1) {
        //move ball to player 2 court
    bv->moveTarget= c::Vec2(wb.cx, wb.cy + bc);
  } else {
      //move ball to player 1 court
    bv->moveTarget = c::Vec2(wb.cx, wb.cy - bc);
  }

  F__LOOP(it,_mallets) {
    auto e= *it;
    auto mv= CC_GEC(f::CMove,e,"f/CMove");
    auto m= CC_GEC(Mallet,e,"f/CPixie");
    auto p= CC_GEC(Player,e,"f/CStats");
    auto c= m->circum();
    if (p->value == 1) {
      m->setPos(wb.cx, mc);
    } else {
      m->setPos(wb.cx, wb.top - mc);
    }
    m->tap=CC_NIL;
  }

  SCAST(GEngine*,_engine)->readyPt(_mallets,_puck);
}

//////////////////////////////////////////////////////////////////////////////
void GLayer::onInited() {
  _puck = _engine->getNodes("f/CAutoma")[0];
  _shared = _engine->getNodes("n/GVars")[0];
  _engine->getNodes("f/CGesture",_mallets);

  auto mr=CC_GNLF(Mallet,mallets,"mallet")->radius();
  auto br=CC_GNLF(Puck,pucks,"puck")->radius();
  auto ss=CC_GNLF(GVars,shared,"slots");

  ss->sq_radii = pow(mr+br, 2);
  ss->goalWidth= 400;

  for (auto node=mallets->head;node;node=node->next) {
    auto m=CC_GNF(Mallet,node,"mallet");
    this->motionees.push_back(m);
  }

  SCAST(GEngine*,engine)->readyPoint(mallets,pucks->head);
}

//////////////////////////////////////////////////////////////////////////////
void GLayer::decorate() {
  engine = mc_new(GEngine);
  centerImage("game.bg");
}

END_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic &topic, void *m) {
  auto y= (GLayer*) getGLayer();

  if (topic == "/game/player/earnscore") {
    auto msg= (j::json*) m;
    y->updateScore(
        JS_INT(msg->operator[]("pnum")),
        JS_INT(msg->operator[]("score")));
  }

}

//////////////////////////////////////////////////////////////////////////////
void Game::decorate() {
  HUDLayer::reify(this, 3);
  GLayer::reify(this, 2);
  play();
}



NS_END




