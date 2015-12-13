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
Shape* reifyShape(not_null<f::XLayer*> layer, cmap, Shape *shape) {
  auto bbox= findBBox(cmap, shape->model, shape->x, shape->y, shape->rot);
  Shape *rc=nullptr;
  auto bricks;

  if (bbox.size() > 0) {
    bricks= reifyBricks(layer, shape.png, shape->x, shape->y, bbox);
    clearOldBricks(shape->bricks);
    s::copy(bricks.begin(), bricks.end(), shape->bricks.begin());
    rc=shape;
  }

  return rc;
}

//////////////////////////////////////////////////////////////////////////
//
float topLine(not_null<a::Node*> node) {
  auto gbox= CC_GNF(GridBox, node, "gbox");
  auto bx= gbox->box;
  return floor((bx.top - bx.bottom) / MGMS()->TILE);
}

//////////////////////////////////////////////////////////////////////////
//
Shape* previewShape(not_null<f::XLayer*> layer, Shape *shape) {
  auto bbox= findBBox([],shape->model,
                          shape->x, shape->y, shape->rot, true);
  if (bbox.size() > 0) {
    auto bricks= reifyBricks(layer,shape.png, shape->x, shape->y, bbox);
    clearOldBricks(shape->bricks);
    s::copy(bricks.begin(), bricks.end(), shape->bricks.begin());
  }
  return shape;
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
void clearOldBricks(s_vec<>& bs) {
  F__LOOP(it, bs) {
    auto& z= *it;
    z->dispose()
  }
  bs.clear();
}

//////////////////////////////////////////////////////////////////////////
//
const s_vec<Brick*> reifyBricks(not_null<f::XLayer*> layer,
    const sstr& png,
    float x,
    float y,
    const s_vec<c::Vec2>& bs) {

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
findBBox(cmap, BModel *model,
    float px, float py, int rID, bool skipCollide) {

  //auto form= model->getLayout(rID);
  s_vec<c::Vec2> bs;
  float x,y;

  for (auto r=0; r < model->getDim(); ++r) {
    y = py - MGMS()->TILE * r;
    for (auto c=0; c < model->getDim(); ++c) {
      x = px + MGMS()->TILE * c;
      if (model->test(rID,r,c)) {
      //if (form[r][c] == 1) {
        if (!skipCollide &&
            maybeCollide(cmap, x, y,
                              x + MGMS()->TILE,
                              y - MGMS()->TILE)) {
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
bool maybeCollide(cmap, float tl_x, float tl_y, float br_x, float br_y) {
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
const f::Cell2D xrefTile(float x, float y) {
  // find center, instead of top left
  auto co = MGMS()->TILE * 0.5f;
  auto y -= co;
  auto x += co;

  // realign actual x,y
  x -= MGMS()->CBOX.left - MGMS()->FENCE;

  return f::Cell2D(
           floor(y / MGMS()->TILE)
           floor(x / MGMS()->TILE)
      );
}

//////////////////////////////////////////////////////////////////////////
//
void initDropper(not_null<c::Node*> par, Dropper *dp) {
  dp->timer = cx::createTimer(par, dp->dropRate / dp->dropSpeed);
}

//////////////////////////////////////////////////////////////////////////
//
void setDropper(not_null<c::Node*> par, Dropper *dp, float r, float s) {
  dp->timer = cx::createTimer(par, r/s);
  dp->dropSpeed=s;
  dp->dropRate=r;
}

//////////////////////////////////////////////////////////////////////////
//
void lockBricks(cmap, emap, Brick *z) {
  auto zs = z->getPosition();
  auto t= xrefTile(zs.x, zs.y);

  cmap[t.row][t.col] = 1;
  emap[t.row][t.col] = z;
}

//////////////////////////////////////////////////////////////////////////
//
void lock(not_null<a::Node*> node, Shape *shape) {
  auto co= CC_GNF(TileGrid, node, "collision");
  auto bs= CC_GNF(BlockGrid, node, "blocks");
  auto& cmap= co->tiles;
  auto& emap= bs->grid;

  F__LOOP(it, shape->bricks) {
    lockBricks(cmap, emap, *it);
  }

  postLock(node, cmap, emap);
}

//////////////////////////////////////////////////////////////////////////
//
void postLock(not_null<a::Node*> node, cmap, emap) {

  auto flines = CC_GNF(FilledLines, node, "flines");
  // search bottom up until top.
  auto top= cmap.length;
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
bool testFilledRow(cmap, int r) {
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
void flashFilled(emap,
    FilledLines *flines,
    const s_vec<int>& lines) {

  F__LOOP(it, lines) {
    auto row= emap[*it];
    for (auto c=0; c < row.size(); ++c) {
      if (row[c]) {
        row[c]->blink();
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

  if (b) {
    pu->timer= cx::createTimer(MGML(), delay);
  } else {
    pu->timer=nullptr;
  }
}

//////////////////////////////////////////////////////////////////////////
//
bool moveDown(not_null<f::XLayer*> layer, cmap, Shape *shape) {
  auto new_y = shape->y - MGMS()->TILE;
  auto x = shape->x;
  auto bs = findBBox(cmap, shape->model, x, new_y, shape->rot);

  if (bs.size() > 0) {
    auto bricks= reifyBricks(layer,shape->png, x, new_y, bs);
    clearOldBricks(shape->bricks);
    s::copy(bricks.begin(), bricks.end(), shape->bricks.begin());
    shape->y= new_y;
    return true;
  } else {
    return false;
  }
}

//////////////////////////////////////////////////////////////////////////
//
bool shiftRight(not_null<f::XLayer*> layer, cmap, Shape *shape) {
  auto new_x= shape->x + csts.TILE;
  auto y= shape->y;
  auto bs= findBBox(cmap, shape->model, new_x, y, shape->rot);

  if (bs.size() > 0) {
    auto bricks= reifyBricks(layer,shape->png, new_x, y, bs);
    clearOldBricks(shape->bricks);
    s::copy(bricks.begin(), bricks.end(), shape->bricks.begin());
    shape->x= new_x;
    return true;
  } else {
    return false;
  }
}

//////////////////////////////////////////////////////////////////////////
//
bool shiftLeft(not_null<f::XLayer*> layer, cmap, Shape *shape) {
  auto new_x= shape->x - MGMS()->TILE;
  auto y= shape->y;
  auto bs= findBBox(cmap, shape->model, new_x, y, shape->rot);

  if (bs.size() > 0) {
    auto bricks= reifyBricks(layer,shape->png, new_x, y, bs);
    clearOldBricks(shape->bricks);
    s::copy(bricks.begin(), bricks.end(), shape->bricks);
    shape->x= new_x;
    return true;
  } else {
    return false;
  }
}

//////////////////////////////////////////////////////////////////////////
//
bool rotateRight(not_null<f::XLayer*> layer,cmap, Shape *shape) {
  auto nF = (shape->rot+1) % shape->model->getLayouts();
  auto bs= findBBox(cmap, shape->model,
                    shape->x, shape->y, nF);

  CCLOG("shape.rot = %d , dim = %d , rot-right , nF = %d",
      shape->rot,
      shape->model->getDim(), nF);

  if (bs.size() > 0) {
    auto bricks= reifyBricks(layer,shape->png, shape->x, shape->y, bs);
    clearOldBricks(shape.bricks);
    s::copy(bricks.begin(), bricks.end(), shape->bricks);
    shape->rot= nF;
    return true;
  } else {
    return false;
  }
}

//////////////////////////////////////////////////////////////////////////
//
bool rotateLeft(not_null<f::XLayer*> layer, cmap, Shape *shape) {
  auto nF = (shape->rot-1) % shape->model->getLayouts();
  auto bs= findBBox(cmap, shape->model, shape->x, shape->y, nF);

  CCLOG("shape.rot = %d , dim = %d , rot-left , nF = %d",
      shape->rot,
      shape->model->getDim(), nF);

  if (bs.size() > 0) {
    auto bricks= reifyBricks(layer,shape->png, shape->x, shape->y, bs);
    clearOldBricks(shape.bricks);
    s::copy(bricks.begin(), bricks.end(), shape->bricks.begin());
    shape->rot= nF;
    return true;
  } else {
    return false;
  }
}


NS_END(tetris)


