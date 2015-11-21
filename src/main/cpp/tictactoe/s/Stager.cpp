// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2015, Ken Leung. All rights reserved.

#include "core/Primitives.h"
#include "x2d/GameScene.h"
#include "core/CCSX.h"
#include "core/Odin.h"
#include "ash/Node.h"
#include "n/cobjs.h"
#include "utils.h"
#include "Stager.h"

NS_ALIAS(ws, fusii::odin)
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(tttoe)


//////////////////////////////////////////////////////////////////////////////
Stager::Stager(not_null<EFactory*> f,
    not_null<c::Dictionary*> d)

  : f::BaseSystem(f, d) {

  this->inited=false;
  SNPTR(board)
}

//////////////////////////////////////////////////////////////////////////////
//
void Stager::addToEngine(not_null<a::Engine*> e) {

  CCLOG("adding system: Stager");
  factory->reifyBoard( MGML());

  BoardNode n;
  board = e->getNodeList(n.typeId());

  if (! inited) {
    onceOnly(board->head);
    inited=true;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
bool Stager::onUpdate(float dt) {
  if (cx::isTransitioning()) { return false; }
  auto node= board->head;
  if (MGMS()->isLive() && NNP(node)) {
    doIt(node,dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Stager::showGrid(a::Node* node) {
  auto view= CC_GNF(PlayView, node, "view");
  auto nil = CC_CSV(c::Integer, "CV_Z");
  auto arr = mapGridPos(1.0f);
  auto pos=0;

  F__LOOP(it, arr) {
    auto sp= cx::reifySprite("z.png");
    sp->setPosition(cx::vboxMID(*it));
    sp->setUserObject(CC_INT(nil));
    view->layer->addAtlasItem("game-pics", sp);
    view->cells[pos++]=sp;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Stager::onceOnly(a::Node* node) {
  auto ps = CC_GNF(Players, node, "players");
  auto human = CC_CSV(c::Integer, "HUMAN");
  auto bot= CC_CSV(c::Integer,"BOT");
  auto pnum = 0;

  showGrid(node);

  if (MGMS()->isOnline()) {
    CCLOG("reply to server: session started ok");
    ws::netSend(nullptr, new ws::OdinEvent(
      ws::MType::SESSION,
      ws::EType::STARTED
    ));
  } else {
    pnum= CCRANDOM_0_1() > 0.5f ? 1 : 2; // randomly choose
    if (ps->parr[pnum].category == human) {
      MGMS()->sendMsg("/hud/timer/show");
    }
    else
    if (ps->parr[pnum].category == bot) {
      //noop
    }
  }

  state->setObject(CC_INT(pnum), "pnum");
}

//////////////////////////////////////////////////////////////////////////
//
void Stager::doIt(a::Node* node, float dt) {

  auto pnum = CC_GDV(c::Integer, state, "pnum");
  auto active = MGMS()->isLive();

  if (!active) {
    pnum= CC_GDV(c::Integer, state, "lastWinner");
  }

  j::Json msg = j::Json::object {
    { "running", active },
    { "pnum", pnum }
  };

  MGMS()->sendMsg("/hud/update", &msg);
}




NS_END(tttoe)


