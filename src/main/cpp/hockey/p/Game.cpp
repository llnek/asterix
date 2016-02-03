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
  MDECL_UPDATE()

  virtual void onTouchesBegan(const s_vec<c::Touch*>&, c::Event*);
  virtual void onTouchesMoved(const s_vec<c::Touch*>&, c::Event*);
  virtual void onTouchesEnded(const s_vec<c::Touch*>&, c::Event*);

  virtual void postReify();

  DECL_PTR(a::NodeList, shared)
};

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchesBegan(const s_vec<c::Touch*> &touches, c::Event*) {
  auto ps= CC_GNLF(Players,shared,"players");
  F__LOOP(it,touches) {
    auto t = *it;
    auto tap = t->getLocation();
    for (auto n=1; n < 3; ++n) {
      if (ps->parr[n]->getBoundingBox().containsPoint(tap)) {
        ps->parr[n].tap= touch;
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchesMoved(const s_vec<c::Touch*> &touches, c::Event* event) {
  auto ps= CC_GNLF(Players,shared,"players");
  auto wb= cx::visBox();
  F__LOOP(it,touches) {
    auto t = *it;
    auto tap = t->getLocation();
    for (auto node=mallets->head; node; node=node->next) {
      auto m= CC_GNF(Mallet,node,"mallet");
      auto &py= ps->parr[m->pnum];
      if (py->tap == t) {
        auto r= m->radius();
        auto cp= m->pos();
        auto npos= tap;
        //clamp
        if (npos.y < r) npos.y  = r;
        if (npos.x < r) npos.x = r;
        if (npos.x > wb.right - r) npos.x = wb.right - r;
        if (npos.y > wb.top - r) npos.y = wb.top - r;
        //keep player inside its court
        if (cp.y < wb.top* 0.5f) {
          if (npos.y > wb.top* 0.5f - r) {
            npos.y = wb.top* 0.5f - r;
          }
        } else {
          if (npos.y < wb.top* 0.5f + r) {
            npos.y = wb.top* 0.5f + r;
          }
        }

        m->nextPos= npos;
        m->vec= c::Vec2(tap.x - cp.x, tap.y - cp.y);
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchesEnded(const s_vec<c::Touch*> &touches, c::Event* ) {
  F__LOOP(it,touches) {
    auto t= *it;
    for (auto node=mallets->head;node;node=node->next) {
      auto m=CC_GNF(Mallet,node,"mallet");
      if (m->tap == t) {
        m->vec=c::Vec2(0,0);
        SNPTR(m->tap)
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::updateScore(int player, int score) {
  auto ball= CC_GNLF(Puck,pucks,"puck");
  auto bc= ball->circum();
  auto wb= cx::visBox();

  getHUD()->updateScore(player, score)
  ball->vec= CC_ZPT;

  if (player == 1) {
        //move ball to player 2 court
    ball->nextPos = c::Vec2(wb.cx, wb.cy + bc);
  } else {
      //move ball to player 1 court
    ball->nextPos = c::Vec2(wb.cx, wb.cy - bc);
  }

  for (auto node=mallets->head;node;node=node->next) {
    auto m= CC_GNF(Mallet,mode,"mallet");
    auto mc= m->circum();
    if (m->pnum == 1) {
      m->setPos(c::Vec2(wb.cx, mc));
    } else {
      m->setPos(c::Vec2(wb.cx, wb.top - mc));
    }
    m->tap=nullptr;
  }
}

//////////////////////////////////////////////////////////////////////////////
void GLayer::postReify() {
  mallets = engine->getNodeList(MalletNode().typeId());
  pucks = engine->getNodeList(PuckNode().typeId());
  shared = engine->getNodeList(SharedNode().typeId());

  auto mr=CC_GNLF(Mallet,mallets,"mallet")->radius();
  auto br=CC_GNLF(Puck,pucks,"puck")->radius();
  auto ss=CC_GNLF(GVars,shared,"slots");

  ss->sq_radii = pow(mr+br, 2);
  ss->goalWidth= 400;

}

//////////////////////////////////////////////////////////////////////////////
void GLayer::decorate() {
  engine = mc_new(GEngine);
}

END_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic &topic, void *m) {
  auto y= (GLayer*) getGLayer(2);

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




