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
  auto cbx,
  auto ch3= cz.height / 3;
  auto cw3= cz.width / 3;
  //x= cw.x + (wb.right - cw.x) * 0.5,
  auto x= wb.right * 0.75;
  auto y= wb.top * 0.25;

  sp->setPosition(x,y);
  MGML()->addAtlasItem("game-pics", sp);

  cbx= ccsx.bbox4(sp);

  //calc hotspots for touch & mouse
  // rotate left right
  hsps.rr= { left: cbx.left + cw3,
        top: cbx.top,
        right: cbx.right - cw3,
        bottom: cbx.top - ch3 };

  hsps.rl= { left: cbx.left + cw3,
        top: cbx.top- 2* ch3,
        right: cbx.right - cw3,
        bottom: cbx.bottom };

  // shifting left, right
  hsps.sl= { left: cbx.left,
        top: cbx.top - ch3,
        right: cbx.left + cw3,
        bottom: cbx.top - 2 * ch3 };

  hsps.sr= { left: cbx.left + 2 * cw3,
        top: cbx.top - ch3,
          right: cbx.right,
          bottom: cbx.top - 2 * ch3 };

    // fast drop down
    hsps.cd= { left: cbx.left + cw3,
          top: cbx.top - ch3,
          right: cbx.right - cw3,
          bottom: cbx.top - 2 * ch3 };
  },
  /**
   * Draw horizontal wall.
   * @method xh
   * @private
   */
  xh(fz, lf_bdy, rt_bdy, ypos) {
    let cw = ccsx.center(),
    wb= ccsx.vbox(),
    wz= ccsx.vrect(),
    hfzw = fz.width * 0.5,
    f, x, y;

    y = ypos;//wb.bottom + fz.height * 0.5;
    x = lf_bdy;
    while (x < rt_bdy){ //}cw.x) {
      f=ccsx.createSprite('gray.png');
      f.setPosition(x, y);
      sh.main.addAtlasItem('game-pics',f);
      x += fz.width;
    }
  },
  /**
   * Draw vertical wall.
   * @method xv
   * @private
   */
  xv(fz, x) {
    let cw = ccsx.center(),
    wb= ccsx.vbox(),
    wz= ccsx.vrect(),
    f, y;

    y= wb.bottom;
    y += fz.height * 0.5;
    while (y < wb.top) {
      f=ccsx.createSprite('gray.png');
      f.setPosition(x, y);
      sh.main.addAtlasItem('game-pics',f);
      y += fz.height;
    }
  },
  /**
   * @method onceOnly_2
   * @private
   */
  onceOnly_2(node, fz, bz, box) {
    const tiles= this.fakeTileMap(bz, box);

    node.blocks.grid = this.initBlockMap(tiles);
    node.collision.tiles = tiles;
    node.gbox.box= box;

    csts.FENCE= fz.width;
    csts.TILE= bz.width;
    csts.CBOX= box;

    sjs.loggr.info("collision tiles and blocks init'ed.");
    sjs.loggr.info("tile size = " + csts.TILE);
  },
  /**
   * @method initBlockMap
   * @private
   */
  initBlockMap(tiles) {
    const grid=[];

    for (let r= 0; r < tiles.length; ++r) {
      let rc= sjs.makeArray(tiles[r].length, undef);
      grid.push(rc);
    }

    return grid;
  },
  /**
   * Create our own collision map using cells.
   * @method fakeTileMap
   * @private
   */
  fakeTileMap(bz, box) {

    let hlen = Math.floor( (box.top - box.bottom) / bz.height),
    wlen = Math.floor((box.right - box.left) / bz.width),
    map=[],
    rc;

    // use 1 to indicate wall
    for (let r = 0; r <= hlen; ++r) {
      if (r===0) {
        rc = sjs.makeArray(wlen+2, 1);
      } else {
        rc = sjs.makeArray(wlen+2, 0);
        rc[0]=1;
        rc[rc.length-1]=1;
      }
      map.push(rc);
    }
    return map;
  }

NS_END(tetris)

