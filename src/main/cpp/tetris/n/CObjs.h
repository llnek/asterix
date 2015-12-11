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

NS_BEGIN(tetris)

typedef int DIM4x4[4][4];
typedef int DIM3x3[3][3];
typedef int DIM2x2[2][2];

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL ShapeShell {
  int shape;
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Shape {
  model= options.model;
  rot= options.rot;
  sstr png;
  float x;
  float y;
  s_vec<> bricks; //=[];
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL CtrlPad {
  hotspots= {};
};

//////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GridBox {
  box= {};
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL BlockGrid {
  grid=[];
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL BoxShape : public XXXShape {
  s_arr<DIM2X2,4> layout = {
    {1,1,
     1,1},
    {1,1,
     1,1},
    {1,1,
     1,1},
    {1,1,
     1,1}
  };
  int dim=2;
};

struct ElShape : public XXXShape {
  s_arr<DIM3X3,4> layout = {
    { 0,1,0,
      0,1,0,
      0,1,1 },
    { 0,0,0,
      1,1,1,
      1,0,0 },
    { 1,1,0,
      0,1,0,
      0,1,0 },
    { 0,0,1,
      1,1,1,
      0,0,0  }
  };
  int dim=3;
};

struct ElxShape : public XXXShape {
  s_arr<DIM3X3,4> layout = {
    { 0,1,0,
      0,1,0,
      1,1,0 },
    { 1,0,0,
      1,1,1,
      0,0,0 },
    { 0,1,1,
      0,1,0,
      0,1,0 },
    { 0,0,0,
      1,1,1,
      0,0,1 }
  };
  int dim=3;
};

struct LineShape : public XXXShape {
  s_arr<DIM4x4,4> layout = {
    { 0,0,0,0,
      1,1,1,1,
      0,0,0,0,
      0,0,0,0 },
    { 0,0,1,0,
      0,0,1,0,
      0,0,1,0,
      0,0,1,0 },
    { 0,0,0,0,
      0,0,0,0,
      1,1,1,1,
      0,0,0,0 },
    { 0,1,0,0,
      0,1,0,0,
      0,1,0,0,
      0,1,0,0 }
  };
  int dim=4;
};

struct NubShape : public XXXShape {
  s_arr<DIM3x3,4> layout= {
    { 0,0,0,
      0,1,0,
      1,1,1 },
    { 1,0,0,
      1,1,0,
      1,0,0 },
    { 1,1,1,
      0,1,0,
      0,0,0 },
    { 0,0,1,
      0,1,1,
      0,0,1 }
  };
  int dim= 3;
};

struct CC_DLL StShape : public XXXShape {
  s_arr<DIM3x3,4> layout= {
    { 0,1,0,
      0,1,1,
      0,0,1 },
    { 0,0,0,
      0,1,1,
      1,1,0 },
    { 1,0,0,
      1,1,0,
      0,1,0 },
    { 0,1,1,
      1,1,0,
      0,0,0 }
  };
  int dim= 3;
};

struct CC_DLL StxShape : public XXXShape {
  s_arr<DIM3x3,4> layout = {
    { 0,1,0,
      1,1,0,
      1,0,0 },
    { 1,1,0,
      0,1,1,
      0,0,0 },
    { 0,0,1,
      0,1,1,
      0,1,0 },
    { 0,0,0,
      1,1,0,
      0,1,1 }
  };
  int dim= 3;
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Block : public c::Sprite {
  void blink() {
    setAnchorPoint(cx::anchorTL());
    setSpriteFrame(frame1);
  }
  void show() {
    setAnchorPoint(cx::anchorTL());
    setSpriteFrame(frame0);
  }
  Block(float x, float y, options) {
    this.options = options;
    this.frame0 = ccsx.getSprite(options.frames[0]);
    this.frame1 = ccsx.getSprite(options.frames[1]);
    this._super();
    this.show();
    this.setPosition(x,y);
  }
};

/**
 * @class Brick
 */
class Brick extends sjs.ES6Claxx {
  /**
   * @memberof module:n/cobjs~Brick
   * @method blink
   */
  blink() {
    if ( !!this.sprite) { this.sprite.blink(); }
  }
  /**
   * @memberof module:n/cobjs~Brick
   * @method dispose
   */
  dispose() {
    if (!!this.sprite) {
      this.sprite.removeFromParent();
      this.sprite=null;
    }
  }
  /**
   * @memberof module:n/cobjs~Brick
   * @method create
   */
  create() {
    return this.sprite = new Block(this.startPos.x, this.startPos.y, this.options);
  }
  /**
   * @memberof module:n/cobjs~Brick
   * @method constructor
   * @param {Number} x
   * @param {Number} y
   * @param {Object} options
   */
  constructor(x, y, options) {
    super();
    this.options = options || {};
    this.startPos = cc.p(x,y);
    this.options.frames= [ '' + options.frame + '.png', '0.png'];
  }
}
xbox.Brick = Brick;

//////////////////////////////////////////////////////////////////////////////
/**
 * @class Dropper
 */
xbox.Dropper= sh.Ashley.casDef({
  /**
   * @memberof module:n/cobjs~Dropper
   * @method constructor
   */
  constructor() {
    this.dropSpeed = csts.DROPSPEED;
    this.dropRate= 80 + 700/1 ;
    this.timer=null;
  }
});

//////////////////////////////////////////////////////////////////////////////
/**
 * @class ElShape
 */
//////////////////////////////////////////////////////////////////////////////
/**
 * @class ElxShape
 */
//////////////////////////////////////////////////////////////////////////////
/**
 * @class FilledLines
 */
xbox.FilledLines= sh.Ashley.casDef({
  /**
   * @memberof module:n/cobjs~FilledLines
   * @method constructor
   */
  constructor() {
    this.lines=[];
  }
});

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
/**
 * @class Motion
 */
xbox.Motion= sh.Ashley.casDef({
  /**
   * @memberof module:n/cobjs~Motion
   * @method constructor
   */
  constructor() {
    this.right=false;
    this.left=false;
    this.rotr= false;
    this.rotl= false;
    this.down=false;
  }
});

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
/**
 * @class Pauser
 */
xbox.Pauser= sh.Ashley.casDef({

  /**
   * @memberof module:n/cobjs~Pauser
   * @method constructor
   */
  constructor() {
    this.pauseToClear=false;
    this.timer=null;
  }
});

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
/**
 * @class TileGrid
 */
xbox.TileGrid= sh.Ashley.casDef({
  /**
   * @memberof module:n/cobjs~TileGrid
   * @method constructor
   */
  constructor() {
    this.tiles=[];
  }
});

//////////////////////////////////////////////////////////////////////////////
/**
 * @property {Array} Shapes
 */
xbox.Shapes = [xbox.LineShape,
               xbox.BoxShape,
               xbox.StShape,
               xbox.ElShape,
               xbox.NubShape,
               xbox.StxShape,
               xbox.ElxShape ];



NS_END(tetris)
#endif

