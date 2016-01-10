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

#if !defined(__COBJS_H__)
#define __COBJS_H__

#include "ash/Ash.h"

NS_BEGIN(breakout)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Ball : public f::ComObj {
  MDECL_COMP_TPID("n/Ball")
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL BrickFence : public a::Component {
  MDECL_COMP_TPID("n/BrickFence")
  s_vec<Brick> bricks;
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Brick : public f::ComObj {
  MDECL_COMP_TPID("n/Brick")
    Brick(not_null<c::Sprite*> s, int value, int color) {
    this.ctor(sprite, 1, value);
    this.color=color;
  }

});
/**
 * @property {Brick} Brick
 */
xbox.Brick = Brick;

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
xbox.Motion = Motion;

//////////////////////////////////////////////////////////////////////////////
/**
 * @class Paddle
 */
const Paddle = sh.Ashley.compDef({

  /**
   * @memberof module:n/cobjs~Paddle
   * @method constructor
   * @param {cc.Sprite}
   */
  constructor(sprite) {
    this.ctor(sprite);
  }
});
/**
 * @property {Paddle} Paddle
 */
xbox.Paddle = Paddle;

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
xbox.Velocity = Velocity;


sjs.merge(exports, xbox);
/*@@
return xbox;
@@*/
//////////////////////////////////////////////////////////////////////////////
//EOF
NS_END(breakout)
#endif


