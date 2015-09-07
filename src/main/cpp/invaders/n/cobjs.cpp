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

#include "cobjs.h"

//////////////////////////////////////////////////////////////////////////////
//
AlienSquad::AlienSquad(const vector<>& aliens, int stepx)
  : AlienSquad() {
  this->aliens=aliens;
  this->stepx=step;
}

//////////////////////////////////////////////////////////////////////////////
//
AlienSquad::AlienSquad() {
}

//////////////////////////////////////////////////////////////////////////////
//
AlienSquad::~AlienSquad() {
}

//////////////////////////////////////////////////////////////////////////////
//
Alien::Alien(Sprite* sprite, float value, int rank)
  : ComObj(sprite, 1, value) {
  this->rank=rank;
}

//////////////////////////////////////////////////////////////////////////////
//
Alien::~Alien() () {
}

//////////////////////////////////////////////////////////////////////////////
//
Alien::Alien() () {
}


//////////////////////////////////////////////////////////////////////////////
Bomb::Bomb(Sprite* s) : ComObj(s) {
  auto wz= CCSX::VisRect();
  vel= Vec2(0, -50 * wz.height / 480);
}

//////////////////////////////////////////////////////////////////////////////
Cannon::Cannon(float coolDownWindow) {
  this->coolDownWindow= coolDownWindow;
  hasAmmo = true;
};

//////////////////////////////////////////////////////////////////////////////
Explosion::Explosion(Sprite* s) : ComObj(s) {
  frameTime= 0.1;
}

void Explosion::Inflate(options) {
    const frames = [ccsx.getSprite('boom_0.png'),
                    ccsx.getSprite('boom_1.png'),
                    ccsx.getSprite('boom_2.png'),
                    ccsx.getSprite('boom_3.png') ],
  auto anim= Animation::create();//frames, this.frameTime);
  anim->addSpriteFrame(CCSX::GetSpriteFrame("boom_0.png"));
  anim->addSpriteFrame(CCSX::GetSpriteFrame("boom_1.png"));
  anim->addSpriteFrame(CCSX::GetSpriteFrame("boom_2.png"));
  anim->addSpriteFrame(CCSX::GetSpriteFrame("boom_3.png"));
  anim->setDelayPerUnit(frameTime);

  sprite->setPosition(options.x, options.y);
  status=true;
  sprite->runAction(new cc.Sequence(new cc.Animate(anim),
      new cc.CallFunc(() => {
        sjs.loggr.debug('explosion done.!');
        this.deflate();
      }, this)
    ));
}

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

