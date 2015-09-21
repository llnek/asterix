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

#include "x2d/XLayer.h"
#include "Game.h"
NS_ALIAS(cx, fusilli::ccsx)
NS_ALIAS(cc, cocos2d)
NS_ALIAS(s, std)
NS_ALIAS(f, fusilli)
NS_BEGIN(invaders)


GameLayer::~GameLayer() {

}

GameLayer::GameLayer()
  : factory(nullptr) {

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
bool GameLayer::Replay() {
  Play(false);
}

//////////////////////////////////////////////////////////////////////////
//
void GameLayer::Play(bool newFlag) {
  InitEngine();
  InitAsh();
  Reset(newFlag);
}

//////////////////////////////////////////////////////////////////////////////
//
void GameLayer::InitAsh() {
  options = cc::Dictionary::create();
  factory = new Factory(engine, d);
  engine->RegoSystem(new Stager(factory, options));
  engine->RegoSystem(new Motion(factory, options));
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
  SetGameMode(mode);
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
  SCAST(Game*,getParent())->Pause();
  Reset();
  GetHUD()->EnableReplay();
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
bool Game::IsOperational() {
  return running;
}

//////////////////////////////////////////////////////////////////////////
//
Game::~Game() {

}

//////////////////////////////////////////////////////////////////////////
//
Game::Game()
  : running(false) {

}

//////////////////////////////////////////////////////////////////////////
//
f::XScene* Game::Realize() {
  auto g = GameLayer::create();
  auto b = BGLayer::create();
  auto h = HUDLayer::create();

  AddLayer(b, 1)->Realize();
  AddLayer(g, 2);
  AddLayer(h)->Realize();

  // realize game layer last
  g->Realize();

  f::MainGame::Set(this);
  return this;
}





NS_END(invaders)

