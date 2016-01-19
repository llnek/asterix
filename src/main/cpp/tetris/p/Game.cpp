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

#include "x2d/GameLayer.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "n/lib.h"
#include "Game.h"
#include "MMenu.h"
#include "HUD.h"
#include "s/GEngine.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(tetris)

BEGIN_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GLayer : public f::GameLayer {

  virtual bool onTouchBegan(c::Touch*, c::Event*) {
    return true;
  }
  virtual void onTouchEnded(c::Touch*, c::Event*);
  virtual void onMouseUp(c::Event*);
  virtual void postReify();

  virtual HUDLayer* getHUD() {
    return (HUDLayer*) getLayer(3);
  }

  STATIC_REIFY_LAYER(GLayer)
  MDECL_DECORATE()
  MDECL_GET_IID(2)

  void onGUI(const c::Vec2&);
  void showMenu();
  void endGame();
  void deco();
  void reset();

  // ----
  void xh(const c::Size& , float, float, float);
  void onceOnly();
  void doCtrl();
  void xv(const c::Size& , float);
  void onceOnly_2(const c::Size&,
      const c::Size& , const f::Box4&);

  const s_vec<FArrBrick>
    initBlockMap(s_vec<f::FArrInt> &tiles);

  const s_vec<f::FArrInt>
    fakeTileMap(const c::Size&, const f::Box4&);


  DECL_PTR(a::NodeList, arena)
};

//////////////////////////////////////////////////////////////////////////
//
void GLayer::onceOnly() {
  auto fld_w = CC_CSV(c::Integer, "FIELD_W");
  auto fz= cx::calcSize("gray.png");
  auto bz= cx::calcSize("0.png");
  auto cw = cx::center();
  auto wb= cx::visBox();
  auto wz= cx::visRect();

  auto lf_boundary= cw.x - fld_w * bz.width - fz.width;
  auto hfzh= HHZ(fz);
  auto hfzw= HWZ(fz);

  // attempt to draw the walls
  this->xh(fz, lf_boundary, cw.x, wb.bottom + hfzh);
  this->xv(fz, lf_boundary);
  this->xv(fz, cw.x);
  //this.xh(fz, cw.x + fz.width, wb.right + fz.width, cw.y);

  onceOnly_2(fz, bz, f::Box4(
    wb.top,
    cw.x - hfzw,
    wb.bottom + fz.height,
    lf_boundary + hfzw
  ));

  doCtrl();
}

//////////////////////////////////////////////////////////////////////////
//
void GLayer::doCtrl() {
  auto cpad= CC_GNLF(CtrlPad, arenaNode, "cpad");
  auto& hsps= cpad->hotspots;
  auto cw = cx::center();
  auto wb= cx::visBox();
  auto wz= cx::visRect();
  //sp= ccsx.createSprite('shadedLight09.png'),
  auto sp= cx::reifySprite("shadedDark09.png");
  auto cz= sp->getContentSize();
  auto ch3= cz.height / 3.0f;
  auto cw3= cz.width / 3.0f;
  //x= cw.x + (wb.right - cw.x) * 0.5,
  auto x= wb.right * 0.75f;
  auto y= wb.top * 0.25f;

  sp->setPosition(x,y);
  MGML()->addAtlasItem("game-pics", sp);

  auto cbx= cx::bbox4(sp);
  //calc hotspots for touch & mouse
  // rotate left right
  hsps["rr"] = f::Box4(
      cbx.top,
      cbx.right - cw3,
      cbx.top - ch3,
      cbx.left + cw3);

  hsps["rl"] = f::Box4(
      cbx.top- 2* ch3,
      cbx.right - cw3,
      cbx.bottom,
      cbx.left + cw3);

  // shifting left, right
  hsps["sl"] = f::Box4(
      cbx.top - ch3,
      cbx.left + cw3,
      cbx.top - 2 * ch3,
      cbx.left );

  hsps["sr"] = f::Box4(
      cbx.top - ch3,
      cbx.right,
      cbx.top - 2 * ch3,
      cbx.left + 2 * cw3);

  // fast drop down
  hsps["cd"] = f::Box4(
      cbx.top - ch3,
      cbx.right - cw3,
      cbx.top - 2 * ch3,
      cbx.left + cw3 );
}

//////////////////////////////////////////////////////////////////////////
// draw horizontal
void GLayer::xh(const c::Size &fz, float lf_bdy, float rt_bdy, float ypos) {
  auto cw = cx::center();
  auto wb= cx::visBox();
  auto wz= cx::visRect();
  auto hfzw = HWZ(fz);
  auto y = ypos;//wb.bottom + fz.height * 0.5;
  auto x = lf_bdy;

  while (x < rt_bdy) { //}cw.x) {
    auto f= cx::reifySprite("gray.png");
    f->setPosition(x, y);
    MGML()->addAtlasItem("game-pics",f);
    x += fz.width;
  }
}

//////////////////////////////////////////////////////////////////////////
// Draw vertical wall
void GLayer::xv(const c::Size &fz, float x) {
  auto wz= cx::visRect();
  auto cw = cx::center();
  auto wb= cx::visBox();
  auto y= wb.bottom;
  y += HHZ(fz);

  while (y < wb.top) {
    auto f= cx::reifySprite("gray.png");
    f->setPosition(x, y);
    MGML()->addAtlasItem("game-pics",f);
    y += fz.height;
  }
}

//////////////////////////////////////////////////////////////////////////
//
void GLayer::onceOnly_2(const c::Size &fz,
    const c::Size &bz,
    const f::Box4 &box) {

  auto blocks= CC_GNLF(BlockGrid, arenaNode, "blocks");
  auto cs= CC_GNLF(TileGrid, arenaNode, "collision");
  auto gbox= CC_GNLF(GridBox, arenaNode, "gbox");
  auto tiles= fakeTileMap(bz, box);
  auto grids = initBlockMap(tiles);

  blocks->grid.resize(grids.size(), FArrBrick());
  cs->tiles.resize(tiles.size(), f::FArrInt());
  s::copy(grids.begin(), grids.end(), blocks->grid.begin());
  s::copy(tiles.begin(), tiles.end(), cs->tiles.begin());
  gbox->box= box;

  XCFG()->resetCst("FENCE", CC_INT( (int) floor(fz.width)));
  XCFG()->resetCst("TILE", CC_INT( (int) floor(bz.width)));
  XCFG()->resetCst("CBOX", f::Box4R::create(box));

  CCLOG("collision tiles and blocks init'ed.");
  CCLOG("fence size = %d", CC_CSV(c::Integer,"FENCE"));
  CCLOG("tile size = %d", CC_CSV(c::Integer,"TILE"));
}

//////////////////////////////////////////////////////////////////////////
//
const s_vec<FArrBrick>
GLayer::initBlockMap(s_vec<f::FArrInt> &tiles) {
  s_vec<FArrBrick> grid;

  F__LOOP(it, tiles) {
    auto &e = *it;
    FArrBrick rc(e.size());
    rc.fill(nullptr);
    grid.push_back(rc);
  }

  return grid;
}

//////////////////////////////////////////////////////////////////////////
// Create our own collision map using cells.
const s_vec<f::FArrInt>
GLayer::fakeTileMap(const c::Size &bz, const f::Box4 &box) {

  auto hlen = (int) floor((box.top - box.bottom) / bz.height);
  auto wlen = (int) floor((box.right - box.left) / bz.width);
  s_vec<f::FArrInt> map;

  wlen += 2; // 2 side walls
  // use 1 to indicate wall
  for (auto r = 0; r <= hlen; ++r) {
    f::FArrInt rc(wlen);
    if (r == 0) {
      rc.fill(1);
    } else {
      rc.fill(0);
      rc.setFirst(1);
      rc.setLast(1);
    }
    map.push_back(rc);
  }

  return map;
}



//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchEnded(c::Touch *t, c::Event*) {
  auto pos= t->getLocationInView();
  pos= CC_DTOR()->convertToGL(pos);
  onGUI(pos);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseUp(c::Event *e) {
  auto evt= (c::EventMouse*)e;
  auto pos= evt->getLocationInView();
  auto b= evt->getMouseButton();
  if (b == MOUSE_BUTTON_LEFT) {
    onGUI(pos);
  }
}

//////////////////////////////////////////////////////////////////////////
//
void GLayer::onGUI(const c::Vec2 &pos) {
  auto motion= CC_GNLF(Gesture, arenaNode, "motion");
  auto cpad= CC_GNLF(CtrlPad, arenaNode, "cpad");
  auto &hsps= cpad->hotspots;

  if (cx::pointInBox(hsps["rr"], pos)) {
    motion->rotr=true;
  }

  if (cx::pointInBox(hsps["rl"], pos)) {
    motion->rotl=true;
  }

  if (cx::pointInBox(hsps["sr"], pos)) {
    motion->right=true;
  }

  if (cx::pointInBox(hsps["sl"], pos)) {
    motion->left=true;
  }

  if (cx::pointInBox(hsps["cd"], pos)) {
    motion->down=true;
  }

}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::postReify() {
  arena= engine->getNodeList(ArenaNode().typeId());
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::decorate() {

  this->engine = mc_new(GEngine);

  centerImage("game.bg");

  regoAtlas("game-pics");
  regoAtlas("lang-pics");

  getHUD()->reset();
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::showMenu() {
  auto f= [=]() { CC_DTOR()->popScene(); };
  auto m= MMenu::reify(mc_new1(MCX,f));
  CC_DTOR()->pushScene(m);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::endGame() {
  surcease();
  getHUD()->endGame();
}

END_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic &topic, void *msg) {
  auto y = (GLayer*) getLayer(2);

  if ("/hud/end" == topic) {
    y->endGame();
  }

  if ("/hud/score/update" == topic) {
    auto json= (j::json*) msg;
    auto v= JS_INT(json->operator[]("score"));
    y->getHUD()->updateScore(v);
  }

  if ("/hud/showmenu" == topic) {
    y->showMenu();
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Game::decorate() {
  HUDLayer::reify(this, 3);
  GLayer::reify(this, 2);
  play();
}

NS_END(tetris)



