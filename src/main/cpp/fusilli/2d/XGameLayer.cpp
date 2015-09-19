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
NS_ALIAS(cx, fusilli::ccsx)
NS_BEGIN(fusilli)


//////////////////////////////////////////////////////////////////////////////
//
XGameLayer::~XGameLayer() {

}

//////////////////////////////////////////////////////////////////////////////
//
XGameLayer::XGameLayer()
  : mode(GMode::ONE)
  , level(1)
  , engine(nullptr) {

}

//////////////////////////////////////////////////////////////////////////////
//
c::Dictionary* XGameLayer::GetLCfg() {
  return XConfig::GetInstance()->GetLevelCfg(s::to_string(level));
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
const Box4 XGameLayer::GetEnclosureBox() {
  return cx::VisBox();
}

//////////////////////////////////////////////////////////////////////////////
//
void XGameLayer::NewGame(const GMode mode) {
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
XLayer* XGameLayer::GetHUD(int tag) {
  auto s= GetScene();
  return s->GetLayer(tag);
}

//////////////////////////////////////////////////////////////////////////////
//
void XGameLayer::update(float dt) {
  if (GetScene()->IsOperational() &&
      NNP(engine)) {
    engine->Update(dt);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
const s::map<int,bool>& XGameLayer::Keys() { return keyboard; }






NS_END(fusilli)

