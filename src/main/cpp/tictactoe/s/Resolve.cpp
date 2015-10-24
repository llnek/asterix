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

#include "x2d/MainGame.h"
#include "Resolve.h"
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
//
Resolve::Resolve(not_null<EFactory*> f,not_null<c::Dictionary*> d)
  : f::BaseSystem(f, d) {
  SNPTR(board)
}

//////////////////////////////////////////////////////////////////////////
//
void Resolve::AddToEngine(not_null<a::Engine*> e) {
  BoardNode n;
  board = e->GetNodeList(n.TypeId());
}

//////////////////////////////////////////////////////////////////////////
//
bool Resolve::OnUpdate(float dt) {
  auto node= board->head;
  if (MGMS()->IsRunning() &&
      NNP(node)) {
    SyncUp(node);
    DoIt(node, dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////
//
void Resolve::SyncUp(a::Node* node) {
  auto view= CC_GNF(PlayView, node, "view");
  auto grid= CC_GNF(Grid, node, "grid");
  auto nil= CC_CSV(c::Integer, "CV_Z");

  for (int i=0; i < grid->values.size(); ++i) {
    auto v= grid->values[i];
    if (v != nil) {
      auto c= XrefCell(i, view);
      if (c.x > 0.0f && c.y > 0.0f ) {
        auto& z= view->cells[i];
        if (NNP(z.sprite)) {
          z.sprite->removeFromParent();
        }
        z.sprite = DrawSymbol(view, c.x, c.y, v);
        z.x= c.x;
        z.y= c.y;
        z.value=v;
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
const c::Vec2 Resolve::XrefCell(int pos, PlayView* view) {
  auto cells = view->boxes.size();
  auto delta=0;

  if (pos >= 0 && pos < cells) {
    auto& gg = view->boxes[pos];
    auto x = gg.left + (gg.right - gg.left  - delta) * 0.5;
    auto y = gg.top - (gg.top - gg.bottom - delta ) * 0.5;
    // the cell's center
    return c::Vec2(x,y);
  } else {
    return c::Vec2(-1.0f, -1.0f);
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Resolve::DoIt(a::Node* node, float dt) {

  auto ps = CC_GNF(Players, node, "players");
  auto gd = CC_GNF(Grid, node, "grid");
  auto winner= -1;
  s::array<int, BD_SZ> combo;

  for (int i=0; i < ps->parr.size(); ++i) {
    if (i > 0 &&
        CheckWin(node, ps->parr[i], gd, combo)) {
      winner=i;
      break;
    }
  }

  if (winner > 0) {
    DoWin(node, ps->parr[winner], combo);
  }
  else
  if (CheckDraw(node, gd)) {
    DoDraw(node);
  }
  else {
    auto& q= MGMS()->MsgQueue();
    if (q.size() > 0 &&
        "forfeit" == q.pop()) {
      DoForfeit(node);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::DoWin(a::Node* node,
    Player& winner,
    const s::array<int, BD_SZ>& combo) {

  ScoreUpdate msg(winner.color, 1);
  MGML()->SendMsg("/hud/score/update", &msg);

  ShowWinningIcons(node, combo);

  DoDone(node, winner);
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::DoDraw(a::Node* node) {
  auto ps= CC_GNF(Players, node, "players");
  DoDone(node, ps->parr[0]);
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::DoForfeit(a::Node* node) {
  auto cur= CC_GDV(c::Integer, state, "pnum");
  auto other= cur == 1 ? 2 : cur == 2 ? 1 : 0;
  auto view = CC_GNF(PlayView, node, "view");
  auto ps= CC_GNF(Players, node, "players");

  auto& loser = ps->parr[cur];
  auto& win= ps->parr[other];

  ScoreUpdate msg(win.color, 1);
  MGML()->SendMsg("/hud/score/update", &msg);

  //gray out the losing icons
  for (auto it= view->cells.begin(); it != view->cells.end(); ++it) {
    auto& z = *it;
    if (z.value == loser.value) {
      if (NNP(z.sprite)) {
        z.sprite->removeFromParent();
      }
      //TODO: why + 2????
      z.sprite = DrawSymbol(view, z.x, z.y, z.value+2);
    }
  }

  DoDone(node, win);
}


//////////////////////////////////////////////////////////////////////////////
//
void Resolve::ShowWinningIcons(a::Node* node,
    const s::array<int,BD_SZ>& combo) {

  auto view = CC_GNF(PlayView, node, "view");
  auto nil = CC_CSV(c::Integer, "CV_Z");

  for (int i=0; i < view->cells.size(); ++i) {
    if (! (s::find(s::begin(combo), s::end(combo), i)
          != combo.end())) {
      //flip the other cells to gray
      auto& z= view->cells[i];
      if (NNP(z.sprite) && z.value != nil) {
        z.sprite->removeFromParent();
        z.sprite= DrawSymbol(view, z.x, z.y, z.value, true);
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::DoDone(a::Node* node, Player& pobj) {

  int pnum = pobj.pnum > 0 ? pobj.pnum : 0;

  MGML()->SendMsg("/hud/timer/hide");
  cx::SfxPlay("game_end");
  MGML()->SendMsg("/hud/end", &pnum);

  state->setObject(CC_INT(pnum), "lastWinner");
  MGMS()->Pause();
}

//////////////////////////////////////////////////////////////////////////////
//
bool Resolve::CheckDraw(a::Node* node, Grid* gd) {

  auto nil= CC_CSV(c::Integer, "CV_Z");

  return ! (s::find(s::begin(gd->values), s::end(gd->values), nil)
    != gd->values.end());
}


//////////////////////////////////////////////////////////////////////////////
//
bool Resolve::CheckWin(a::Node* node,
    Player& p, Grid* game,
    s::array<int, BD_SZ>& combo) {

  auto bd= CC_GNF(Board, node, "board");

  CCLOG("checking win for %s", p.color.c_str());

  for (auto it= bd->GOALS.begin(); it != bd->GOALS.end(); ++it) {
    auto& g = *it;
    int cnt=0;
    for (int i=0; i < g.size(); ++i) {
      auto pos = g[i];
      if (game->values[pos] == p.value) {
        ++cnt;
      }
    }
    if (cnt == g.size()) {
       // found a winning combo, this guy wins!
      s::copy(s::begin(g), s::end(g), s::begin(combo));
      return true;
    }
  }
  return false;
}





NS_END(tttoe)

