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
USING_NS_CC;

//////////////////////////////////////////////////////////////////////////////
//
NS_FI_BEGIN

//////////////////////////////////////////////////////////////////////////////
//
XGameLayer::~XGameLayer() {

}

XGameLayer::XGameLayer() {

}

void XGameLayer::KeyInput() {
  //ccsx.onKeyPolls(this.keyboard);
  //ccsx.onTouchOne(this.ebus);
  //ccsx.onMouse(this.ebus);
}

  /**
   * @memberof module:zotohlab/asx/scenes~XGameLayer
   * @method getLCfg
   * @return {Object}
   */
void XGameLayer::GetLCfg() {
  return sh.getLevelCfg(this.level);
}


  /**
   * @memberof module:zotohlab/asx/scenes~XGameLayer
   * @method signal
   * @protected
   * @param {String} topic
   * @param {Function} cb
   */
void XGameLayer::Signal(topic, cb) {
  this.ebus.on(topic,cb);
}

  /**
   * @memberof module:zotohlab/asx/scenes~XGameLayer
   * @method keyPoll
   * @protected
   * @param {Number} key
   */
void XGameLayer::KeyPoll(key){
  return this.keyboard[key];
}

  /**
   * @method initEngine
   * @protected
   */
void XGameLayer::InitEngine(syss, fact) {
  this.engine = this.newFlow();
  if (sjs.isfunc(fact)) {
    fact(this.engine, this.options);
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
const Box4 XGameLayer::GetEnclosureBox() {
  return CCSX::VisBox();
  /*
  var csts = xcfg.csts,
  wz = ccsx.vrect();
  return { top: wz.height - csts.TILE,
           left: csts.TILE,
           bottom: csts.TILE,
           right: wz.width - csts.TILE };
   * */
},

  /**
   * @memberof module:zotohlab/asx/scenes~XGameLayer
   * @method setGameMode
   * @param {Number} mode
   */
void XGameLayer::SetGameMode(mode) {
  xcfg.csts.GAME_MODE=mode;
}

  /**
   * Reset and create new Ash Engine.
   *
   * @memberof module:zotohlab/asx/scenes~XGameLayer
   * @method newFlow
   */
void XGameLayer::NewFlow() {
  return this.engine= new Ash.Engine();
}

  /**
   * @memberof module:zotohlab/asx/scenes~XGameLayer
   * @method newGame
   * @param {Number} mode
   */
void XGameLayer::NewGame(mode) {
  if (xcfg.sound.open) {
    cc.audioEngine.stopAllEffects();
    cc.audioEngine.stopMusic();
  }
  this.onNewGame(mode);
  this.scheduleUpdate();
}

  /**
   * @memberof module:zotohlab/asx/scenes~XGameLayer
   * @method setup
   * @protected
   */
void XGameLayer::Setup() {
  const m= this.options.mode;
  if (m === sh.gtypes.ONLINE_GAME ||
      m === sh.gtypes.P2_GAME ||
      m === sh.gtypes.P1_GAME) {
    this.newGame(m);
  }
}

  /**
   * @memberof module:zotohlab/asx/scenes~XGameLayer
   * @method operational
   * @return {Boolean}
   */
bool XGameLayer::Operational() { return true; }

  /**
   * @memberof module:zotohlab/asx/scenes~XGameLayer
   * @method getBackgd
   * @return {cc.Layer} - background layer
   */
Layer* XGameLayer::GetBackgd() {
  const rc= this.ptScene.getLayers();
  return rc['BackLayer'];
}

  /**
   * @memberof module:zotohlab/asx/scenes~XGameLayer
   * @method getHUD
   * @return {cc.Layer}  the HUD layer
   */
Layer* XGameLayer::GetHUD() {
  const rc= this.ptScene.getLayers();
  return rc['HUD'];
}

  /**
   * @memberof module:zotohlab/asx/scenes~XGameLayer
   * @method update
   */
void XGameLayer::update(float dt) {
  if (this.operational()  && !!this.engine) {
    this.engine.update(dt);
  }
}

  /**
   * @memberof module:zotohlab/asx/scenes~XGameLayer
   * @method keys
   * @return {Array}  keys
   */
void XGameLayer::Keys() { return this.keyboard; }

XGameLayer::~XGameLayer() {
}

XGameLayer::XGameLayer() {
  this.ebus= ebus.reify();
  this.keyboard= [];
  this.players= [];
  this.level= 1;
  this.actor= null;
  sh.main = this;
  const vbox= ccsx.vbox();
  sjs.loggr.debug('cc.view: vbox: left: ' + vbox.left +
                  ', bottom: ' + vbox.bottom +
                  ', top: ' + vbox.top +
                  ', right: ' + vbox.right);
}










NS_FI_END
