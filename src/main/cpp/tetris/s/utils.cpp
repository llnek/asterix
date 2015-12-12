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

#include "utils.h"

NS_BEGIN(tetris)

//////////////////////////////////////////////////////////////////////////////
//
Shape* reifyShape(not_null<f::XLayer*> layer, cmap, Shape& shape) {
  auto bbox= findBBox(cmap, shape.model,
                          shape.x, shape.y, shape.rot);
  auto bricks;
  auto rc=nullptr;

  if (bbox.length > 0) {
    bricks= reifyBricks(layer, shape.png,
                             shape.x, shape.y, bbox);
    clearOldBricks(shape.bricks);
    shape.bricks=bricks;
    rc=shape;
  }

  return rc;
}

//////////////////////////////////////////////////////////////////////////
//
float topLine(a::Node* node) {
  auto gbox= CC_GNF(GridBox, node, "gbox");
  auto bx= gbox.box;
  return floor((bx.top - bx.bottom) / MGMS()->TILE);
}

//////////////////////////////////////////////////////////////////////////
//
Shape* previewShape(not_null<f::XLayer*> layer, Shape& shape) {
  auto bbox= findBBox([],shape.model,
                          shape.x,shape.y,shape.rot,true);
  bricks;
  if (bbox.length > 0) {
    bricks= reifyBricks(layer,shape.png,
                             shape.x,shape.y,bbox);
    clearOldBricks(shape.bricks);
    shape.bricks=bricks;
  }
  return shape;
}

//////////////////////////////////////////////////////////////////////////
//
void disposeShape(Shape* shape) {
  if (NNP(shape)) {
    clearOldBricks(shape->bricks);
    shape->bricks.length=0;
  }
}

//////////////////////////////////////////////////////////////////////////
//
void clearOldBricks(bs) {
  F__LOOP(it, bs) {
    auto& z= *it;
    z->dispose()
  }
}

//////////////////////////////////////////////////////////////////////////
//
void reifyBricks(not_null<f::XLayer*> layer, const sstr& png, x,y, bs) {
  let pt, obj,
  bricks=[];

  F__LOOP(it, bs) {
    auto& pt = *it;
    auto obj= new Brick( pt.x, pt.y, { frame: png } );
    layer->addAtlasItem("game-pics",obj.create());
    bricks.push(obj);
  }

  return bricks;
}

//////////////////////////////////////////////////////////////////////////
//
const s_vec<c::Vec2>
findBBox(cmap, model, left, top, rID, skipCollide) {
  auto vptr= model->getLayout();
  s_vec<c::Vec2> bs;

  let form= model.layout[rID],
  x,y,
  pt,

  for (auto r=0; r < model->dim; ++r) {
    y = top - MGMS()->TILE * r;
    for (auto c=0; c < model->dim; ++c) {
      x = left + MGMS()->TILE * c;
      if (form[r][c] == 1) {
        if (!skipCollide &&
            maybeCollide(cmap, x, y,
                              x + MGMS()->TILE,
                              y - MGMS()->TILE)) {
          return [];
        }
        bs.push(c::ccp(x,y));
      }
    }
  }
  return bs;
}

//////////////////////////////////////////////////////////////////////////
//
bool maybeCollide(cmap, tl_x, tl_y, br_x, br_y) {
  auto tile= xrefTile(tl_x , tl_y);

  CCLOG("tile = %d, %d", tile.row , tile.col);

  if (tile.row < 0 || tile.col < 0 ||
      cmap[tile.row][tile.col] != 0)  {
    CCLOG("collide! tile = %d, %d", tile.row , tile.col);
    return true;
  } else {
    return false;
  }
}

//////////////////////////////////////////////////////////////////////////
//
const c::Size xrefTile(x,y) {
  // find center, instead of top left
  auto co = MGMS()->TILE * 0.5;
  auto y -= co;
  auto x += co;

  // realign actual x,y
  x -= MGMS()->CBOX.left - MGMS()->FENCE;

  return c::Size( floor(x / MGMS()->TILE),
           floor(y / MGMS()->TILE)
           //col: Math.floor(x / csts.TILE),
           //row: Math.floor(y / csts.TILE)
      );
}

//////////////////////////////////////////////////////////////////////////
//
void initDropper(not_null<c::Node*> par, dp) {
  dp.timer = cx::createTimer(par, dp.dropRate / dp.dropSpeed);
}

//////////////////////////////////////////////////////////////////////////
//
void setDropper(not_null<c::Node*> par, dp, r, s) {
  dp.timer = cx::createTimer(par, r/s);
  dp.dropSpeed=s;
  dp.dropRate=r;
}

//////////////////////////////////////////////////////////////////////////
//
void lockBricks(cmap, emap, z) {
  auto zs = z->sprite->getPosition();
  auto t= xrefTile(zs.x, zs.y);

  cmap[t.row][t.col] = 1;
  emap[t.row][t.col] = z;
}

//////////////////////////////////////////////////////////////////////////
//
lock(a::Node* node, shape) {
  auto cmap= CC_GNF(TileGrid, node, "collision")->tiles;
  auto emap= CC_GNF(BlockGrid, node, "blocks")->grid;

  R.forEach((z) => {
    this.lockBricks(cmap, emap, z);
  }, shape.bricks);

  postLock(node, cmap, emap);
}

//////////////////////////////////////////////////////////////////////////
//
postLock(a::Node* node, cmap, emap) {

  // search bottom up until top.
  auto top= cmap.length;
  s_vec<> rc;

  // 0 is the bottom wall
  for (auto r = 1; r < top; ++r) {
    if (testFilledRow(cmap, r)) {
      rc.push_back(r);
    }
  }

  if (rc.size() > 0) {
    pauseForClearance(node, true, 0.5);
    flashFilled(emap, CC_GNF(FilledLines, node, "flines"), rc);
  }
}

//////////////////////////////////////////////////////////////////////////
//
bool testFilledRow(cmap, r) {
  auto row= cmap[r];

  // negative if any holes in the row
  for (auto c=0; c < row.size(); ++c) {
    if (row[c] != 1) { return false; }
  }

  // entire row must be filled.
  return true;
}

//////////////////////////////////////////////////////////////////////////
//
void flashFilled(emap, flines, lines) {
  R.forEach((z) => {
    let row= emap[z];
    for (let c=0; c < row.length; ++c) {
      if (row[c]) {
        row[c].blink();
      }
    }
  }, lines);

  flines->lines=lines;
}

//////////////////////////////////////////////////////////////////////////
//
void pauseForClearance(not_null<a::Node*> node, b, float delay) {
  auto pu= CC_GNF(Pauser, node, "pauser");
  auto flines = CC_GNF(FilledLines, node, "flines");

  pu->pauseToClear=b;
  flines->lines=[];

  if (b) {
    pu->timer= cx::createTimer(MGML(), delay);
  } else {
    pu->timer=nullptr;
  }
}

//////////////////////////////////////////////////////////////////////////
//
bool moveDown(not_null<f::XLayer*> layer, cmap, shape) {
  auto new_y = shape.y - MGMS()->TILE;
  x = shape.x,
  bricks,
  auto bs = findBBox(cmap, shape.model,
                     x, new_y, shape.rot);

  if (bs.length > 0) {
    bricks= reifyBricks(layer,shape.png, x, new_y, bs);
    clearOldBricks(shape.bricks);
    shape.bricks=bricks;
    shape.y= new_y;
    return true;
  } else {
    return false;
  }
}

//////////////////////////////////////////////////////////////////////////
//
bool shiftRight(not_null<f::XLayer*> layer, cmap, shape) {
  auto new_x= shape.x + csts.TILE,
  y= shape.y,
  bricks,
  bs= findBBox(cmap, shape.model,
                    new_x, y, shape.rot);

  if (bs.length > 0) {
    bricks= reifyBricks(layer,shape.png, new_x, y, bs);
    clearOldBricks(shape.bricks);
    shape.bricks=bricks;
    shape.x= new_x;
    return true;
  } else {
    return false;
  }
}

//////////////////////////////////////////////////////////////////////////
//
bool shiftLeft(not_null<f::XLayer*> layer, cmap, shape) {
  auto new_x= shape.x - MGMS()->TILE;
  y= shape.y,
  bricks,
  bs= findBBox(cmap, shape.model,
                    new_x, y, shape.rot);

  if (bs.length > 0) {
    bricks= reifyBricks(layer,shape.png, new_x, y, bs);
    clearOldBricks(shape.bricks);
    shape.bricks=bricks;
    shape.x= new_x;
    return true;
  } else {
    return false;
  }
}

//////////////////////////////////////////////////////////////////////////
//
bool rotateRight(not_null<f::XLayer*> layer,cmap,shape) {
  auto nF = sjs.xmod(shape.rot+1, shape.model.layout.length);
  bricks,
  auto bs= findBBox(cmap, shape.model,
                    shape.x, shape.y, nF);

  CCLOG("shape.rot = " + shape.rot +
                  ", dim = " +
                  shape.model.dim +
                  ", rot-right , nF = " + nF);
  if (bs.length > 0) {
    bricks= reifyBricks(layer,shape.png, shape.x, shape.y, bs);
    clearOldBricks(shape.bricks);
    shape.bricks=bricks;
    shape.rot= nF;
    return true;
  } else {
    return false;
  }
}

//////////////////////////////////////////////////////////////////////////
//
bool rotateLeft(not_null<f::XLayer*> layer,cmap,shape) {
  auto nF = sjs.xmod(shape.rot-1, shape.model.layout.length),
  bricks,
  bs= findBBox(cmap, shape.model,
                    shape.x, shape.y, nF);

  sjs.loggr.debug("shape.rot = " + shape.rot +
                  ", dim = " +
                  shape.model.dim +
                  ", rot-right , nF = " + nF);
  if (bs.length > 0) {
    bricks= reifyBricks(layer,shape.png, shape.x, shape.y, bs);
    clearOldBricks(shape.bricks);
    shape.bricks=bricks;
    shape.rot= nF;
    return true;
  } else {
    return false;
  }
}


NS_END(tetris)


