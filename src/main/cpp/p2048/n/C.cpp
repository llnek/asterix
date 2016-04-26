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

#include "core/XConfig.h"
#include "core/COMP.h"
#include "core/CCSX.h"
#include "C.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(p2048)

//////////////////////////////////////////////////////////////////////////////
//
owner<Card*> Card::create(int value, const CCT_SZ &sz, const CCT_PT &pos) {
  auto z= mc_new1(Card,value);
  z->inix(sz, pos);
  z->autorelease();
  return z;
}

//////////////////////////////////////////////////////////////////////////////
//
void Card::runNewNumberAction() {
    auto a= c::ScaleBy::create(0.05,0.99);
  this->runAction( c::Sequence::create( a, a->reverse(), CC_NIL));
}

//////////////////////////////////////////////////////////////////////////////
//
void Card::setNumber(int num) {

  _value= num;

  if (num > 0) {
    _numLabel->setString(FTOS(num));
  } else {
    _numLabel->setString("");
  }

  if (num >= 0) {
    _numLabel->setSystemFontSize(100);
  }

  if (num >= 16) {
    _numLabel->setSystemFontSize(90);
  }

  if (num >= 128) {
    _numLabel->setSystemFontSize(60);
  }

  if (num >= 1024) {
    _numLabel->setSystemFontSize(40);
  }

  if(num == 0){
    setColor(c::Color3B(200,190,180));
  }

  if (num== 2) {
    setColor(c::Color3B(240,230,220));
  }

  if (num == 4) {
    setColor(c::Color3B(240,220,200));
  }

  if (num == 8) {
    setColor(c::Color3B(240,180,120));
  }

  if (num == 16) {
    setColor(c::Color3B(240,140,90));
  }

  if (num == 32) {
    setColor(c::Color3B(240,120,90));
  }

  if (num == 64) {
    setColor(c::Color3B(240,90,60));
  }

  if (num == 128) {
    setColor(c::Color3B(240,90,60));
  }

  if (num == 256) {
    setColor(c::Color3B(240,200,70));
  }

  if (num == 512) {
    setColor(c::Color3B(240,200,70));
  }

  if (num == 1024) {
    setColor(c::Color3B(0,130,0));
  }

  if (num == 2048) {
    setColor(c::Color3B(0,130,0));
  }

}

//////////////////////////////////////////////////////////////////////////////
//
bool Card::inix(const CCT_SZ &sz, const CCT_PT &pos) {

  if (! c::LayerColor::initWithColor(
        c::Color4B(200,190,180,255)
        )) {
  return false; }

  setContentSize(CCT_SZ(sz.width-15,sz.height-15));
  setContentSize(sz);
  CC_POS1(this, pos);

  sstr vs= "";
  if (_value > 0) {
    vs= FTOS(_value);
  }
  _numLabel = c::Label::createWithSystemFont(
      vs,
      "Consolas", XCFG()->scaleFont(100));

  CC_POS2(_numLabel, HWZ(sz), HHZ(sz));
  _numLabel->setTag(8);
  addChild(_numLabel);
}


NS_END



