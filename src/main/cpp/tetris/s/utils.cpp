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

#include "x2d/GameScene.h"
#include "utils.h"

NS_BEGIN(tetris)

//////////////////////////////////////////////////////////////////////////////
//
BEGIN_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
owner<Shape*> mkShape(not_null<f::XLayer*> layer,
    float x, float y,
    const ShapeInfo &info, const s_vec<c::Vec2>& bbox) {

  Shape *rc=nullptr;

  if (bbox.size() > 0) {
    auto bs= reifyBricks(layer, info.png, bbox);
    rc= mc_new_1(Shape, info);
    rc->x=x;
    rc->y=y;
    rc->bricks.resize(bs.size());
    s::copy(bs.begin(), bs.end(), rc->bricks.begin());
  }

  return rc;
}

END_NS_UNAMED()

//////////////////////////////////////////////////////////////////////////////
//
owner<Shape*> reifyShape(not_null<f::XLayer*> layer,
    s_vec<f::FArrInt> &cmap,
    float x, float y, const ShapeInfo &info) {

  auto bbox= findBBox(cmap, info.model, x, y, info.rot);
  return mkShape(layer, x, y, info, bbox);
}

//////////////////////////////////////////////////////////////////////////
//
owner<Shape*> previewShape(not_null<f::XLayer*> layer,
    float x, float y,
    const ShapeInfo& info) {
    s_vec<f::FArrInt> dummy;
  auto bbox= findBBox(dummy, info.model, x, y, info.rot, true);
  return mkShape(layer, x, y, info, bbox);
}

//////////////////////////////////////////////////////////////////////////
//
float topLine(not_null<a::Node*> node) {
  auto gx= CC_GNF(GridBox, node, "gbox");
  return floor((gx->box.top - gx->box.bottom) / CC_CSV(c::Integer, "TILE"));
}

//////////////////////////////////////////////////////////////////////////
//
void disposeShape(Shape *shape) {
  if (NNP(shape)) {
    clearOldBricks(shape->bricks);
  }
}

//////////////////////////////////////////////////////////////////////////
//
void clearOldBricks(s_vec<Brick*> &bs) {
  F__LOOP(it, bs) {
    auto& z= *it;
    z->dispose();
  }
  bs.clear();
}

//////////////////////////////////////////////////////////////////////////
//
const s_vec<Brick*> reifyBricks(not_null<f::XLayer*> layer,
    const sstr &png,
    const s_vec<c::Vec2> &bs) {

  s_vec<Brick*> bricks;

  F__LOOP(it, bs) {
    auto obj= Brick::reify( *it, png );
    bricks.push_back(obj);
    layer->addAtlasItem("game-pics", obj);
  }

  return bricks;
}

//////////////////////////////////////////////////////////////////////////
//
const s_vec<c::Vec2>
findBBox(s_vec<f::FArrInt> &cmap, BModel *model,
    float px, float py, int rID, bool skipCollide) {

  auto tile = CC_CSV(c::Integer, "TILE");
  s_vec<c::Vec2> bs;
  float x,y;

  for (auto r=0; r < model->getDim(); ++r) {
    y = py - tile * r;
    for (auto c=0; c < model->getDim(); ++c) {
      x = px + tile * c;
      if (model->test(rID,r,c)) {
        if (!skipCollide &&
            maybeCollide(cmap, x, y,
                              x + tile,
                              y - tile)) {
          return s_vec<c::Vec2> {};
        }
        bs.push_back(c::ccp(x,y));
      }
    }
  }

  return bs;
}

//////////////////////////////////////////////////////////////////////////
//
bool maybeCollide(s_vec<f::FArrInt> &cmap,
    float tl_x, float tl_y, float br_x, float br_y) {

  auto tile= xrefTile(tl_x , tl_y);
  CCLOG("tile = %d, %d", tile.row , tile.col);

  if (tile.row < 0 || tile.col < 0 ) {
    auto &cm= cmap[tile.row];
    if (cm.get(tile.col) != 0)  {
      CCLOG("collide! tile = %d, %d", tile.row , tile.col);
      return true;
    }
  }
  return false;
}

//////////////////////////////////////////////////////////////////////////
//
const f::Cell2D xrefTile(float x, float y) {
  // find center, instead of top left
  auto tile = CC_CSV(c::Integer, "TILE") ;
  auto co = tile * 0.5f;
  auto bx= CC_CSV(f::Box4R, "CBOX");
  y -= co;
  x += co;

  // realign actual x,y
  x -= bx.left - CC_CSV(c::Integer, "FENCE");

  return f::Cell2D(
           floor(y / tile),
           floor(x / tile));
}

//////////////////////////////////////////////////////////////////////////
//
void initDropper(Dropper *dp) {
  dp->timer = cx::reifyTimer(MGML(), dp->dropRate / dp->dropSpeed);
}

//////////////////////////////////////////////////////////////////////////
//
void setDropper(not_null<c::Node*> par, Dropper *dp, float r, float s) {
  dp->timer = cx::reifyTimer(par, r/s);
  dp->dropSpeed=s;
  dp->dropRate=r;
}

//////////////////////////////////////////////////////////////////////////
//
void lockBricks(s_vec<f::FArrInt> &cmap,
    s_vec<FArrBrick> &emap, Brick *z) {
  auto zs = z->getPosition();
  auto t= xrefTile(zs.x, zs.y);

  auto &cm= cmap[t.row];
  cm.set(t.col, 1);
  auto &em= emap[t.row];
  em.set(t.col, z);
}

//////////////////////////////////////////////////////////////////////////
//
void lock(not_null<a::Node*> node, Shape *shape) {
  auto co= CC_GNF(TileGrid, node, "collision");
  auto bs= CC_GNF(BlockGrid, node, "blocks");
  auto &cmap= co->tiles;
  auto &emap= bs->grid;

  F__LOOP(it, shape->bricks) {
    lockBricks(cmap, emap, *it);
  }

  postLock(node, cmap, emap);
}

//////////////////////////////////////////////////////////////////////////
//
void postLock(not_null<a::Node*> node,
    s_vec<f::FArrInt> &cmap,
    s_vec<FArrBrick> &emap) {

  auto flines = CC_GNF(FilledLines, node, "flines");
  // search bottom up until top.
  auto top= cmap.size();
  s_vec<int> rc;

  // 0 is the bottom wall
  for (auto r = 1; r < top; ++r) {
    if (testFilledRow(cmap, r)) {
      rc.push_back(r);
    }
  }

  if (rc.size() > 0) {
    pauseForClearance(node, true, 0.5f);
    flashFilled(emap, flines, rc);
  }
}

//////////////////////////////////////////////////////////////////////////
//
bool testFilledRow(s_vec<f::FArrInt> &cmap, int r) {
  auto &row= cmap[r];

  // negative if any holes in the row
  for (auto c=0; c < row.size(); ++c) {
    if (row.get(c) != 1) { return false; }
  }

  // entire row must be filled.
  return true;
}

//////////////////////////////////////////////////////////////////////////
//
void flashFilled(s_vec<FArrBrick> &emap,
    FilledLines *flines,
    const s_vec<int> &lines) {

  F__LOOP(it, lines) {
    auto &row= emap[*it];
    for (auto c=0; c < row.size(); ++c) {
      if (row.get(c) != nullptr) {
        row.get(c)->blink();
      }
    }
  }

  flines->lines=lines;
}

//////////////////////////////////////////////////////////////////////////
//
void pauseForClearance(not_null<a::Node*> node, bool b, float delay) {
  auto flines = CC_GNF(FilledLines, node, "flines");
  auto pu= CC_GNF(Pauser, node, "pauser");

  flines->lines.clear();
  pu->pauseToClear=b;
  pu->timer=nullptr;

  if (b) {
    pu->timer= cx::reifyTimer(MGML(), delay);
  }
}

//////////////////////////////////////////////////////////////////////////
//
bool moveDown(not_null<f::XLayer*> layer,
    s_vec<f::FArrInt> &cmap, Shape *shape) {

  auto new_y = shape->y - CC_CSV(c::Integer, "TILE");
  auto x = shape->x;
  auto bs = findBBox(cmap, shape->info.model, x, new_y, shape->info.rot);

  if (bs.size() > 0) {
    auto bricks= reifyBricks(layer,shape->info.png, bs);
    clearOldBricks(shape->bricks);
    shape->bricks.resize(bricks.size());
    s::copy(bricks.begin(), bricks.end(), shape->bricks.begin());
    shape->y= new_y;
    return true;
  } else {
    return false;
  }
}

//////////////////////////////////////////////////////////////////////////
//
bool shiftRight(not_null<f::XLayer*> layer,
    s_vec<f::FArrInt> &cmap, Shape *shape) {

  auto new_x= shape->x + CC_CSV(c::Integer, "TILE");
  auto y= shape->y;
  auto bs= findBBox(cmap, shape->info.model, new_x, y, shape->info.rot);

  if (bs.size() > 0) {
    auto bricks= reifyBricks(layer,shape->info.png, bs);
    clearOldBricks(shape->bricks);
    shape->bricks.resize(bricks.size());
    s::copy(bricks.begin(), bricks.end(), shape->bricks.begin());
    shape->x= new_x;
    return true;
  } else {
    return false;
  }
}

//////////////////////////////////////////////////////////////////////////
//
bool shiftLeft(not_null<f::XLayer*> layer,
    s_vec<f::FArrInt> &cmap, Shape *shape) {

  auto new_x= shape->x - CC_CSV(c::Integer, "TILE");
  auto y= shape->y;
  auto bs= findBBox(cmap, shape->info.model, new_x, y, shape->info.rot);

  if (bs.size() > 0) {
    auto bricks= reifyBricks(layer,shape->info.png, bs);
    clearOldBricks(shape->bricks);
    shape->bricks.resize(bricks.size());
    s::copy(bricks.begin(), bricks.end(), shape->bricks.begin());
    shape->x= new_x;
    return true;
  } else {
    return false;
  }
}

//////////////////////////////////////////////////////////////////////////
//
bool rotateRight(not_null<f::XLayer*> layer,
    s_vec<f::FArrInt> &cmap, Shape *shape) {

  auto nF = (shape->info.rot+1) % shape->info.model->size();
  auto bs= findBBox(cmap, shape->info.model,
                    shape->x, shape->y, nF);

  CCLOG("shape.rot = %d , dim = %d , rot-right , nF = %d",
      shape->info.rot,
      shape->info.model->getDim(), nF);

  if (bs.size() > 0) {
    auto bricks= reifyBricks(layer,shape->info.png, bs);
    clearOldBricks(shape->bricks);
    shape->bricks.resize(bricks.size());
    s::copy(bricks.begin(), bricks.end(), shape->bricks.begin());
    shape->info.rot= nF;
    return true;
  } else {
    return false;
  }
}

//////////////////////////////////////////////////////////////////////////
//
bool rotateLeft(not_null<f::XLayer*> layer,
    s_vec<f::FArrInt> &cmap, Shape *shape) {

  auto nF = (shape->info.rot-1) % shape->info.model->size();
  auto bs= findBBox(cmap, shape->info.model, shape->x, shape->y, nF);

  CCLOG("shape.rot = %d , dim = %d , rot-left , nF = %d",
      shape->info.rot,
      shape->info.model->getDim(), nF);

  if (bs.size() > 0) {
    auto bricks= reifyBricks(layer,shape->info.png, bs);
    clearOldBricks(shape->bricks);
    shape->bricks.resize(bricks.size());
    s::copy(bricks.begin(), bricks.end(), shape->bricks.begin());
    shape->info.rot= nF;
    return true;
  } else {
    return false;
  }
}


NS_END(tetris)


