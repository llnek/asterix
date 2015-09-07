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

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL AlienSquad : public Component {
public:

  AlienSquad(const vector<Alien>& aliens, int step);
  virtual ~AlienSquad();

private:

  DISALLOW_COPYASSIGN(AlienSquad)
  AlienSquad();
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Alien : public ComObj {
public:

  Alien(Sprite* sprite, float value, int rank);
  virtual ~Alien();

private:
  DISALLOW_COPYASSIGN(Alien)
  Alien();

};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Bomb : public ComObj {
public:

  virtual ~Bomb();
  Bomb(Sprite*);

private:
  DISALLOW_COPYASSIGN(Bomb)
  Bomb();
};

//////////////////////////////////////////////////////////////////////////////
class CC_DLL Cannon : public Component {
public:

  Cannon(float coolDownWindow);
  virtual ~Cannon();

private:

  DISALLOW_COPYASSIGN(Cannon)
  float coolDownWindow;
};


//////////////////////////////////////////////////////////////////////////////
/**
 * @class Explosion
 */
const Explosion = sh.Ashley.compDef({

  /**
   * @memberof module:n/cobjs~Explosion
   * @method constructor
   * @param {cc.Sprite}
   */
  constructor(sprite) {
    this.ctor(sprite);
    this.frameTime= 0.1;
  },

  /**
   * @protected
   */
  inflate(options) {
    const frames = [ccsx.getSprite('boom_0.png'),
                    ccsx.getSprite('boom_1.png'),
                    ccsx.getSprite('boom_2.png'),
                    ccsx.getSprite('boom_3.png') ],
    anim= new cc.Animation(frames, this.frameTime);

    this.sprite.setPosition(options.x, options.y);
    this.status=true;

    this.sprite.runAction(new cc.Sequence(new cc.Animate(anim),
      new cc.CallFunc(() => {
        sjs.loggr.debug('explosion done.!');
        this.deflate();
      }, this)
    ));
  }

});
/**
 * @property {Explosion} Explosion
 */
xbox.Explosion = Explosion;

//////////////////////////////////////////////////////////////////////////////
/**
 * @class Looper
 */
const Looper = sh.Ashley.casDef({

  /**
   * @memberof module:n/cobjs~Looper
   * @method constructor
   * @param {Number} count
   */
  constructor(count) {
    this.timers=sjs.makeArray(count,null);
  }
});
/**
 * @property {Looper} Looper
 */
xbox.Looper= Looper;

//////////////////////////////////////////////////////////////////////////////
/**
 * @class Missile
 */
const Missile = sh.Ashley.compDef({

  /**
   * @memberof module:n/cobjs~Missile
   * @method constructor
   * @param {cc.Sprite} sprite
   */
  constructor(sprite) {
    const wz= ccsx.vrect();
    this.ctor(sprite);
    this.vel= {
      x: 0,
      y: 150 * wz.height / 480
    };
  }
});
/**
 * @property {Missile} Missile
 */
xbox.Missile= Missile;

//////////////////////////////////////////////////////////////////////////////
/**
 * @class Motion
 */
const Motion = sh.Ashley.casDef({

  /**
   * @memberof module:n/cobjs~Motion
   * @method constructor
   */
  constructor() {
    this.right = false;
    this.left = false;
  }
});
/**
 * @property {Motion} Motion
 */
xbox.Motion= Motion;

//////////////////////////////////////////////////////////////////////////////
/**
 * @class Ship
 */
const Ship = sh.Ashley.compDef({

  /**
   * @memberof module:n/cobjs~Ship
   * @method constructor
   * @param {cc.Sprite} sprite
   * @param {Array} frames
   */
  constructor(sprite,frames) {
    this.ctor(sprite);
    this.frames=frames;
  }
});
/**
 * @property {Ship} Ship
 */
xbox.Ship = Ship;

//////////////////////////////////////////////////////////////////////////////
/**
 * @class Velocity
 */
const Velocity = sh.Ashley.casDef({

  /**
   * @memberof module:n/cobjs~Velocity
   * @method constructor
   * @param {Number} vx
   * @param {Number} vy
   */
  constructor(vx,vy) {
    this.vel = {
      x: vx || 0,
      y: vy || 0
    };
  }
});
/**
 * @property {Velocity} Velocity
 */
xbox.Velocity= Velocity;




sjs.merge(exports, xbox);
/*@@
return xbox;
@@*/
//////////////////////////////////////////////////////////////////////////////
//EOF

