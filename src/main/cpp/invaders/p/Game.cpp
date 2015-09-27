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

#include "x2d/XGameLayer.h"
#include "x2d/MainGame.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "s/Stager.h"
#include "s/Motions.h"
#include "s/Move.h"
#include "s/Collide.h"
#include "s/Resolve.h"
#include "s/Aliens.h"
#include "Game.h"
#include "HUD.h"

NS_ALIAS(cx, fusii::ccsx)
NS_ALIAS(cc, cocos2d)
NS_ALIAS(s, std)
NS_ALIAS(f, fusii)
NS_BEGIN(invaders)


//////////////////////////////////////////////////////////////////////////
//
class CC_DLL BGLayer : public f::XLayer {
private:
  NO__CPYASS(BGLayer)
public:
  virtual f::XLayer* Realize() {
    CenterImage(XCFGS()->GetImage("game.bg"));
    return this;
  }
  virtual int GetIID() { return 1; }
  virtual ~BGLayer() {}
  BGLayer() {}
  CREATE_FUNC(BGLayer)
};

//////////////////////////////////////////////////////////////////////////////
//
GameLayer::~GameLayer() {
  delete factory;
}

//////////////////////////////////////////////////////////////////////////////
//
GameLayer::GameLayer() {
  SNPTR(factory)
}

//////////////////////////////////////////////////////////////////////////
//
f::XLayer* GameLayer::Realize() {
  //cx::OnKeyPolls(this->keyboard);
  return this;
}

//////////////////////////////////////////////////////////////////////////
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
}

//////////////////////////////////////////////////////////////////////////
//
void GameLayer::Replay() {
  Play(false);
}

//////////////////////////////////////////////////////////////////////////
//
void GameLayer::Play(bool newFlag) {
  InitAsh();
  Reset(newFlag);
}

//////////////////////////////////////////////////////////////////////////////
//
void GameLayer::InitAsh() {
  auto options = c::Dictionary::create();

  factory = new Factory(engine, options);
  engine = a::Engine::Create();

  engine->RegoSystem(new Stager(factory, options));
  engine->RegoSystem(new Motions(factory, options));
  engine->RegoSystem(new Move(factory, options));
  engine->RegoSystem(new Aliens(factory, options));
  engine->RegoSystem(new Collide(factory, options));
  engine->RegoSystem(new Resolve(factory, options));
}

//////////////////////////////////////////////////////////////////////////
//
void GameLayer::SpawnPlayer() {
  factory->BornShip();
}

//////////////////////////////////////////////////////////////////////////
//
void GameLayer::OnPlayerKilled() {
  cx::SfxPlay("xxx-explode");
  if (GetHUD()->ReduceLives(1)) {
    OnDone();
  } else {
    SpawnPlayer();
  }
}

//////////////////////////////////////////////////////////////////////////
//
void GameLayer::OnNewGame(const f::GMode mode) {
  //sh.sfxPlay('start_game');
  SetMode(mode);
  Play(true);
}

//////////////////////////////////////////////////////////////////////////
//
void GameLayer::OnEarnScore(int score) {
  GetHUD()->UpdateScore(score);
}

//////////////////////////////////////////////////////////////////////////
//
void GameLayer::OnDone() {
  auto g= (Game*) getParent();
  g->Pause();
  Reset(false);
  GetHUD()->EnableReplay();
}

//////////////////////////////////////////////////////////////////////////
//
void GameLayer::SendMsg(const stdstr& topic, void* msg) {

  if (topic == "/game/players/earnscore") {
    c::Integer* i = (c::Integer*) msg;
    OnEarnScore(i->getValue());
    i->release();
  }

  if (topic == "/hud/showmenu") {

    auto f= []() { DIRTOR()->popScene(); }
    auto a= c::CallFunc::create(f);
    MainMenu* m = MainMenu::CreateWithBackAction(a);

    DIRTOR()->pushScene(MainMenu::CreateWithBackAction(a));
  }

  if (topic == "/hud/replay") {
    Play(false);
  }

  if (topic == "/game/players/killed") {
    OnPlayerKilled(msg);
  }

}

//////////////////////////////////////////////////////////////////////////////
//
void Game::Resume() {
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
  running = false;
}

//////////////////////////////////////////////////////////////////////////
//
f::XScene* Game::Realize() {

  auto g = (f::XLayer*) GameLayer::create();
  auto b = (f::XLayer*) BGLayer::create();
  auto h = (f::XLayer*) HUDLayer::create();

  AddLayer(b, 1)->Realize();
  AddLayer(g, 2);
  AddLayer(h, 3)->Realize();

  // realize game layer last
  g->Realize();

  f::MainGame::Set(this);
  return this;
}





NS_END(invaders)

