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

#include "n/GNodes.h"
#include "Stage.h"

NS_BEGIN(tetris)

//////////////////////////////////////////////////////////////////////////
//
Stage::Stage(not_null<EFactory*> f, not_null<c::Dictionary*> d)

  : BaseSystem<EFactory>(f, d) {

}

//////////////////////////////////////////////////////////////////////////
//
void Stage::addToEngine(not_null<a::Engine*> e) {
  ArenaNode n;
  arena= e->getNodeList(n.typeId());
}

//////////////////////////////////////////////////////////////////////////
//
bool Stage::update(float dt) {
  auto node = arena->head;
  if (MGMS()->isLive() &&
      NNP(node)) {
    if (! inited) {
      onceOnly(node);
    }
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////
//
void Stage::onceOnly(a::Node* node) {
  auto fld_w = CC_CSV(c::Integer, "FIELD_W");
  auto fz= cx::calcSize("gray.png");
  auto bz= cx::calcSize("0.png");
  auto cw = cx::center();
  auto wb= cx::visBox();
  auto wz= cx::visRect();
  auto lf_boundary= cw.x - fld_w * bz.width - fz.width;
  auto hfzh= HHZ(fz);
  auto hfzw= HWZ(fz);

  // attempt to draw the walls
  this->xh(fz, lf_boundary, cw.x, wb.bottom + hfzh);
  this->xv(fz, lf_boundary);
  this->xv(fz, cw.x);
  //this.xh(fz, cw.x + fz.width, wb.right + fz.width, cw.y);

  onceOnly_2(node, fz, bz, f::Box4(
    wb.top,
    cw.x - hfzw,
    wb.bottom + fz.height,
    lf_boundary + hfzw
  ));

  doCtrl(node);
}

//////////////////////////////////////////////////////////////////////////
//
void Stage::doCtrl(a::Node* node) {
  auto cpad= CC_GNF(CtrlPad, node, "cpad");
  auto hsps= cpad->hotspots;
  auto cw = cx::center();
  auto wb= cx::visBox();
  auto wz= cx::visRect();
  //sp= ccsx.createSprite('shadedLight09.png'),
  auto sp= cx::reifySprite("shadedDark09.png");
  auto cz= sp->getContentSize();
  auto cbx= cx::bbox4(sp);
  auto ch3= cz.height / 3;
  auto cw3= cz.width / 3;
  //x= cw.x + (wb.right - cw.x) * 0.5,
  auto x= wb.right * 0.75;
  auto y= wb.top * 0.25;

  sp->setPosition(x,y);
  MGML()->addAtlasItem("game-pics", sp);

  //calc hotspots for touch & mouse
  // rotate left right
  hsps.rr= f::Box4(
      cbx.top,
      cbx.right - cw3,
      cbx.top - ch3,
      cbx.left + cw3);

  hsps.rl= f::Box4(
      cbx.top- 2* ch3,
      cbx.right - cw3,
      cbx.bottom,
      cbx.left + cw3);

  // shifting left, right
  hsps.sl= f::Box4(
      cbx.top - ch3,
      cbx.left + cw3,
      cbx.top - 2 * ch3,
      cbx.left );

  hsps.sr= f::Box4(
      cbx.top - ch3,
      cbx.right,
      cbx.top - 2 * ch3,
      cbx.left + 2 * cw3);

  // fast drop down
  hsps.cd= f::Box4(
      cbx.top - ch3,
      cbx.right - cw3,
      cbx.top - 2 * ch3,
      cbx.left + cw3 );
}

//////////////////////////////////////////////////////////////////////////
// draw horizontal
void Stage::xh(const c::Size& fz, float lf_bdy, float rt_bdy, float ypos) {
  auto cw = cx::center();
  auto wb= cx::visBox();
  auto wz= cx::visRect();
  auto hfzw = HWZ(fz);
  auto y = ypos;//wb.bottom + fz.height * 0.5;
  auto x = lf_bdy;

  while (x < rt_bdy) { //}cw.x) {
    auto f= cx::reifySprite("gray.png");
    f->setPosition(x, y);
    MGML()->addAtlasItem("game-pics",f);
    x += fz.width;
  }
}

//////////////////////////////////////////////////////////////////////////
// Draw vertical wall
void Stage::xv(const c::Size& fz, float x) {
  auto cw = cx::center();
  auto wb= cx::visBox();
  auto wz= cx::visRect();
  auto y= wb.bottom;
  y += HHZ(fz);

  while (y < wb.top) {
    auto f= cx::reifySprite("gray.png");
    f->setPosition(x, y);
    MGML()->addAtlasItem("game-pics",f);
    y += fz.height;
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Stage::onceOnly_2(a::Node* node, const c::Size& fz,
    const c::Size& bz,
    const f::Box& box) {

  auto blocks= CC_GNF(BlockGrid,node, "blocks");
  auto cs= CC_GNF(TileGrid,node, "collision");
  auto gbox= CC_GNF(GridBox, node, "gbox");
  auto tiles= fakeTileMap(bz, box);

  blocks->grid = initBlockMap(tiles);
  cs->tiles = tiles;
  gbox->box= box;

  MGMS()->FENCE= (int) floor(fz.width);
  MGMS()->TILE= (int) floor(bz.width);
  MGMS()->CBOX= box;

  CCLOG("collision tiles and blocks init'ed.");
  CCLOG("tile size = %d", MGMS()->TILE);
}

//////////////////////////////////////////////////////////////////////////
//
const s_vec<>
Stage::initBlockMap(const s_vec<f::FArrInt*>& tiles) {
  s_vec<> grid;

  F__LOOP(it, tiles) {
    auto& e = *it;
    auto rc= new f::FArray<>(e->size());
    grid.push_back(rc);
  }

  return grid;
}

//////////////////////////////////////////////////////////////////////////
// Create our own collision map using cells.
const s_vec<f::FArrInt*>
Stage::fakeTileMap(const c::Size& bz, const f::Box4& box) {

  auto hlen = (int) floor((box.top - box.bottom) / bz.height);
  auto wlen = (int) floor((box.right - box.left) / bz.width);
  s_vec<f::FArrInt*> map;

  // use 1 to indicate wall
  wlen += 2; // 2 side walls
  for (auto r = 0; r <= hlen; ++r) {
    auto rc= new f::FArrInt(wlen);
    if (r == 0) {
      rc->fill(1);
    } else {
      rc->fill(0);
      rc->setFirst(1);
      rc->setLast(1);
    }
    map.push_back(rc);
  }

  return map;
}


NS_END(tetris)

