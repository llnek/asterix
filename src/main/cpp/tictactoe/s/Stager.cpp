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
#include "x2d/MainGame.h"
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
void Stager::AddToEngine(not_null<a::Engine*> e) {

  CCLOG("adding system: Stager");
  factory->ReifyArena( MGML());

  ArenaNode n;
  arena = e->GetNodeList(n.TypeId());

  if (! inited) {
    OnceOnly(arena->head);
    inited=true;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
bool Stager::OnUpdate(float dt) {
  if (cx::IsTransitioning()) { return false; }
  auto node= arena->head;
  if (MGMS()->IsLive() && NNP(node)) {
    DoIt(node,dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Stager::ShowGrid(a::Node* node) {
  auto view= CC_GNF(PlayView, node, "view");
  auto nil = CC_CSV(c::Integer, "CV_Z");
  auto arr = MapGridPos(1.0f);
  auto pos=0;

  F__LOOP(it, arr) {
    auto sp= cx::ReifySprite("z.png");
    sp->setPosition(cx::VBoxMID(*it));
    view->layer->AddAtlasItem("game-pics", sp);
    view->cells[pos++]=ViewData(sp, sp->getPositionX(),
                                sp->getPositionY(),
                                nil);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Stager::OnceOnly(a::Node* node) {
  auto ps = CC_GNF(Players, node, "players");
  auto human = CC_CSV(c::Integer, "HUMAN");
  auto bot= CC_CSV(c::Integer,"BOT");
  auto pnum = 0;

  ShowGrid(node);

  if (MGMS()->IsOnline()) {
    CCLOG("reply to server: session started ok");
    NetSend(ws::OdinEvent(
      ws::MType::SESSION,
      ws::EType::STARTED
    ));
  } else {
    pnum= CCRANDOM_0_1() > 0.5f ? 1 : 2; // randomly choose
    if (ps->parr[pnum].category == human) {
      MGMS()->SendMsg("/hud/timer/show");
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
void Stager::DoIt(a::Node* node, float dt) {

  auto pnum = CC_GDV(c::Integer, state, "pnum");
  auto active = MGMS()->IsLive();
  HUDUpdate msg(active, pnum);

  if (!active) {
    pnum= CC_GDV(c::Integer, state, "lastWinner");
  }

  MGMS()->SendMsg("/hud/update", &msg);
}




NS_END(tttoe)


