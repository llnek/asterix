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
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "XGameLayer.h"
#include "XScene.h"

NS_ALIAS(den,CocosDenshion)
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(fusii)


//////////////////////////////////////////////////////////////////////////////
//
XGameLayer::~XGameLayer() {
  delete engine;
}

//////////////////////////////////////////////////////////////////////////////
//
XGameLayer::XGameLayer() {
  mode = GMode::ONE;
  level = 1;
  SNPTR(engine)
}

//////////////////////////////////////////////////////////////////////////////
//
ComObj* XGameLayer::GetPlayer() {
  return nullptr;
}

//////////////////////////////////////////////////////////////////////////////
//
c::Dictionary* XGameLayer::GetLCfg() {
  return XCFGS()->GetLevelCfg(s::to_string(level));
}

//////////////////////////////////////////////////////////////////////////////
//
bool XGameLayer::KeyPoll(c::EventKeyboard::KeyCode key) {
  auto it = keyboard.find(key);
  if (it != keyboard.end()) {
    return it->second;
  } else {
    return false;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
const Box4 XGameLayer::GetEnclosureBox() {
  return cx::VisBox();
}

//////////////////////////////////////////////////////////////////////////////
//
void XGameLayer::NewGame(const GMode mode) {
  if (XCFGS()->HasAudio()) {
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
  return this;
}

//////////////////////////////////////////////////////////////////////////////
//
XLayer* XGameLayer::GetBackgd(int tag) {
  auto s = GetScene();
  return s->GetLayer(tag);
}

//////////////////////////////////////////////////////////////////////////////
//
XHUDLayer* XGameLayer::GetHUD(int tag) {
  auto s= GetScene();
  return SCAST(XHUDLayer*, s->GetLayer(tag));
}

//////////////////////////////////////////////////////////////////////////////
//
void XGameLayer::update(float dt) {
  if (GetScene()->IsRunning() &&
      NNP(engine)) {
    engine->Update(dt);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
const s::map<c::EventKeyboard::KeyCode,bool>&
XGameLayer::Keys() {
  return keyboard;
}





NS_END(fusii)

