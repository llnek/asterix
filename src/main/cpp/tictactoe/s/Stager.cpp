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

#include "core/CCSX.h"
#include "utils.h"
#include "Stager.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
Stager::Stager(not_null<EFactory*> f,
    not_null<c::Dictionary*> d)

  : f::BaseSystem(f,d) {

  this->inited=false;
}

//////////////////////////////////////////////////////////////////////////////
//
void Stager::AddToEngine(not_null<a::Engine*> e) {

  e->AddEntity(factory->ReifyBoard(MGML()));
  CCLOG("adding system: Stager");
  if (! inited) {
    OnceOnly(e);
    inited=true;
  }
  BoardNode n;
  board= e->GetNodeList(n.TypeId());
}

//////////////////////////////////////////////////////////////////////////////
//
bool Stager::OnUpdate(float dt) {
  if (cx::IsTransitioning()) { return false; }
  auto node= board->head;
  if (MGMS()->IsRunning() && NNP(node) {
    DoIt(node,dt);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Stager::ShowGrid(a::Node* node) {
  auto view= CC_GNF(GridView, node, "view");
  auto nil = CC_CSV(c::integer, "CV_Z");
  auto arr = MapGridPos();
  auto pos=0;

  for (auto it= arr.begin(); it != arr.end(); ++it) {
    auto sp= cx::ReifySprite("z.png");
    sp->setPosition(cx::VisBoxMID(*it));
    view->layer->AddAtlasItem("game-pics", sp);
    view->cells[pos++]=GridData(sp, sp.getPositionX(),
                                sp.getPositionY(),
                                nil);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Stager::OnceOnly(a::Node* node, float dt) {
  auto human = CC_CSV(c::Integer, "HUMAN");
  auto bot= CC_CSV(c::Integer,"BOT");
  auto ps = CC_GNF(Players, node, "players");
  auto pnum = 0;

  ShowGrid(node);

  if (MGMS()->IsOnline()) {
    CCLOG("reply to server: session started ok");
    MGMS()->NetSend({
      type: evts.MSG_SESSION,
      code: evts.STARTED
    });
  } else {
    pnum= CCRANDOM_0_1() > 0.5f ? 1 : 2; // randomly choose
    if (ps->parr[pnum]->category == human) {
      MGML()->SendMsg("/hud/timer/show");
    }
    else
    if (parr[pnum]->category == bot) {
      //noop
    }
  }

  state->setObject(c::Integer::create(pnum), "actor");
}

//////////////////////////////////////////////////////////////////////////
//
Stager::DoIt(a::Node* node, float dt) {

  auto actor = CC_GDV(c::Integer, state, "actor");
  auto active = MGMS()->IsRunning();
  HUDUpdate msg(active,actor);

  if (!active) {
    actor= CC_GDV(c::Integer, state, "lastWinner");
  }

  MGML()->SendMsg("/hud/update", &msg);
}




NS_END(tttoe)


