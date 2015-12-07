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
#include "utils.h"
#include "CObjs.h"

NS_ALIAS(cx, fusii::ccsx)
NS_USING(fusii)
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
// Calculate position of each individual cells in the grid,
// so that we can detect when a user clicks on the cell
const s::array<Box4,GD_SZ> mapGridPos(float scale) {
  // memorize the co-ordinates of each cell on the board, so
  // we know which cell the user has clicked on
  auto csz = cx::scaleSize(cx::calcSize("z.png"), scale);
  auto ro= 8.0f/72.0f * scale;
  auto gh = csz.height * ro;
  auto gw = csz.width * ro;
  auto zh= csz.height * BD_SZ + gh * (BD_SZ-1);
  auto zw= csz.width * BD_SZ + gw * (BD_SZ-1);

  s::array<Box4, GD_SZ> boxes;
  auto cw = cx::center();
  int x2, y2;

  auto x0 = cw.x - zw * 0.5f;
  auto y0 = cw.y + zh * 0.5f;
  auto x1= x0;
  auto y1= y0;

  for (int r=0; r < BD_SZ; ++r) {
    for (int c= 0; c < BD_SZ; ++c) {
      auto& bx= boxes[r*BD_SZ+c];
      y2 = y1 - csz.height;
      x2 = x1 + csz.width;
      bx.top= y1;
      bx.right= x2;
      bx.bottom= y2;
      bx.left= x1;
      x1= x2 + gw;
    }
    y1 = y2 - gh;
    x1 = x0;
  }

  return boxes;
}

//////////////////////////////////////////////////////////////////////////////
//
const s::vector<ArrDim> mapGoalSpace() {

  s::vector<ArrDim> goals;
  ArrDim dx;
  ArrDim dy;

  for (int r=0; r < BD_SZ; ++r) {

    ArrDim h;
    ArrDim v;

    for (int c=0; c < BD_SZ; ++c) {
      h[c] = r * BD_SZ + c;
      v[c] = c * BD_SZ + r;
    }

    goals.push_back(h);
    goals.push_back(v);

    dx[r] = r * BD_SZ + r;
    dy[r] = (BD_SZ - r - 1) * BD_SZ + r;
  }

  goals.push_back(dx);
  goals.push_back(dy);

  return goals;
}

//////////////////////////////////////////////////////////////////////////
//
const sstr pkFlip(const sstr& img, bool flip) {
  if (flip) {
    return img + ".i.png";
  } else {
    return img + ".png";
  }
}

//////////////////////////////////////////////////////////////////////////
//
const sstr xrefImg(int value) {
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
c::Sprite* drawSymbol(not_null<a::Component*> c,
    float x, float y,
    int value, bool flip) {

  auto frame = pkFlip(xrefImg(value), flip);
  auto view= (PlayView*) c.get();
  auto s1= cx::reifySprite(frame);

  s1->setAnchorPoint(cx::anchorC());
  s1->setPosition(x,y);

  view->layer->addAtlasItem("game-pics", s1);
  return s1;
}

//////////////////////////////////////////////////////////////////////////////
//
void prepareSeedData(f::GMode m) {

    j::json seed = j::json( {
    {"ppids", j::json {} },
    {"pnum", 1 }
    });

  if (m == f::GMode::TWO) {
    seed["ppids"][ XCFG()->getL10NStr("p1") ] =
    j::json::array_t {
    1, XCFG()->getL10NStr("player1") };
    seed["ppids"][ XCFG()->getL10NStr("p2") ] =
    j::json::array_t { 2, XCFG()->getL10NStr("player2") };
  }

  if (m == f::GMode::ONE) {
    seed["ppids"][ XCFG()->getL10NStr("cpu") ] =
          j::json::array_t { 2, XCFG()->getL10NStr("computer") };
    seed["ppids"][ XCFG()->getL10NStr("p1") ] =
          j::json::array_t { 1, XCFG()->getL10NStr("player1") };
  }

  if (m == f::GMode::NET) {
    seed["pnum"] = 0;
  }

  XCFG()->setSeedData(seed);
}



NS_END(tttoe)










