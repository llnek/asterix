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

#include "core/XConfig.h"
#include "Generate.h"

NS_BEGIN(tetris)


//////////////////////////////////////////////////////////////////////////
//
Generate::Generate(not_null<EFactory*> f, not_null<c::Dictionary*> d)

  : BaseSystem<EFactory>(f, d) {

}

//////////////////////////////////////////////////////////////////////////
//
void Generate::addToEngine(not_null<a::Engine*> e) {
  ArenaNode n;
  arena= e->getNodeList(n.typeId());
  nextShapeInfo= randNext();
  nextShape=nullptr;
}

//////////////////////////////////////////////////////////////////////////
//
bool Generate::update(float dt) {
  auto node = arena->head;

  if (MGMS()->isLive() &&
      NNP(node)) {

    auto sl = CC_GNF(ShapeShell, node, "shell");
    auto dp = CC_GNF(Dropper, node, "dropper");
    if (ENP(sl->shape)) {
      sl->shape = reifyNextShape(node, MGML());
      if (NNP(sl->shape)) {
        //show new next shape in preview window
        previewNextShape(node, MGML());
        //activate drop timer
        dp->dropSpeed= CC_CSV(c::Float, "DROPSPEED");
        initDropper(MGML(), dp);
      } else {
        return false;
      }
    }
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////
//
void Generate::reifyNextShape(a::Node *node, f::XLayer *layer) {
  auto co= CC_GNF(TileGrid, node, "collision");
  auto bks= CC_GNF(BlockGrid, node, "grid");
  auto gbox= CC_GNF(GridBox, node, "gbox");
  auto& tiles = co->tiles;
  auto tile= MGMS()->TILE;
  auto wz= cx::visRect();
  auto shape= new Shape(gbox->box.left + 5 * tile,
      gbox->box.top - tile,
      nextShapeInfo);
  shape= reifyShape(layer, tiles, shape);
  if (ENP(shape)) {
    CCLOG("game over.  you lose.");
    bks->grid.clear();
    MGMS()->sendMsg("/hud/end");
  }

  return shape;
}

//////////////////////////////////////////////////////////////////////////
//
void Generate::previewNextShape(a::Node *node, f::XLayer *layer) {
  auto info = randNext();
  gbox= node.gbox,
  cw = ccsx.center(),
  wb = ccsx.vbox(),
  shape,
  sz = (1 + info.model.dim) * csts.TILE,
  x = cw.x + (wb.right - cw.x) * 0.5,
  y = wb.top * 0.7;

  x -= sz * 0.5;
  y += sz * 0.5;

  utils.disposeShape(this.nextShape);
  this.nextShape= null;
  shape= new cobjs.Shape(x,y, info);
  this.nextShapeInfo= info;
  this.nextShape= utils.previewShape(layer, shape);
},
/**
   * @method randNext
   * @private
   */
  randNext() {
    const n= sjs.rand( cobjs.Shapes.length),
    proto= cobjs.Shapes[n];

    return {
      png: sjs.rand(csts.BLOCK_COLORS) + 1,
      rot: sjs.rand(proto.layout.length),
      model: proto
    };
  }

}, {
/**
 * @memberof module:s/generate~Generate
 * @property {Number} Priority
 */
Priority: xcfg.ftypes.Generate
});


NS_END(tetris)


