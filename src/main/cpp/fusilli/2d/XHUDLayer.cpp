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

#include "XHUDLayer.h"
USING_NS_CC;

//////////////////////////////////////////////////////////////////////////////
//
NS_FI_BEGIN

//////////////////////////////////////////////////////////////////////////////
//
XHUDLayer::~XHUDLayer() {

}

XHUDLayer::XHUDLayer() {

}

  /**
   * Remove this icon.
   * @memberof module:zotohlab/asx/scenes~XGameHUDLayer
   * @method removeIcon
   * @param {Object} icon
   */
void XHUDLayer::RemoveIcon(Node* icon) {
  RemoveAtlasItem(HudAtlas(), icon);
}

  /**
   * Add an icon.
   * @memberof module:zotohlab/asx/scenes~XGameHUDLayer
   * @method addIcon
   * @param {Object} icon
   * @param {Number} z
   * @param {Number} idx
   */
void XHUDLayer::AddIcon(Node* icon, int z, int idx) {
  AddAtlasItem(HudAtlas(), icon, z, idx);
}

void XHUDLayer::HudAtlas() {
  return m_atlasId;
}

  /**
   * @protected
   */
void XHUDLayer::Setup() {
  this.atlasId = this.options.hudAtlas || 'game-pics';
  this.scoreLabel = null;
  this.lives= null;
  this.score= 0;
  this.replayBtn = null;

  this.initAtlases();
  this.initIcons();
  this.initLabels();
  this.initCtrlBtns();
}

void XHUDLayer::InitAtlases() {}

void XHUDLayer::InitIcons() {}

void XHUDLayer::InitLabels() {}

  /**
   * @protected
   */
void XHUDLayer::InitCtrlBtns() {
  let opts = this.options.i_replay;

  if (!!opts) {
    this.addReplayIcon(ccsx.pmenu1(opts), opts.where);
  }

  opts= this.options.i_menu;
  if (!!opts) {
    this.addMenuIcon(ccsx.pmenu1(opts), opts.where);
  }
}

  /**
   * Get the score.
   * @memberof module:zotohlab/asx/scenes~XGameHUDLayer
   * @method getScore
   * @return {Number}
   */
float XHUDLayer::GetScore() { return m_score; }

  /**
   * Reset the HUD as a new game.
   * @memberof module:zotohlab/asx/scenes~XGameHUDLayer
   * @method resetAsNew
   */
void XHUDLayer::ResetAsNew() {
  Reset();
}

  /**
   * Reset the HUD.
   * @memberof module:zotohlab/asx/scenes~XGameHUDLayer
   * @method reset
   */
void XHUDLayer::Reset() {
  this.disableReplay();
  this.score= 0;
  if (this.lives) {
    this.lives.resurrect();
  }
}

  /**
   * Reduce x amount of lives.
   * @memberof module:zotohlab/asx/scenes~XGameHUDLayer
   * @method reduceLives
   * @param {Number} x
   * @return {Boolean} - true if no more lives.
   */
void XHUDLayer::ReduceLives(x) {
  this.lives.reduce(x);
  return this.lives.isDead();
}

  /**
   * Update the score.
   * @memberof module:zotohlab/asx/scenes~XGameHUDLayer
   * @method updateScore
   * @param {Number}
   */
void XHUDLayer::UpdateScore(float num) {
  this.score += num;
  this.scoreLabel.setString(Number(this.score).toString());
}

  /**
   * Disable the replay button.
   * @memberof module:zotohlab/asx/scenes~XGameHUDLayer
   * @method disableReplay
   */
void XHUDLayer::DisableReplay() {
  this.replayBtn.setVisible(false);
}

  /**
   * Enable the replay button.
   * @memberof module:zotohlab/asx/scenes~XGameHUDLayer
   * @method enableReplay
   */
void XHUDLayer::EnableReplay() {
  this.replayBtn.setVisible(true);
}

  /**
   * Add the main menu icon.
   * @memberof module:zotohlab/asx/scenes~XGameHUDLayer
   * @method addMenuItem
   * @param {cc.Menu} menu
   * @param {Object} where
   */
void XHUDLayer::AddMenuIcon(Menu* menu, where) {
  let c= menu.getChildByTag(1),
  hh = ccsx.getScaledHeight(c) * 0.5,
  hw = ccsx.getScaledWidth(c) * 0.5,
  wz= ccsx.vbox(),
  x,y;

  if (where === ccsx.acs.Bottom) {
    y = wz.bottom + csts.TILE  + hh;
  } else {
    y = wz.top - csts.TILE - hh;
  }
  menu.setPosition(wz.right - csts.TILE - hw, y);
  this.addItem(menu);
}

  /**
   * Add a replay icon.
   * @memberof module:zotohlab/asx/scenes~XGameHUDLayer
   * @method addReplayIcon
   * @param {cc.Menu} menu
   * @param {Object} where
   */
void XHUDLayer::AddReplayIcon(Menu* menu, where) {
  let c= menu.getChildByTag(1),
  hh = ccsx.getScaledHeight(c) * 0.5,
  hw = ccsx.getScaledWidth(c) * 0.5,
  wz= ccsx.vbox(),
  x, y;

  if (where === ccsx.acs.Bottom) {
    y = wz.bottom + csts.TILE  + hh;
  } else {
    y = wz.top - csts.TILE  - hh;
  }
  menu.setPosition(wz.left + csts.TILE + hw, y);
  this.replayBtn=menu;
  this.addItem(menu);
}









NS_FI_END
