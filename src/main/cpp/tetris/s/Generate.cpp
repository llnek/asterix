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
#include "core/CCSX.h"
#include "Generate.h"
#include "s/utils.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(tetris)

//////////////////////////////////////////////////////////////////////////////
//
BEGIN_NS_UNAMED()

LineModel m1;
BoxModel m2;
StModel m3;
ElModel m4;
NubModel m5;
StxModel m6;
ElxModel m7;

s_arr<BModel*,7> ListOfModels = {
  &m1,
  &m2,
  &m3,
  &m4,
  &m5,
  &m6,
  &m7
};

END_NS_UNAMED()

//////////////////////////////////////////////////////////////////////////
//
Generate::Generate(not_null<EFactory*> f, not_null<c::Dictionary*> d)

  : XSystem<EFactory>(f, d) {

}

//////////////////////////////////////////////////////////////////////////
//
void Generate::addToEngine(not_null<a::Engine*> e) {
  ArenaNode n;
  arena= e->getNodeList(n.typeId());
  nextShape=nullptr;
  nextShapeInfo= randNextInfo();
}

//////////////////////////////////////////////////////////////////////////
//
bool Generate::update(float dt) {
  auto n = arena->head;

  if (MGMS()->isLive()) {

    auto sl = CC_GNF(ShapeShell, n, "shell");
    auto dp = CC_GNF(Dropper, n, "dropper");
    if (ENP(sl->shape)) {
      sl->shape = reifyNextShape(n, MGML());
      if (NNP(sl->shape)) {
        //show new next shape in preview window
        previewNextShape(n, MGML());
        //activate drop timer
        dp->dropSpeed= CC_CSV(c::Float, "DROPSPEED");
        initDropper( dp);
      } else {
        return false;
      }
    }
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////
//
Shape * Generate::reifyNextShape(not_null<a::Node*> node, not_null<f::XLayer*> layer) {
  auto co= CC_GNF(TileGrid, node, "collision");
  auto bks= CC_GNF(BlockGrid, node, "grid");
  auto gbox= CC_GNF(GridBox, node, "gbox");
  auto tile= CC_CSV(c::Integer, "TILE");
  auto &tiles = co->tiles;
  auto wz= cx::visRect();
  auto x = gbox->box.left + 5 * tile;
    auto y = gbox->box.top - tile;
  auto shape= reifyShape(layer, tiles,
      x,y,
      nextShapeInfo);
  if (ENP(shape)) {
    CCLOG("game over.  you lose.");
    bks->grid.clear();
    SENDMSG("/hud/end");
  }

  return shape;
}

//////////////////////////////////////////////////////////////////////////
//
void Generate::previewNextShape(not_null<a::Node*> node, not_null<f::XLayer*> layer) {
  auto gbox= CC_GNF(GridBox, node, "gbox");
  auto tile = CC_CSV(c::Integer, "TILE");
  auto info = randNextInfo();
  auto cw = cx::center();
  auto wb = cx::visBox();

  auto sz = (1 + info.model->getDim()) * tile;
  auto x = cw.x + (wb.right - cw.x) * 0.5f;
  auto y = wb.top * 0.7f;

  x -= sz * 0.5f;
  y += sz * 0.5f;

  disposeShape(nextShape);
  SNPTR(nextShape)

  nextShape= previewShape(layer, x, y, info);
  nextShapeInfo= info;
}

//////////////////////////////////////////////////////////////////////////////
//
const ShapeInfo Generate::randNextInfo() {
  auto bc= CC_CSV(c::Integer, "BLOCK_COLORS");
  auto n= cx::randInt(ListOfModels.size());
  auto proto= ListOfModels[n];
  return ShapeInfo(proto,
      cx::randInt(proto->size()),
      s::to_string(cx::randInt(bc) + 1) + ".png" );
}


NS_END(tetris)


