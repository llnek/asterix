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

#include "Motions.h"
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
//
Motions::Motions(not_null<EFactory*> f, not_null<c::Dictionary*> d)

  : f::BaseSystem(f, d) {

  inited=false;
}

//////////////////////////////////////////////////////////////////////////
//
Motions::~Motions() {
  evQ.clear();
}

//////////////////////////////////////////////////////////////////////////
//
void Motions::AddToEngine(not_null<a::Engine*> e) {

  NetPlayNode n;
  netplay = e->GetNodeList(n.TypeId());

  GUINode g;
  gui = e->GetNodeList(g.TypeId());

  evQ.clear();
}

//////////////////////////////////////////////////////////////////////////
//
void Motions::OnceOnly() {
  let ws= this.state.wsock,
  t, m, s;
  if (MGMS()->IsOnline()) {
    ws.cancelAll();
    s=Rx.Observable.create( obj => {
      ws.listenAll( msg => {
        obj.onNext({group:'net',
                    event: msg});
      });
    });
  } else {
    s= Rx.Observable.never();
  }
  t=Rx.Observable.create( obj => {
    sh.main.signal('/touch/one/end',
                   msg => obj.onNext(msg));
  });
  m=Rx.Observable.create( obj => {
    sh.main.signal('/mouse/up',
                   msg => obj.onNext(msg));
  });
  this.stream= Rx.Observable.merge(m,t,s);
  this.stream.subscribe( msg => {
    if (!!this.evQ) {
      this.evQ.push(msg);
    }
  });
}

//////////////////////////////////////////////////////////////////////////
//
bool Motions::OnUpdate(float dt) {
  auto evt= evQ.empty() ? nullptr : evQ.pop();
  auto n= netplay->head;
  auto g= gui->head;

  if (!inited) {
    OnceOnly();
    inited=true;
  }
  else if (NNP(evt)) {
    if (evt->group == "net") {
      if (NNP(n)) { OnNet(n, evt->event); }
    } else {
      if (NNP(g)) { OnGUI(g,evt); }
    }
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////
//
void Motions::OnSocket(a::Node* node, evt) {
  switch (evt.type) {
    case evts.MSG_NETWORK:
      OnNet(node, evt);
    break;
    case evts.MSG_SESSION:
      OnSess(node, evt);
    break;
  }
}

void Motions::OnNet(a::Node* node, evt) {
  switch (evt.code) {
    case evts.RESTART:
      CCLOG("restarting a new game...");
      MGML()->SendMsg("/net/restart");
    break;
    case evts.STOP:
      if (MGMS()->IsRunning()) {
        CCLOG("game will stop");
        MGML()->SendMsg("/hud/timer/hide");
        OnSess(node,evt);
        MGML()->SendMsg("/net/stop", evt);
      }
    break;
  }
}

void Motions::OnSess(a::Node* node, evt) {
  auto grid= CC_GNF(GridView, node, "grid");
  auto ps= CC_GNF(Players, node, "players");
  auto cmd= evt.source.cmd;
  auto snd="";
  auto vs = grid->values;

  if (NNP(cmd) &&
      cmd.cell >= 0 &&
      cmd.cell < vs.Size()) {

    if (ps->parr[1]->value == cmd->value) {
      snd= "x_pick";
    } else {
      snd= "o_pick";
    }
    vs[cmd->cell] = cmd->value;
    cx::SfxPlay(snd);
  }

  auto pnum= evt.source.pnum > 0 ? evt.source.pnum : -1;
  if (pnum == 1 || pnum == 2) {} else { return; }

  switch (evt.code) {
    case evts.POKE_MOVE:
      CCLOG("player %d: my turn to move", pnum);
      MGML()->SendMsg("/hud/timer/show");
      state->setObject(c::Integer::create(pnum), "actor");
    break;
    case evts.POKE_WAIT:
      CCLOG("player %d: my turn to wait", pnum);
      MGML()->SendMsg("/hud/timer/hide");
      // toggle color
      state->setObject(c::Integer::create( pnum==1 ? 2 : 1), "actor");
    break;
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Motions::OnGUI(a::Node* node, evt) {
  if (!MGMS()->IsRunning()) {return;}
  auto sel = CC_GNF(UISelection, node, "selection");
  auto map = CC_GNF(GridView, node, "view")->gridMap;
  auto cur = CC_GDV(c::Integer, state, "actor");
  auto sz= map.Size();

  //set the mouse/touch position
  sel->px = evt.loc.x;
  sel->py = evt.loc.y;
  sel->cell= -1;

  if (cur <= 0) { return; }

  //which cell did he click on?
  for (int n=0; n < sz; ++n) {
    auto rect = map[n];
    if (sel->px >= rect.left && sel->px <= rect.right &&
        sel->py >= rect.bottom && sel->py <= rect.top) {
      sel->cell= n;
      break;
    }
  }
}




NS_END(tttoe)

