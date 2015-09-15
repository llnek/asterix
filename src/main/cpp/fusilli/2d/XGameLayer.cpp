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

#include "audio/include/SimpleAudioEngine.h"
#include "ash/Engine.h"
#include "support/XConfig.h"
#include "support/CCSX.h"
#include "XGameLayer.h"
NS_ALIAS(den,CocosDenshion)
NS_BEGIN(fusilli)


//////////////////////////////////////////////////////////////////////////////
//
XGameLayer::~XGameLayer() {

}

//////////////////////////////////////////////////////////////////////////////
//
XGameLayer::XGameLayer()
: mode(ONE_PLAY)
, level(1)
, engine(nullptr)
, actor(nullptr) {
}

//////////////////////////////////////////////////////////////////////////////
//
void XGameLayer::PKInput() {
  //ccsx.onKeyPolls(this.keyboard);
  //ccsx.onTouchOne(this.ebus);
  //ccsx.onMouse(this.ebus);
}

//////////////////////////////////////////////////////////////////////////////
//
cc::Dictionary* XGameLayer::GetLCfg() {
  return XConfig::GetInstance()->GetLevelCfg(to_string(level));
}

//////////////////////////////////////////////////////////////////////////////
//
bool XGameLayer::KeyPoll(int key) {
  auto it = keyboard.find(key);
  if (it != keyboard.end()) {
    return it->second;
  } else {
    return false;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void XGameLayer::InitEngine(const s::vector<a::System*>& ss) {
  engine = Engine::create();
  for (auto it= ss.begin(); it != ss.end(); ++it) {
    engine->AddSystem(*it);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
const Box4 XGameLayer::GetEnclosureBox() {
  return CCSX::VisBox();
}

//////////////////////////////////////////////////////////////////////////////
//
void XGameLayer::SetGameMode(const GameMode mode) {
  this->mode= mode;
}

//////////////////////////////////////////////////////////////////////////////
//
void XGameLayer::NewGame(const GameMode mode) {
  if (XConfig::GetInstance()->HasAudio()) {
    den::SimpleAudioEngine::getInstance()->stopAllEffects();
    den::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
  }
  OnNewGame(mode);
  this->scheduleUpdate();
}

//////////////////////////////////////////////////////////////////////////////
//
XLayer* XGameLayer::Realize() {
  NewGame(mode);
}

//////////////////////////////////////////////////////////////////////////////
//
XLayer* XGameLayer::GetBackgd() {
  auto s = GetScene();
  return s->GetLayer("BackLayer");
}

//////////////////////////////////////////////////////////////////////////////
//
XLayer* XGameLayer::GetHUD() {
  auto s= GetScene();
  return s->GetLayer("HUD");
}

//////////////////////////////////////////////////////////////////////////////
//
void XGameLayer::update(float dt) {
  if (IsOperational() && NNP(engine)) {
    engine->Update(dt);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
const s::map<int,bool>& XGameLayer::Keys() { return keyboard; }






NS_END(fusilli)

