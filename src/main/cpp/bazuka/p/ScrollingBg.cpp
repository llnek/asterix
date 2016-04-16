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
#pragma once
//////////////////////////////////////////////////////////////////////////////

#include "core/XConfig.h"
#include "core/COMP.h"
#include "core/CCSX.h"
#include "ScrollingBg.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(bazuka)

//////////////////////////////////////////////////////////////////////////////
//
ScrollingBg* ScrollingBg::create(const sstr &png, float speed, float yPos) {
  auto ob = f::reifyRefType<ScrollingBg>();
  ob->set(png,speed,yPos);
  return ob;
}

//////////////////////////////////////////////////////////////////////////////
//
void ScrollingBg::set(const sstr &png, float speed, float yPos) {
  auto wz= cx::visSize();
  auto wb= cx::visBox();

  head = cx::reifySprite(png);
  tail = cx::reifySprite(png);

  CC_POS2(head, wb.cx, yPos);
  head->setAnchorPoint(cx::anchorB());
  head->setScaleX(1.01);
  addChild(head);

  CC_POS2(tail, wb.cx + wz.size.width,yPos);
  tail->setAnchorPoint(cx::anchorL());
  tail->setScaleX(1.01);
  addChild(tail);

  this->speed = speed;
}

//////////////////////////////////////////////////////////////////////////////
//
void ScrollingBg::sync() {

  auto wz= cx::visSize();
  auto wb= cx::visBox();

    // scroll bg left or right

  if (head->getPositionX() < wb.left - HWZ(wz)) {
    CC_POS2(head, wb.right + HWZ(wz), head->getPositionY());
  }

  if (tail->getPositionX() < wb.left - HWZ(wz)) {
    CC_POS2(tail, wb.right + HWZ(wz), head->getPositionY());
  }

  auto pos1 = head->getPosition();
  auto pos2 = tail->getPosition();

  head->setPosition(pos1.x - speed, pos1.y);
  tail->setPosition(pos2.x - speed, pos2.y);

}

NS_END


