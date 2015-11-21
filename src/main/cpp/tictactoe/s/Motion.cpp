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

#include "x2d/GameScene.h"
#include "Motion.h"
NS_ALIAS(ws, fusii::odin)
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
//
Motions::Motions(not_null<EFactory*> f, not_null<c::Dictionary*> d)

  : f::BaseSystem(f, d) {

  SNPTR(netplay)
  SNPTR(gui)
  inited=false;
}

//////////////////////////////////////////////////////////////////////////
//
Motions::~Motions() {
  flushQ();
}

//////////////////////////////////////////////////////////////////////////
//
void Motions::flushQ() {
  f::EmptyQueue<c::Event*>(evQ);
}

//////////////////////////////////////////////////////////////////////////
//
void Motions::addToEngine(not_null<a::Engine*> e) {

  NetPlayNode n;
  netplay = e->getNodeList(n.typeId());

  GUINode g;
  gui = e->getNodeList(g.typeId());

  flushQ();
}

//////////////////////////////////////////////////////////////////////////
//
void Motions::onceOnly() {
  auto ws= MGMS()->wsock();
  if (NNP(ws)) {
    ws->listen( [=](ws::OdinEvent* evt) {
      this->evQ.push(evt);
    });
  }
}

//////////////////////////////////////////////////////////////////////////
//
bool Motions::onUpdate(float dt) {
  auto n= netplay->head;
  auto g= gui->head;

  if (!inited) {
    onceOnly();
    inited=true;
  }

  if (evQ.size() > 0) {
    auto evt= evQ.front();
    evQ.pop();
    if (evt->getType() == c::Event::Type::MOUSE) {
      if (NNP(g)) { onGUI(g, (c::EventMouse*) evt); }
    }
    else
    if (evt->getType() == c::Event::Type::TOUCH) {
      if (NNP(g)) { onGUI(g, (c::EventTouch*) evt); }
    }
    else
    if (evt->getType() == c::Event::Type::CUSTOM) {
      if (NNP(n)) { onSocket(n, (ws::OdinEvent*)evt); }
    }

    evt->release();
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////
//
void Motions::onSocket(a::Node* node, ws::OdinEvent* evt) {
  switch (evt->type) {
    case ws::MType::NETWORK:
      onNet(node, evt);
    break;
    case ws::MType::SESSION:
      onSess(node, evt);
    break;
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Motions::onNet(a::Node* node, const ws::OdinEvent* evt) {

  switch (evt->code) {
    case ws::EType::RESTART:
      CCLOG("restarting a new game...");
      MGMS()->sendMsg("/net/restart");
    break;
    case ws::EType::STOP:
      if (MGMS()->isLive()) {
        CCLOG("game will stop");
        MGMS()->sendMsg("/hud/timer/hide");
        onSess(node, evt);
        MGMS()->sendMsg("/net/stop", evt);
      }
    break;
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Motions::onSess(a::Node* node, const ws::OdinEvent* evt) {
  auto ps= CC_GNF(Players, node, "players");
  auto grid= CC_GNF(Grid, node, "grid");
  auto snd="";
  auto source = evt->doco["source"];
  auto cmd= source["cmd"];
  auto pnum= source["pnum"].int_value();

  if (cmd.is_object()) {
    auto cell= cmd["cell"].int_value();
    auto cv= cmd["value"].int_value();
    if (cell >= 0 &&
        cell < grid->values.size()) {
      if (ps->parr[1].value == cv) {
        snd= "x_pick";
      } else {
        snd= "o_pick";
      }
      grid->values[cell] = cv;
      cx::sfxPlay(snd);
    }
  }

  if (pnum == 1 || pnum == 2) {} else { return; }

  switch (evt->code) {
    case ws::EType::POKE_MOVE:
      CCLOG("player %d: my turn to move", pnum);
      MGMS()->sendMsg("/hud/timer/show");
      state->setObject(CC_INT(pnum), "pnum");
    break;
    case ws::EType::POKE_WAIT:
      CCLOG("player %d: my turn to wait", pnum);
      MGMS()->sendMsg("/hud/timer/hide");
      // toggle color
      auto p2= pnum == 1 ? 2 : 1;
      state->setObject(CC_INT(p2), "pnum");
    break;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Motions::onGUI(a::Node* node, c::EventMouse* evt) {
  onGUIXXX(node, evt->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
void Motions::onGUI(a::Node* node, c::EventTouch* evt) {
  //OnGUIXXX(node, evt->getLocation());
}

//////////////////////////////////////////////////////////////////////////
//
void Motions::onGUIXXX(a::Node* node, const c::Vec2& pos) {

  auto sel = CC_GNF(UISelection, node, "selection");
  auto view = CC_GNF(PlayView, node, "view");
  auto cur = CC_GDV(c::Integer, state, "pnum");
  auto n=0;

  //set the mouse/touch position
  sel->cell= -1;
  sel->px = pos.x;
  sel->py = pos.y;

  if (cur >  0) {} else { return; }

  //which cell did he click on?
  F__LOOP(it, view->boxes) {
    auto& bx = *it;
    if (sel->px >= bx.left && sel->px <= bx.right &&
        sel->py >= bx.bottom && sel->py <= bx.top) {
      sel->cell= n;
      break;
    }
    ++n;
  }
}




NS_END(tttoe)

