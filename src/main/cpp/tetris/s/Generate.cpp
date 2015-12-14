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
  nextShapeInfo= randNextInfo();
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
Shape * Generate::reifyNextShape(a::Node *node, f::XLayer *layer) {
  auto co= CC_GNF(TileGrid, node, "collision");
  auto bks= CC_GNF(BlockGrid, node, "grid");
  auto gbox= CC_GNF(GridBox, node, "gbox");
  auto& tiles = co->tiles;
  auto tile= MGMS()->TILE;
  auto wz= cx::visRect();
  auto shape= new Shape(*nextShapeInfo);
  shape->x = gbox->box.left + 5 * tile;
  shape->y = gbox->box.top - tile,
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
  auto gbox= CC_GNF(GridBox, node, "gbox");
  auto tile = MGMS()->TILE;
  auto info = randNextInfo();
  auto cw = cx::center();
  auto wb = cx::visBox();

  auto sz = (1 + info->model->getDim()) * tile;
  auto x = cw.x + (wb.right - cw.x) * 0.5f;
  auto y = wb.top * 0.7f;

  x -= sz * 0.5f;
  y += sz * 0.5f;

  disposeShape(nextShape);
  SNPTR(nextShape)

  shape= new Shape(*info);
  shape->x = x;
  shape->y = y;
  nextShapeInfo= info;
  nextShape= previewShape(layer, shape);
}

//////////////////////////////////////////////////////////////////////////////
//
Shape * Generate::randNextInfo() {
  auto bc= CC_CSV(c::Integer, "BLOCK_COLORS");
  auto n= cx::randInt(ListOfModels.size());
  auto proto= ListOfModels[n];
  return mc_new_3(Shape, proto,
      cx::randInt(proto->getLayouts()),
      s::to_string(cx::randInt(bc) + 1) + ".png" );
}


NS_END(tetris)


