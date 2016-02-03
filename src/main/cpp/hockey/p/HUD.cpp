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
#include "HUD.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(hockey)

//////////////////////////////////////////////////////////////////////////
//
void HUDLayer::decorate() {

  auto wb= cx::visBox();
  auto b= cx::reifyBmfLabel(wb.right-60, wb.top * 0.5f + 80, "OCR", 60, "888");
  b->setRotation(90);
  addItem(b);
  slabels[2]=b;
  scores[2]=0;

  b= cx::reifyBmfLabel(wb.right-60, wb.top * 0.5f - 80, "OCR", 60, "888");
  b->setRotation(90);
  addItem(b);
  slabels[1]=b;
  scores[1]=0;
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::updateScore(int pnum, int value) {

  assert(pnum > 0 && pnum < scores.size());
  auto sum = scores[pnum] + value;

  slabels[pnum]->setString(s::to_string(sum));
  scores[pnum] = sum;
}




NS_END



