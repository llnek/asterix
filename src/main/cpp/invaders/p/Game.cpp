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

#include "Game.h"
NS_USING(cocos2d)
NS_USING(std)
NS_USING(fusilli)
NS_BEGIN(invaders)


//////////////////////////////////////////////////////////////////////////
//
void GameLayer::PKInput() {
  CCSX::OnKeyPolls(this->keyboard);
}

//////////////////////////////////////////////////////////////////////////
//
void GameLayer::Reset(bool newFlag) {
  if (atlases.empty()) {
    RegoAtlas("game-pics");
    RegoAtlas("lang-pics");
  }
  else
  for (auto it= atlases.begin(); it != atlases.end(); ++it) {
    it->second->removeAllChildren();
  }

  if (newFlag) {
    GetHUD()->ResetAsNew();
  } else {
    GetHUD()->Reset();
  }
}

//////////////////////////////////////////////////////////////////////////
//
bool GameLayer::IsOperational() {
  return true;
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
  Reset(newFlag);
}

//////////////////////////////////////////////////////////////////////////
//
void GameLayer::SpawnPlayer() {
  sh.factory.bornShip();
}

//////////////////////////////////////////////////////////////////////////
//
void GameLayer::OnPlayerKilled() {
  CCSX::SfxPlay("xxx-explode");
  if (GetHUD()->ReduceLives(1)) {
    OnDone();
  } else {
    SpawnPlayer();
  }
}

//////////////////////////////////////////////////////////////////////////
//
void GameLayer::OnNewGame(const GameMode mode) {
  //sh.sfxPlay('start_game');
  SetGameMode(mode);
  Play(true);
}

//////////////////////////////////////////////////////////////////////////
//
void GameLayer::OnEarnScore(float score) {
  GetHUD()->UpdateScore(score);
}

//////////////////////////////////////////////////////////////////////////
//
void GameLayer::OnDone() {
  running=false;
  Reset();
  GetHUD()->EnableReplay();
}

//////////////////////////////////////////////////////////////////////////
//
MainGame::~MainGame() {

}

//////////////////////////////////////////////////////////////////////////
//
MainGame::MainGame() {
  running = false;
}

//////////////////////////////////////////////////////////////////////////
//
XScene* MainGame::Realize() {
  auto y = BGLayer::create();
  auto g = GameLayer::create();

  AddLayer(y)->Realize();
  AddLayer(g);

  y = HUDLayer::create();
  AddLayer(y)->Realize();

  // realize game layer last
  g->Realize();

  CCSX::Main = g;
  return this;
}


NS_END(invaders)

