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
owner<ScrollingBg*> ScrollingBg::create(const sstr &png, float speed, float yPos) {
  auto ob = f::reifyRefType<ScrollingBg>();
  ob->set(png,speed,yPos);
  return ob;
}

//////////////////////////////////////////////////////////////////////////////
//
void ScrollingBg::set(const sstr &png, float speed, float yPos) {
  auto wz= cx::visSize();
  auto wb= cx::visBox();

  _head = cx::reifySprite(png);
  _tail = cx::reifySprite(png);
  XCFG()->fit(_head);
  XCFG()->fit(_tail);

  _head->setAnchorPoint(cx::anchorB());
  _head->setScaleX(1.01);
  CC_POS2(_head, wb.cx, yPos);
  addChild(_head);

  CC_POS2(_tail, wb.cx + wz.width, yPos);
  _tail->setAnchorPoint(cx::anchorL());
  _tail->setScaleX(1.01);
  addChild(_tail);

  this->_speed = speed;
}

//////////////////////////////////////////////////////////////////////////////
//
void ScrollingBg::sync() {

  auto wz= cx::visSize();
  auto wb= cx::visBox();

    // scroll bg left or right

  if (_head->getPositionX() < wb.left - HWZ(wz)) {
    CC_POS2(_head, wb.right + HWZ(wz), _head->getPositionY());
  }

  if (_tail->getPositionX() < wb.left - HWZ(wz)) {
    CC_POS2(_tail, wb.right + HWZ(wz), _head->getPositionY());
  }

  auto pos1 = _head->getPosition();
  auto pos2 = _tail->getPosition();

  CC_POS2(_head, pos1.x - _speed, pos1.y);
  CC_POS2(_tail, pos2.x - _speed, pos2.y);

}




NS_END


