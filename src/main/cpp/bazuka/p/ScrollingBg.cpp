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
#pragma once
//////////////////////////////////////////////////////////////////////////////

#include "core/XConfig.h"
#include "ScrollingBg.h"
#include "core/CCSX.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(bazuka)

//////////////////////////////////////////////////////////////////////////////
//
ScrollingBg* ScrollingBg::create(const sstr &name, float speed, float yPos) {
  auto ob = mc_new(ScrollingBg);
  ob->iniz(name, speed, yPos);
  ob->autorelease();
  return ob;
}

//////////////////////////////////////////////////////////////////////////////
//
bool ScrollingBg::iniz(const sstr &name, float speed, float yPos) {

  auto wz= cx::visRect();
  auto wb= cx::visBox();

  gameBg1 = cx::createSprite(name);
  gameBg1->setPosition(wb.cx, yPos);
  gameBg1->setAnchorPoint(cx::anchorB());
  gameBg1->setScaleX(1.01);
  addChild(gameBg1);

  gameBg2 = cx::createSprite(name);
  gameBg2->setPosition(wb.cx + wz.size.width, yPos);
  gameBg2->setAnchorPoint(cx::anchorL());
  gameBg2->setScaleX(1.01);
  addChild(gameBg2);

  this->speed = speed;
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void ScrollingBg::sync() {

  auto wz= cx::visRect();
  auto wb= cx::visBox();

    // scroll bg left or right
  if (gameBg1->getPosition().x < wb.left - HWZ(wz.size)) {
    gameBg1->setPosition(wb.right + HWZ(wz.size), gameBg1->getPosition().y);
  }

  if (gameBg2->getPosition().x < wb.left - HWZ(wz.size)) {
    gameBg2->setPosition(wb.right + HWZ(wz.size), gameBg2->getPosition().y);
  }

    auto bg1pos = gameBg1->getPosition();
    gameBg1->setPosition(bg1pos.x - speed, bg1pos.y);

    auto bg2pos = gameBg2->getPosition();
    gameBg2->setPosition(bg2pos.x - speed, bg2pos.y);
}


NS_END


