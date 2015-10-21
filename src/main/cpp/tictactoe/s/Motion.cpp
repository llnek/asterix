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
    ws->ListenAll( [=](const ws::Event& evt) {
      this->evQ.push_back(evt);
    });
  }

  /*
  t=Rx.Observable.create( obj => {
    sh.main.signal('/touch/one/end',
                   msg => obj.onNext(msg));
  });
  m=Rx.Observable.create( obj => {
    sh.main.signal('/mouse/up',
                   msg => obj.onNext(msg));
  });
  */
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
void Motions::OnSocket(a::Node* node, const ws::Event& evt) {
  switch (evt.type) {
    case ws::MType::NETWORK:
      OnNet(node, evt);
    break;
    case ws::MType::SESSION:
      OnSess(node, evt);
    break;
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Motions::OnNet(a::Node* node, const ws::Event& evt) {
  switch (evt.code) {
    case ws::EType::RESTART:
      CCLOG("restarting a new game...");
      MGML()->SendMsg("/net/restart");
    break;
    case ws::EType::STOP:
      if (MGMS()->IsRunning()) {
        CCLOG("game will stop");
        MGML()->SendMsg("/hud/timer/hide");
        OnSess(node, evt);
        MGML()->SendMsg("/net/stop", &evt);
      }
    break;
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Motions::OnSess(a::Node* node, const ws::Event& evt) {
  auto ps= CC_GNF(Players, node, "players");
  auto grid= CC_GNF(Grid, node, "grid");
  auto snd="";
  auto pnum= evt.doco["pnum"].GetInt();
  auto p2=pnum;

  if (evt.doco.HasMember("cmd")) {
    auto cmd= evt.doco["cmd"];
    auto cell= cmd["cell"].GetInt();
    auto cv= cmd["value"].GetInt();
    if (cell >= 0 &&
        cell < grid->values.Size()) {
      if (ps->parr[1]->value == cv) {
        snd= "x_pick";
      } else {
        snd= "o_pick";
      }
      grid->values[cell] = cv;
      cx::SfxPlay(snd);
    }
  }

  if (pnum == 1 || pnum == 2) {} else { return; }

  switch (evt.code) {
    case ws::EType::POKE_MOVE:
      CCLOG("player %d: my turn to move", pnum);
      MGML()->SendMsg("/hud/timer/show");
      state->setObject(CC_INT(pnum), "actor");
    break;
    case ws::EType::POKE_WAIT:
      CCLOG("player %d: my turn to wait", pnum);
      MGML()->SendMsg("/hud/timer/hide");
      // toggle color
      p2= pnum == 1 ? 2 : 1;
      state->setObject(CC_INT(p2), "actor");
    break;
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Motions::OnGUI(a::Node* node, float x, float y) {
  auto sel = CC_GNF(UISelection, node, "selection");
  auto map = CC_GNF(GridView, node, "view");//->gridMap;
  auto cur = CC_GDV(c::Integer, state, "actor");
  auto n=0;

  //set the mouse/touch position
  sel->cell= -1;
  sel->px = x;
  sel->py = y;

  if (cur <= 0) { return; }

  //which cell did he click on?
  for (auto it= view->mpos.begin(); it != view->mpos.end(); ++it) {
    auto bx = *it;
    if (sel->px >= bx.left && sel->px <= bx.right &&
        sel->py >= bx.bottom && sel->py <= bx.top) {
      sel->cell= n;
      break;
    }
    ++n;
  }
}




NS_END(tttoe)

