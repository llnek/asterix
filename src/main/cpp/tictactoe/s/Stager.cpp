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
#include "Stager.h"
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
Stager::Stager(not_null<a::Engine*> e,
    not_null<c::Dictionary*> options)

  : f::BaseSystem(e,options) {

  this->inited=false;
}

//////////////////////////////////////////////////////////////////////////////
//
Stager::~Stager() {

}

//////////////////////////////////////////////////////////////////////////////
//
void Stager::RemoveFromEngine(not_null<a::Engine*> e) {
  SNPTR(board)
}

void Stager::AddToEngine(not_null<a::Engine*> e) {
  e->AddEntity(sh.factory.reifyBoard(sh.main,
                                       this.state));
  CCLOG("adding system: Stager");
  if (! inited) {
    OnceOnly(e);
    inited=true;
  }
  BoardNode n;
  board= e->GetNodeList(n.TypeId());
}

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
  auto cs= a::NodeFld<GridView>(node, "view");
  auto mgs = MapGridPos();
  auto pos=0;
  sp;

  R.forEach( mp => {
    sp= ccsx.createSprite('z.png');
    sp.setPosition(ccsx.vboxMID(mp));
    sh.main.addAtlasItem('game-pics',sp);
    cs[pos++]=[sp, sp.getPositionX(), sp.getPositionY(), csts.CV_Z];
  }, mgs);
}

//////////////////////////////////////////////////////////////////////////////
//
void Stager::OnceOnly(a::Node* node, float dt) {
  auto human = f::CstVal<c::Integer>("HUMAN")->geValue();
  auto bot= f::CstVal<c::Integer>("BOT")->geValue();
  auto ps = a::NodeFld<Players>(node, "players")->parr;
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
    if (parr[pnum]->category == human) {
      MGML()->SendMsg("/hud/timer/show");
    }
    else
    if (parr[pnum]->category == bot) {
    }
  }

  state->setObject(c::Integer::create(pnum), "actor");
}

//////////////////////////////////////////////////////////////////////////
//
Stager::DoIt(a::Node* node, float dt) {

  auto actor = CC_GDV(c::Integer,state,"actor");
  auto active = MGMS()->IsRunning();

  if (!active) {
    actor= CC_GDV(c::Integer,state,"lastWinner");
  }

  MGML()->SendMsg("/hud/update", {
    running: active,
    pnum: actor
  });
}

NS_END(tttoe)


