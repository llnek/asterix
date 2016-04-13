// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Kenneth Leung. All rights reserved.

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "HUD.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(hockey)

//////////////////////////////////////////////////////////////////////////
//
void HUDLayer::decoUI() {

  auto b= cx::reifyBmfLabel("dft", "000");
  auto wb= cx::visBox();

  CC_POS2(b, wb.right-60, wb.cy + 80);
  XCFG()->scaleNode(b,36);
  b->setRotation(90);
  addItem(b);
  _slabels[2]=b;
  _scores[2]=0;

  b= cx::reifyBmfLabel("dft","000");
  CC_POS2(b, wb.right-60, wb.cy - 80);
  XCFG()->scaleNode(b,36);
  b->setRotation(90);
  addItem(b);
  _slabels[1]=b;
  _scores[1]=0;
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::updateScore(int pnum, int value) {

  assert(pnum > 0 && pnum < _scores.size());
  auto sum = _scores[pnum] + value;

  _slabels[pnum]->setString(FTOS(sum));
  _scores[pnum] = sum;
}




NS_END



