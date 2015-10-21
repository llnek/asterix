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

#include "utils.h"

NS_ALIAS(cx, fusii::ccsx)
NS_USING(fusii)
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////
// Calculate position of each individual cells in the grid,
// so that we can detect when a user clicks on the cell
s::vector<Box4> MapGridPos(int gsz, float scale) {
  // memorize the co-ordinates of each cell on the board, so
  // we know which cell the user has clicked on
  auto sp = cx::ReifySprite("z.png");
  auto z = sp->getContentSize();
  auto csz = cx::ScaleSize(z, scale);
  auto cells= gsz * gsz;
  auto ro= 8/72 * scale;
  auto cw = cx::Center();
  auto gh = csz.height * ro;
  auto gw = csz.width * ro;
  auto zh= gsz * csz.height + (gsz-1) * gh;
  auto zw= gsz * csz.width + (gsz-1) * gw;

  auto x0 = cw.x - zw * 0.5;
  auto y0 = cw.y + zh * 0.5;
  auto x1= x0;
  auto y1=y0;
  int x2,y2;
  s::vector<Box4> gridMap;

  for (int r=0; r < gsz; ++r) {
    for (int c= 0; c < gsz; ++c) {
      y2 = y1 - csz.height;
      x2 = x1 + csz.width;
      //gridMap[r * gsz + c] = f::Box4(y1, x2, y2, x1);
      gridMap.push_back( Box4(y1, x2, y2, x1));
    }
    y1 = y2 - gh;
    x1 = x0;
  }

  return gridMap;
}

//////////////////////////////////////////////////////////////////////////
//
const stdstr PkFlip(const stdstr& img, bool flip) {
  if (flip) {
    return img + ".i.png";
  } else {
    return img + ".png";
  }
}

//////////////////////////////////////////////////////////////////////////
//
const stdstr& XrefImg(int value) {
  auto x= CC_CSV(c::Integer,"CV_X");
  auto o= CC_CSV(c::Integer,"CV_O");
  auto z= CC_CSV(c::Integer,"CV_Z");

  if (x==value) { return "x"; }
  if (o==value) { return "o"; }
  if (z==value) { return "z"; }

  throw "bad value!";
}

//////////////////////////////////////////////////////////////////////////
//
c::Sprite* DrawSymbol(not_null<GridView*> view,
    float x, float y,
    int value, bool flip) {

  auto frame = PkFlip(XrefImg(value), flip);
  auto s1= cx::ReifySprite(frame);

  s1->setAnchorPoint(cx::AnchorC());
  s1->setPosition(x,y);

  view->layer->AddAtlasItem("game-pics", s1);
  return s1;
}


NS_END(tttoe)

