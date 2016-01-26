// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Ken Leung. All rights reserved.

#include "x2d/GameScene.h"
#include "lib.h"

NS_BEGIN(tetris)

//////////////////////////////////////////////////////////////////////////
//
static void reifyBricks(not_null<f::XLayer*> layer,
    const s_vec<c::Vec2> &bs,
    const sstr &png,
    s_vec<Brick*> &bricks) {

  F__LOOP(it, bs) {
    auto obj= Brick::reify( *it, png );
    bricks.push_back(obj);
    layer->addAtlasItem("game-pics", obj->sprite);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
static owner<Shape*> mkShape(not_null<f::XLayer*> layer,
    float x, float y,
    const ShapeInfo &info,
    const s_vec<c::Vec2> &bbox) {

  Shape *rc=nullptr;

  if (bbox.size() > 0) {
    rc= mc_new1(Shape, info);
    rc->x=x;
    rc->y=y;
    reifyBricks(layer, bbox, info.png, rc->bricks);
  }

  return rc;
}

//////////////////////////////////////////////////////////////////////////
//
static void lockBrick(s_vec<FArrBrick> &emap, Brick *z) {

  auto zs = z->sprite->getPosition();
  auto &e= emap[0];
  auto hlen= emap.size();
  auto last= e.size() - 1;
  auto t= xrefTile(zs);

  // must be inside the 2 walls
  assert(t.col > 0 && t.col < last);
  assert(t.row > 0);

  auto &em= emap[t.row];
  auto c= em.get(t.col);
  assert(ENP(c));
  em.set(t.col, z);
}

//////////////////////////////////////////////////////////////////////////
//
static void postLock(not_null<a::Node*> node,
    s_vec<FArrBrick> &emap) {

  auto flines = CC_GNF(FilledLines, node, "flines");
  // search bottom up until top.
  auto top= emap.size();
  s_vec<int> rc;

  // 0 is the bottom wall
  for (auto r = 1; r < top; ++r) {
    if (testFilledRow(emap, r)) {
      rc.push_back(r);
    }
  }

  if (rc.size() > 0) {
    pauseForClearance(node, true, 0.5f);
    flashFilled(emap, flines, rc);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
owner<Shape*> reifyShape(not_null<f::XLayer*> layer,
    s_vec<FArrBrick> &emap,
    float x, float y, const ShapeInfo &info) {

  return mkShape(layer, x, y, info,
      findBBox(emap, info.model, x, y, info.rot));
}

//////////////////////////////////////////////////////////////////////////
//
owner<Shape*> previewShape(not_null<f::XLayer*> layer,
    float x, float y,
    const ShapeInfo &info) {

  s_vec<FArrBrick> dummy;
  return mkShape(layer, x, y, info,
      findBBox(dummy, info.model, x, y, info.rot, true));
}

//////////////////////////////////////////////////////////////////////////
//
int topLine(not_null<a::Node*> node) {
  auto bks= CC_GNF(BlockGrid, node, "blocks");
  return (int) bks->grid.size();
}

//////////////////////////////////////////////////////////////////////////
//
void disposeShape(Shape *shape) {
  if (NNP(shape))
    clearOldBricks(shape->bricks);
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
const s_vec<c::Vec2> findBBox(s_vec<FArrBrick> &emap,
    BModel *model,
    float px, float py,
    int rID, bool skipCollide) {

  auto tile = CC_CSV(c::Float, "TILE");
  auto dim= model->getDim();
  s_vec<c::Vec2> bs;
  float x,y;

  for (auto r=0; r < dim; ++r) {
    y = py - r * tile;
    for (auto c=0; c < dim; ++c) {
      x = px + c * tile;
      if (model->test(rID,r,c)) {
        if (!skipCollide &&
            maybeCollide(emap, x, y)) {
          return s_vec<c::Vec2> {};
        }
        bs.push_back(c::ccp(x,y));
      }
    }
  }

  CCLOG("found valid bboxes = %d", bs.size());
  return bs;
}

//////////////////////////////////////////////////////////////////////////
//
bool maybeCollide(s_vec<FArrBrick> &emap,
    float tl_x, float tl_y) {

  auto tile= xrefTile(tl_x , tl_y);
  auto &e= emap[0];
  auto hlen= emap.size();
  auto last= e.size() -1;

  if (tile.row >= 0 && tile.row < hlen &&
      tile.col >= 0 && tile.col < last)
  {}
  else
  { return true;  } // bad index

  auto &em= emap[tile.row];
  auto c= em.get(tile.col);
  auto rc=false;

  if (NNP(c) )  {
    CCLOG("collide! tile = %d, %d", tile.row , tile.col);
    rc= true;
  }

  return rc;
}

//////////////////////////////////////////////////////////////////////////////
//
const f::Cell2D xrefTile(const c::Vec2 &pos) {
  return xrefTile(pos.x, pos.y);
}

//////////////////////////////////////////////////////////////////////////
//
const f::Cell2D xrefTile(float x, float y) {
  // find center, instead of top left
  auto tile = CC_CSV(c::Float, "TILE") ;
  auto co = tile * 0.5f;
  auto bx= CC_CSV(f::Box4R, "CBOX");
  y -= co;
  x += co;

  auto rc= f::Cell2D(
    (int) floor((y - bx.bottom) / tile),
    (int) floor((x - bx.left) / tile));

  CCLOG("tile = %d, %d", rc.row , rc.col);
  return rc;
}

//////////////////////////////////////////////////////////////////////////
//
void initDropper(Dropper *dp) {
  // drop at normal rate
  setDropper(MGML(), dp, dp->dropRate, dp->dropSpeed);
}

//////////////////////////////////////////////////////////////////////////
//
void setDropper(not_null<c::Node*> par,
    Dropper *dp, float r, float s) {

  dp->timer = cx::reifyTimer(par, s/r);
}

//////////////////////////////////////////////////////////////////////////
//
void lock(not_null<a::Node*> node, Shape *shape) {
  auto bs= CC_GNF(BlockGrid, node, "blocks");
  auto &emap= bs->grid;

  F__LOOP(it, shape->bricks) {
    lockBrick(emap, *it);
  }

  postLock(node, emap);
}

//////////////////////////////////////////////////////////////////////////
//
bool testFilledRow(s_vec<FArrBrick> &emap, int r) {
  auto &row= emap[r];
  auto last= row.size() -1;

  // negative if any holes in the row
  for (auto c=1; c < last; ++c) {
    if (row.get(c) == nullptr) { return false; }
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
    auto last= row.size()-1;
    for (auto c=1; c < last; ++c) {
      auto b= row.get(c);
      if (NNP(b)) { b->blink(); }
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
    s_vec<FArrBrick> &emap,
    Shape *shape) {

  auto tile= CC_CSV(c::Float, "TILE");
  auto new_y = shape->y - tile;
  auto x = shape->x;
  auto rc=false;
  auto bs = findBBox(emap, shape->info.model, x, new_y, shape->info.rot);

  if (bs.size() > 0) {
    clearOldBricks(shape->bricks);
    shape->y= new_y;
    reifyBricks(layer, bs, shape->info.png, shape->bricks);
    rc= true;
  }

  return rc;
}

//////////////////////////////////////////////////////////////////////////
//
bool shiftRight(not_null<f::XLayer*> layer,
    s_vec<FArrBrick> &emap,
    Shape *shape) {

  auto tile = CC_CSV(c::Float, "TILE");
  auto new_x= shape->x + tile;
  auto y= shape->y;
  auto rc=false;
  auto bs= findBBox(emap, shape->info.model, new_x, y, shape->info.rot);

  if (bs.size() > 0) {
    clearOldBricks(shape->bricks);
    shape->x= new_x;
    reifyBricks(layer, bs, shape->info.png, shape->bricks);
    rc= true;
  }

  return rc;
}

//////////////////////////////////////////////////////////////////////////
//
bool shiftLeft(not_null<f::XLayer*> layer,
    s_vec<FArrBrick> &emap, Shape *shape) {

  auto tile= CC_CSV(c::Float, "TILE");
  auto new_x= shape->x - tile;
  auto y= shape->y;
  auto rc=false;
  auto bs= findBBox(emap, shape->info.model, new_x, y, shape->info.rot);

  if (bs.size() > 0) {
    clearOldBricks(shape->bricks);
    shape->x= new_x;
    reifyBricks(layer, bs, shape->info.png, shape->bricks);
    rc= true;
  }

  return rc;
}

//////////////////////////////////////////////////////////////////////////
//
bool rotateRight(not_null<f::XLayer*> layer,
    s_vec<FArrBrick> &emap, Shape *shape) {

  auto nF = f::modulo( (shape->info.rot+1),
      shape->info.model->size());
  auto rc=false;
  auto bs= findBBox(emap, shape->info.model,
                    shape->x, shape->y, nF);

  CCLOG("shape.rot = %d , dim = %d , rot-right , nF = %d",
      shape->info.rot,
      shape->info.model->getDim(), nF);

  if (bs.size() > 0) {
    clearOldBricks(shape->bricks);
    shape->info.rot= nF;
    reifyBricks(layer, bs, shape->info.png, shape->bricks);
    rc= true;
  }

  return rc;
}

//////////////////////////////////////////////////////////////////////////
//
bool rotateLeft(not_null<f::XLayer*> layer,
    s_vec<FArrBrick> &emap, Shape *shape) {

  auto nF = f::modulo( (shape->info.rot-1) ,
      shape->info.model->size());
  auto rc=false;
  auto bs= findBBox(emap, shape->info.model, shape->x, shape->y, nF);

  CCLOG("shape.rot = %d , dim = %d , rot-left , nF = %d",
      shape->info.rot,
      shape->info.model->getDim(), nF);

  if (bs.size() > 0) {
    clearOldBricks(shape->bricks);
    shape->info.rot= nF;
    reifyBricks(layer, bs, shape->info.png, shape->bricks);
    rc= true;
  }

  return rc;
}


NS_END(tetris)


