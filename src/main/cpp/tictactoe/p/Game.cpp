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

#include "s/EFactory.h"
#include "core/CCSX.h"
#include "core/Odin.h"
#include "Game.h"
NS_ALIAS(ws, fusii::odin)
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(tttoe)
BEGIN_NS_UNAMED()
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

//////////////////////////////////////////////////////////////////////////
//
class CC_DLL GameLayer : public f::XGameLayer {
private:

  NO__CPYASS(GameLayer)
  EFactory* factory;
  bool playable;

  HUDLayer* GetHUD();
  void MkAsh();

public:

  virtual void SendMsg(const stdstr& topic, void* msg);
  virtual void Reset(bool newFlag) ;
  virtual void Replay() ;
  virtual void Play();
  virtual void OnGameOver();

  virtual f::XLayer* Realize();

  void OnPlayerKilled();
  void OnEarnScore(int);
  void SpawnPlayer();

  virtual int GetIID() { return 2; }
  virtual void InizGame();

  DECL_CTOR(GameLayer)
};

//////////////////////////////////////////////////////////////////////////
//
GameLayer::~GameLayer() {

}

//////////////////////////////////////////////////////////////////////////
//
GameLayer::GameLayer() {
  playable=false;
}

//////////////////////////////////////////////////////////////////////////
//
HUDLayer* GameLayer::GetHUD() {
  return static_cast<HUDLayer*>( GetScene()->GetLayer(3));
}

//////////////////////////////////////////////////////////////////////////
//
f::XLayer* GameLayer::Realize() {
  EnableEventHandlers();
  cx::PauseAudio();
  playable=false;
  InizGame();
  scheduleUpdate();
  return this;
}

//////////////////////////////////////////////////////////////////////////
//
void GameLayer::InizGame() {

  CC_LOOP(it, atlases) { it->second->removeAllChildren(); }

  if (atlases.empty()) {
    RegoAtlas("game-pics");
    RegoAtlas("lang-pics");
  }

  f::EmptyQueue( MGMS()->MsgQueue() );
  MkAsh();

  auto seed = XCFG()->GetSeedData();
  auto p1c= CC_CSS("P1_COLOR");
  auto p2c= CC_CSS("P2_COLOR");
  auto ppids = seed["ppids"];
  stdstr p1k;
  stdstr p2k;
  stdstr p1n;
  stdstr p2n;

  CC_LOOP(it, ppids) {
    auto arr= it->second;
    if (arr[0].int_value() == 1) {
      p1k= it->first;
      p1n= arr[1].string_value();
    } else {
      p2k= it->first;
      p2n= arr[1].string_value();
    }
  }

  GetHUD()->RegoPlayers(p1c, p1k, p1n, p2c, p2k, p2n);
  GetHUD()->Reset();

  this->options->setObject(CC_INT(0), "lastWinner");
  this->playable=true;

  CCLOG("init-game - ok");
}

//////////////////////////////////////////////////////////////////////////
//
void GameLayer::MkAsh() {
  auto e = a::Engine::Reify();
  auto d = CC_DICT();
  auto f = new EFactory(e, d);

  CC_DROP(this->options)
  this->options= d;

  this->engine = e;
  this->factory=f;

  e->RegoSystem(new Resolve(f, d));
  e->RegoSystem(new Stager(f, d));
  e->RegoSystem(new Motion(f, d));
  e->RegoSystem(new Logic(f, d));

  CC_KEEP(this->options)
}

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
void GameLayer::UpdateHUD() {
  if (this->playable) {
    GetHUD()->DrawStatus(CC_GDV(c::Integer, this->options, "pnum"));
  } else {
    GetHUD()->DrawResult(CC_GDV(c::Integer, this->options, "lastWinner"));
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GameLayer::PlayTimeExpired() {
  MGMS()->MsgQueue().push("forfeit");
}

//////////////////////////////////////////////////////////////////////////////
//
void GameLayer::OverAndDone(int winner) {
  GetHUD()->EndGame(winner);
}

//////////////////////////////////////////////////////////////////////////
//
void GameLayer::update(float dt) {
  if (playable && NNP(engine)) {
    engine->Update(dt);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GameLayer::SendMsg(const stdstr& topic, void* msg) {
  auto par = CC_PCAST(Game*);

  if ("/hud/showmenu" == topic) {
    ShowMenu();
  }
  else
  if ("/hud/timer/show" == topic) {
    GetHUD()->ShowTimer();
  }
  else
  if ("/net/restart" == topic) {
    GetHUD()->KillTimer();
    Play(false);
  }
  else
  if ("/net/stop" == topic) {
    OverAndDone(msg.status);
  }
  else
  if ("/hud/timer/hide" == topic) {
    GetHUD()->KillTimer();
  }
  else
  if ("/hud/score/update" == topic) {
    auto p = (ScoreUpdate*) msg;
    GetHUD()->UpdateScore(p->color, p->score);
  }
  else
  if ("/hud/end" == topic) {
    auto p= (int*) msg;
    OverAndDone(*p);
  }
  else
  if ("/hud/update" == topic) {
    auto p= (HUDUpdate*) msg;
    GetHUD()->Draw(p->running, p->pnum);
  }
  else
  if ("/player/timer/expired" == topic) {
    PlayTimeExpired();
  }
  else
  if ("/game/stop" == topic) {
    playable=false;
  }
  else
  if ("/game/play" == topic) {
    playable=true;
  }
}

END_NS_UNAMED()

//////////////////////////////////////////////////////////////////////////
//
void Game::Stop() {
  GetGLayer()->SendMsg("/game/stop");
}

//////////////////////////////////////////////////////////////////////////
//
void Game::Play() {
  GetGLayer()->SendMsg("/game/play");
}

//////////////////////////////////////////////////////////////////////////
//
f::XGameLayer* Game::GetGLayer() {
  return static_cast<f::XGameLayer*>( GetLayer(2));
}

//////////////////////////////////////////////////////////////////////////
//
f::XScene* Game::Realize() {

  auto g = f::ReifyRefType<GameLayer>();
  auto b = f::ReifyRefType<BGLayer>();
  auto h = f::ReifyRefType<HUDLayer>();

  AddLayer( static_cast<f::XLayer*>(b), 1)->Realize();
  AddLayer( static_cast<f::XLayer*>(g), 2);
  AddLayer( static_cast<f::XLayer*>(h), 3)->Realize();

  // set this to be THE main game
  Bind(this);

  // realize game layer last
  g->Realize();

  return this;
}


NS_END(tttoe)

