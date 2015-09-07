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

#include "Component.h"


class CC_DLL ComObj : public Component {
  /**
   * Take damage, reduce health.
   * @memberof module:zotohlab/asterix~ComObj
   * @method hurt
   * @param {Number} damage
   * @param {Object} from
   */
  hurt(damage, from) {
    this.HP -= sjs.isnum(damage) ? damage : 1;
  },

  /**
   * Reborn from the dead - take from the pool.
   * @memberof module:zotohlab/asterix~ComObj
   * @method inflate
   * @param {Object} options
   */
  inflate(options) {
    options= options || {};
    if (!!this.sprite) {
      if (sjs.hasKey(options, 'x') &&
          sjs.hasKey(options, 'y')) {
        this.sprite.setPosition(options.x,
                                options.y);
      }
      if (sjs.hasKey(options, 'deg')) {
        this.sprite.setRotation(options.deg);
      }
      if (sjs.hasKey(options, 'scale')) {
        this.sprite.setScale(options.scale);
      }
      this.sprite.setVisible(true);
    }
    this.HP= this.origHP;
    this.status=true;
  },

  /**
   * Die and ready to be recycled.
   * @memberof module:zotohlab/asterix~ComObj
   * @method deflate
   */
  deflate() {
    if (!!this.sprite) {
      this.sprite.unscheduleAllCallbacks();
      this.sprite.stopAllActions();
      this.sprite.setVisible(false);
    }
    this.status=false;
  },

  /**
   * Get Sprite's height.
   * @memberof module:zotohlab/asterix~ComObj
   * @method height
   * @return {Number}
   */
  height() {
    if (!!this.sprite) {
      return this.sprite.getContentSize().height;
    }
  },

  /**
   * Get Sprite's width.
   * @memberof module:zotohlab/asterix~ComObj
   * @method width
   * @return {Number}
   */
  width() {
    if (!!this.sprite) {
      return this.sprite.getContentSize().width;
    }
  },

  /**
   * Set Sprite's position.
   * @memberof module:zotohlab/asterix~ComObj
   * @method setPos
   * @param {Number} x
   * @param {Number} y
   */
  setPos(x,y) {
    if (!!this.sprite) {
      this.sprite.setPosition(x,y);
    }
  },

  /**
   * Get the Sprite's position.
   * @memberof module:zotohlab/asterix~ComObj
   * @method pos
   * @return {cc.Point}
   */
  pos() {
    if (!!this.sprite) {
      return this.sprite.getPosition();
    }
  },

  /**
   * Get the Sprite's size.
   * @memberof module:zotohlab/asterix~ComObj
   * @method size
   * @return {cc.Size}
   */
  size() {
    if (!!this.sprite) {
      return this.sprite.getContentSize();
    }
  },

  /**
   * Get the tag value.
   * @memberof module:zotohlab/asterix~ComObj
   * @method rtti
   * @return {String}
   */
  rtti() { return this._name; },

  /**
   * Set tag value.
   * @memberof module:zotohlab/asterix~ComObj
   * @method rego
   * @param {String} n
   */
  rego(n) { this._name = n; },

  /**
   * Get the Sprite's tag value.
   * @memberof module:zotohlab/asterix~ComObj
   * @method pid
   * @return {Number}
   */
  pid() {
    if (!!this.sprite) { return this.sprite.getTag(); }
  },

  /**
   * @private
   */
  ctor(sprite, health, score) {
    this._name= ["co" , ++SEED].join(':');
    this.origHP = health || 1;
    this.sprite = sprite;
    this.HP = this.origHP;
    this.value = score || 0;
    this.status=false;
  }
},

/**
 * @extends module:zotohlab/asterix~ComObj
 * @class SimpleComp
 */
SimpleComp = Ash.Class.extend(sjs.mergeEx(ComObj, {

  /**
   * @memberof module:zotohlab/asterix~SimpleComp
   * @method constructor
   * @param {cc.Sprite}
   */
  constructor(sprite) {
    this.ctor(sprite);
  }
})),


