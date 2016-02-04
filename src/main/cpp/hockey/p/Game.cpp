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

  virtual void onTouchStart(ComObj*, const s_vec<c::Touch*>& );
  virtual void onTouchMotion(ComObj*, const s_vec<c::Touch*>& );
  virtual void onTouchEnd(ComObj*, const s_vec<c::Touch*>& );

  virtual void postReify();

  DECL_PTR(a::NodeList, mallets)
  DECL_PTR(a::NodeList, pucks)
  DECL_PTR(a::NodeList, shared)
};

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onTouchStart(ComObj *cobj, const s_vec<c::Touch*> &touches) {
  auto m = (Mallet*) cobj;
  F__LOOP(it,touches) {
    auto t = *it;
    auto tap = t->getLocation();
    if (m->bbox().containsPoint(tap)) {
      m->tap= t;
    }
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchMotion(ComObj *cobj, const s_vec<c::Touch*> &touches) {
  auto wb= cx::visBox();
  auto m= (Mallet*)cobj;
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

      m->nextPos= npos;
      m->vec= c::Vec2(tap.x - cp.x, tap.y - cp.y);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchEnd(ComObj *cobj, const s_vec<c::Touch*> &touches) {
  auto m = (Mallet*) cobj;
  F__LOOP(it,touches) {
    auto t= *it;
    if (m->tap == t) {
      m->vec= CC_ZPT;
      m->tap= CC_NIL;
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
      m->setPos(wb.cx, mc);
    } else {
      m->setPos(wb.cx, wb.top - mc);
    }
    m->tap=CC_NIL;
  }

  SCAST(GEngine*,engine)->readyPoint(mallets,pucks->head);
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




