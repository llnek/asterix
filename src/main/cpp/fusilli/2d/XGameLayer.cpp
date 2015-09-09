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

#include "XGameLayer.h"
NS_BEGIN(fusilli)

//////////////////////////////////////////////////////////////////////////////
//
XGameLayer::~XGameLayer() {

}

//////////////////////////////////////////////////////////////////////////////
//
XGameLayer::XGameLayer() {

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
Dictionary* XGameLayer::GetLCfg() {
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
void XGameLayer::InitEngine(syss, fact) {
  engine = new Engine();
  if (sjs.isfunc(fact)) {
    fact(engine, this.options);
  }
  R.forEach( z => {
    this.engine.addSystem(new (z)(this.options), z.Priority);
  }, R.filter( x => {
    return sjs.isfunc(x);
  }, syss));
}

  /**
   * @memberof module:zotohlab/asx/scenes~XGameLayer
   * @method getEnclosureBox
   * @return {Object} rect box.
   */
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
  if (xcfg.sound.open) {
    SimpleAudioEngine::getInstance()->stopAllEffects();
    SimpleAudioEngine::getInstance()->stopMusic();
  }
  OnNewGame(mode);
  this->scheduleUpdate();
}

//////////////////////////////////////////////////////////////////////////////
//
void XGameLayer::Realize() {
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
  if (IsOerational()  && NNP(engine)) {
    engine->Update(dt);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
const map<int,bool>& XGameLayer::Keys() { return keyboard; }

//////////////////////////////////////////////////////////////////////////////
//
XGameLayer::~XGameLayer() {
}

//////////////////////////////////////////////////////////////////////////////
//
XGameLayer::XGameLayer() {
  actor= nullptr;
  level= 1;
  //sh.main = this;
}












NS_END(fusilli)

