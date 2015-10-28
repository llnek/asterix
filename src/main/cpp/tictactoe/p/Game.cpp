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

#include "core/Odin.h"
#include "Game.h"

NS_ALIAS(ws, fusii::odin);
NS_ALIAS(cx, fusii::ccsx)
NS_ALIAS(cc, cocos2d)
NS_ALIAS(s, std)
NS_ALIAS(f, fusii)
NS_BEGIN(tttoe)


//////////////////////////////////////////////////////////////////////////
//
class CC_DLL BGLayer : f::XLayer {
public:

  virtual f::XLayer* Realize() {
    CenterImage("game.bg");
    return this;
  }

  NO__CPYASS(BGLayer)
  IMPL_CTOR(BGLayer)
};

//////////////////////////////////////////////////////////////////////////////
//
void GameLayer::Replay() {
  if (MGMS()->IsOnline()) {
    // request server to restart a new game
    ws::Send(MGMS()->WSOCK(), ws::Event(
      ws::MType::SESSION,
      ws::EType::REPLAY
    ));
  } else {
    InizGame();
    Reset(false);
    InitAsh();
    GetScene()->Resume();
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GameLayer::Play() {
  Reset(true);
  InitAsh();

  // sort out names of players
  let p1ids, p2ids;
  sjs.eachObj((v,k) => {
    if (v[0] === 1) {
      p1ids= [k, v[1] ];
    } else {
      p2ids= [k, v[1] ];
    }
  }, this.options.ppids);

  InitPlayers();

  GetHUD()->RegoPlayers(csts.P1_COLOR, p1ids,
                            csts.P2_COLOR, p2ids);

  f::EmptyQueue( MGMS()->MsgQueue() );
  GetScene()->Resume();
}

//////////////////////////////////////////////////////////////////////////////
//
void GameLayer::Reset(bool newFlag) {

  for (auto it= atlases.begin(); it != atlases.end(); ++it) {
    it->second->removeAllChildren();
  }

  if (atlases.empty()) {
    RegoAtlas("game-pics");
    RegoAtlas("lang-pics");
  }

  if (newFlag) {
    GetHUD()->ResetAsNew();
  } else {
    GetHUD()->Reset();
  }

  mc_del_ptr(factory)
  mc_del_ptr(engine)
}

//////////////////////////////////////////////////////////////////////////////
//
void GameLayer::UpdateHUD() {
  if (MGMS()->IsRunning()) {
    GetHUD()->DrawStatus(CC_GDV(c::Integer, options, "pnum"));
  } else {
    GetHUD()->DrawResult(CC_GDV(c::Integer, options, "lastWinner"));
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GameLayer::PlayTimeExpired() {
  MGMS()->MsgQueue().push("forfeit");
}

//////////////////////////////////////////////////////////////////////////////
//
void GameLayer::InitPlayers() {

  auto human = CC_CSV(c::Integer, "HUMAN");
  auto bot = CC_CSV(c::Integer, "BOT");
  auto netp = CC_CSV(c::Integer, "NETP");

  auto p1c= CC_CSV(c::Integer, "P1_COLOR");
  auto p2c= CC_CSV(c::Integer, "P2_COLOR");
  auto vx= CC_CSV(c::Integer, "CV_X");
  auto vo= CC_CSV(c::Integer, "CV_O");

  auto p1cat= human;
  auto p2cat= bot;

  if (mode == f::GMode::NET) {
    p2cat = netp;
    p1cat = netp;
  }
  else
  if (mode == f::GMode::ONE) {
  }
  else
  if (mode == f::GMode::TWO) {
    p2cat= human;
    //p1cat= human;
  }

  p1= new Player(p1cat, vx, 1, p1c);
  p2= new Player(p2cat, vo, 2, p2c);

}

//////////////////////////////////////////////////////////////////////////////
//
void GameLayer::OverAndDone(int winner) {
  GetHUD()->EndGame(winner);
}

//////////////////////////////////////////////////////////////////////////////
//
void GameLayer::SendMsg(const stdstr& topic, void* msg) {
  auto par = CC_PCAST(Game*);

  if ("/hud/showmenu" == topic) {
    ShowMenu();
  }

  if ("/hud/replay" == topic) {
    Replay();
  }

  if ("/hud/timer/show" == topic) {
    GetHUD()->ShowTimer();
  }

  if ("/net/restart" == topic) {
    GetHUD()->KillTimer();
    Play(false);
  }

  if ("/net/stop" == topic) {
    OverAndDone(msg.status);
  }

  if ("/hud/timer/hide" == topic) {
    GetHUD()->KillTimer();
  }

  if ("/hud/score/update" == topic) {
    GetHUD()->UpdateScore(msg.color, msg.score);
  }

  if ("/hud/end" == topic) {
    OverAndDone(msg.winner);
  }

  if ("/hud/update" == topic) {
    GetHUD()->Draw(msg.running, msg.pnum);
  }

  if ("/player/timer/expired" == topic) {
    PlayTimeExpired(msg);
  }

}

//////////////////////////////////////////////////////////////////////////////
//
void Game::Resume() {
  running= true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Game::Run() {
  running= true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Game::Pause() {
  running= false;
}

//////////////////////////////////////////////////////////////////////////////
//
bool Game::IsRunning() {
  return running;
}

//////////////////////////////////////////////////////////////////////////
//
Game::~Game() {
}

//////////////////////////////////////////////////////////////////////////
//
Game::Game() {
}

//////////////////////////////////////////////////////////////////////////
//
f::XGameLayer* Game::GetGLayer() {
  return (f::XGameLayer*) GetLayer(2);
}

//////////////////////////////////////////////////////////////////////////
//
f::XScene* Game::Realize() {

  auto g = (f::XLayer*) f::ReifyRefType<GameLayer>();
  auto b = (f::XLayer*) f::ReifyRefType<BGLayer>();
  auto h = (f::XLayer*) f::ReifyRefType<HUDLayer>();

  AddLayer(b, 1)->Realize();
  AddLayer(g, 2);
  AddLayer(h, 3)->Realize();

  // set this to be THE main game
  Bind(this);

  // realize game layer last
  g->Realize();

  return this;
}


NS_END(tttoe)

