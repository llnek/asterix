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
#include "Menu.h"
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
    CenterImage("game.bg");
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
  InizGame();
  Play();
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

  MGMS()->ResetPools();

  if (newFlag) {
    GetHUD()->ResetAsNew();
  } else {
    GetHUD()->Reset();
  }

  mc_del_ptr(factory)
  mc_del_ptr(engine)
}

//////////////////////////////////////////////////////////////////////////
//
void GameLayer::Replay() {
  InizGame();
  Reset(false);
  InitAsh();
  GetScene()->Resume();
}

//////////////////////////////////////////////////////////////////////////
//
void GameLayer::Play() {
  //CCLOG("play game, newflag = %s", newFlag ? "true" : "false");
  Reset(true);
  InitAsh();
  GetScene()->Resume();
}

//////////////////////////////////////////////////////////////////////////////
//
void GameLayer::InitAsh() {
  auto options = c::Dictionary::create();
  auto e = a::Engine::Reify();

  CCLOG("about to init-ash");

  factory = new Factory(e, options);
  engine = e;

  e->RegoSystem(new Stager(factory, options));
  e->RegoSystem(new Motions(factory, options));
  e->RegoSystem(new Move(factory, options));
  e->RegoSystem(new Aliens(factory, options));
  e->RegoSystem(new Collide(factory, options));
  e->RegoSystem(new Resolve(factory, options));

  CCLOG("init-ash - ok");
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
    FinzGame();
  } else {
    SpawnPlayer();
  }
}

//////////////////////////////////////////////////////////////////////////
//
void GameLayer::OnEarnScore(int score) {
  GetHUD()->UpdateScore(score);
}

//////////////////////////////////////////////////////////////////////////
//
void GameLayer::OnGameOver() {
  //CCLOG("game over!");
  CC_PCAST(Game*)->Pause();
  //Reset(false);
  GetHUD()->EnableReplay();
}

//////////////////////////////////////////////////////////////////////////
//
void GameLayer::SendMsg(const stdstr& topic, void* msg) {

  if (topic == "/game/player/earnscore") {
    f::ComObj* i = (f::ComObj*) msg;
    OnEarnScore(i->score);
  }
  else
  if (topic == "/hud/showmenu") {

    auto f= []() { CC_DTOR()->popScene(); };
    auto a= c::CallFunc::create(f);
    CC_DTOR()->pushScene(MainMenu::ReifyWithBackAction(a));
  }
  else
  if (topic == "/hud/replay") {
    Replay();
  }
  else
  if (topic == "/game/player/killed") {
    OnPlayerKilled();
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

  ReifyPool("explosions");
  ReifyPool("aliens");
  ReifyPool("missiles");
  ReifyPool("bombs");

  AddLayer(b, 1)->Realize();
  AddLayer(g, 2);
  AddLayer(h, 3)->Realize();

  // set this to be THE main game
  Bind(this);

  // realize game layer last
  g->Realize();

  return this;
}

NS_END(invaders)

