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
#include "XLib.h"
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////////
/*
c::Menu* addMenuIcon(not_null<XLayer*> layer,
    not_null<c::MenuItem*> b) {

  auto menu= reifyRefType<cocos2d::Menu>();
  menu->addChild(b);
  layer->addItem(menu);
  return menu;
}
*/
//////////////////////////////////////////////////////////////////////////////
//
/*
c::Menu* addReplayIcon(not_null<XLayer*> layer, not_null<c::MenuItem*> c,
    const c::Vec2& where ) {

  auto tile= CstVal<c::Integer>("TILE")->getValue();
  auto menu= ReifyRefType<cocos2d::Menu>();
  auto hh = cx::getScaledHeight(c);
  auto hw = cx::getScaledWidth(c);
  auto wz= cx::visBox();
  float y;

  menu->addChild(c);

  if (where.y == cx::anchorB().y) {
    y = wz.bottom + tile  + hh;
  } else {
    y = wz.top - tile  - hh;
  }

  menu->setPosition(wz.left + tile + hw, y);
  layer->addItem(menu);

  return menu;
}
*/

//////////////////////////////////////////////////////////////////////////////
//
c::Menu* addAudioIcons(not_null<XLayer*> layer,
    not_null<c::MenuItem*> off,
    not_null<c::MenuItem*> on,
    const c::Vec2& anchor, const c::Vec2& pos) {

  auto cb = [](c::Ref* r) {
    auto t = SCAST(c::MenuItemToggle*, r);
    auto b= t->getSelectedIndex() == 0;
    XCFG()->toggleAudio( b);
  };

  c::Vector<c::MenuItem*> items;
  items.pushBack(on);
  items.pushBack(off);

  auto audio = c::MenuItemToggle::createWithCallback(cb, items);
  audio->setSelectedIndex( XCFG()->hasAudio() ? 0 : 1);
  audio->setAnchorPoint(anchor);

  // need null to end var-args
  auto menu= c::Menu::create(audio, nullptr);
  menu->setPosition(pos);
  layer->addItem(menu);
  return menu;
}










NS_END(fusii)


