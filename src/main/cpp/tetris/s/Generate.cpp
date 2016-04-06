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
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Generate.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(tetris)

//////////////////////////////////////////////////////////////////////////////
//
BEGIN_NS_UNAMED

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

END_NS_UNAMED

//////////////////////////////////////////////////////////////////////////
//
void Generate::preamble() {
  _arena = _engine->getNodes("n/BlockGrid")[0];
  _nextShapeInfo= randNextInfo();
  _nextShape=CC_NIL;
}

//////////////////////////////////////////////////////////////////////////
//
bool Generate::update(float dt) {
  auto rc=true;
  if (MGMS()->isLive()) {
    auto sl = CC_GEC(ShapeShell, _arena, "n/ShapeShell");
    auto dp = CC_GEC(Dropper, _arena, "n/Dropper");
    auto cfg= MGMS()->getLCfg()->getValue();
    auto sp = sl->shape;

    if (E_NIL(sp)) {
      sp = reifyNextShape();
      if (sp) {
        previewNextShape();
        sl->shape= sp;
        //activate drop timer
        dp->dropSpeed= JS_FLOAT(cfg["speed"]);
        dp->dropRate= JS_FLOAT(cfg["nrate"]);
        initDropper(dp);
      } else {
        rc= false;
      }
    }
  }

  return rc;
}

//////////////////////////////////////////////////////////////////////////
//
owner<Shape*> Generate::reifyNextShape() {
  auto bks= CC_GEC(BlockGrid, _arena, "n/BlockGrid");
  auto ss= CC_GEC(GVars, _arena, "n/GVars");
  auto tile= CC_CSV(c::Float,"TILE");
  auto x = ss->cbox.left + 5 * tile;
  auto y = ss->cbox.top - tile;
  auto shape= reifyShape(ss->cbox,bks->grid, x,y, _nextShapeInfo);
  if (E_NIL(shape)) {
    CCLOG("game over.  you lose.");
    bks->grid.clear();
    SENDMSG("/hud/end");
  }

  return shape;
}

//////////////////////////////////////////////////////////////////////////
//
void Generate::previewNextShape() {
  auto ss= CC_GEC(GVars, _arena, "n/GVars");
  auto tile = CC_CSV(c::Float, "TILE");
  auto info = randNextInfo();
  auto wb = cx::visBox();

  auto sz = (1 + info.model->dim()) * tile;
  auto x = wb.cx + (wb.right-wb.cx) * 0.5;
  auto y = wb.top * 0.7;

  x -= HTV(sz);
  y += HTV(sz);

  disposeShape(_nextShape);
  S__NIL(_nextShape)

  _nextShape= previewShape(ss->cbox,info, x, y);
  _nextShapeInfo= info;
}

//////////////////////////////////////////////////////////////////////////////
//
const ShapeInfo Generate::randNextInfo() {
  auto bc= CC_CSV(c::Integer, "BLOCK+COLORS");
  auto n= cx::randInt(ListOfModels.size());
  auto proto= ListOfModels[n];
  return ShapeInfo(proto,
      cx::randInt(proto->size()),
      FTOS(cx::randInt(bc) + 1) + ".png" );
}


NS_END


