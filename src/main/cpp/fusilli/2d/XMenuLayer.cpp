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

#include "2d/CCMenuItem.h"
#include "2d/CCMenu.h"
#include "XMenuLayer.h"
NS_BEGIN(fusilli)



//////////////////////////////////////////////////////////////////////////////
//
c::Menu* XMenuLayer::MkBackQuit(c::MenuItem* b,
    c::MenuItem* q, bool vert) {

  auto sz= b->getContentSize();
  auto padding = 10;
  auto menu= c::Menu::create();
  menu->addChild(b);
  menu->addChild(q);

  if (!vert) {
    menu->alignItemsHorizontallyWithPadding(padding);
  } else {
    menu->alignItemsVerticallyWithPadding(padding);
  }
  return menu;
}

//////////////////////////////////////////////////////////////////////////////
//
void XMenuLayer::MkAudio() {
  AddAudioIcon();
}

//////////////////////////////////////////////////////////////////////////////
//
XMenuLayer::~XMenuLayer() {

}

//////////////////////////////////////////////////////////////////////////////
//
XMenuLayer::XMenuLayer() {

}


NS_END(fusilli)

