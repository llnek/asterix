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
#include "MMenu.h"
#include "HUD.h"
#include "End.h"
#include "Game.h"
#include "n/lib.h"
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
    return
      (HUDLayer*) getSceneX()->getLayer(3);
  }

  STATIC_REIFY_LAYER(GLayer)
  MDECL_DECORATE()
  MDECL_GET_IID(2)

  void onGUI(const c::Vec2&);
  void showMenu();
  void endGame();
  void deco();

  // ----

  void initBlockMap(BlockGrid*, const c::Size&, const f::Box4&);
  void xh(const c::Size& , float, float, float);
  void xv(const c::Size& , float);
  void onceOnly();
  void doCtrl();
  void onceOnly_2(const c::Size&,
      const c::Size& , const f::Box4&);

  DECL_PTR(a::NodeList, arena)
};

//////////////////////////////////////////////////////////////////////////
//
void GLayer::onceOnly() {
  auto fld_w = CC_CSV(c::Integer, "FIELD_W");
  auto fz= cx::calcSize("gray.png");
  auto bz= cx::calcSize("0.png");
  auto wb= cx::visBox();

  CCLOG("gray.png, width= %d, height = %d",
      (int)fz.width, (int)fz.height);
  CCLOG("0.png, width= %d, height = %d",
      (int)bz.width, (int)bz.height);

  auto lf_boundary= wb.cx - fld_w * bz.width - fz.width;
  auto hfzh= HHZ(fz);
  auto hfzw= HWZ(fz);

  // attempt to draw the walls
  this->xh(fz, lf_boundary, wb.cx, wb.bottom + hfzh);
  this->xv(fz, lf_boundary);
  this->xv(fz, wb.cx);
  //this.xh(fz, cw.x + fz.width, wb.right + fz.width, cw.y);

  onceOnly_2(fz, bz, f::Box4(
    wb.top,
    wb.cx - hfzw,
    wb.bottom + fz.height,
    lf_boundary + hfzw
  ));

  doCtrl();
}

//////////////////////////////////////////////////////////////////////////
//
void GLayer::doCtrl() {
  auto cpad= CC_GNLF(CtrlPad, arena, "cpad");
  auto& hsps= cpad->hotspots;
  auto wz= cx::visRect();
  auto wb= cx::visBox();
  //sp= ccsx.createSprite('shadedLight09.png'),
  auto sp= cx::reifySprite("shadedDark09.png");
  auto cz= sp->getContentSize();
  auto ch3= cz.height / 3.0f;
  auto cw3= cz.width / 3.0f;
  //x= cw.x + (wb.right - cw.x) * 0.5,
  auto x= wb.right * 0.75f;
  auto y= wb.top * 0.25f;

  sp->setPosition(x,y);
  addAtlasItem("game-pics", sp);

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
  auto hfzw = HWZ(fz);
  auto x = lf_bdy;

  while (x < rt_bdy) {
    auto f= cx::reifySprite("gray.png");
    f->setPosition(x, ypos);
    addAtlasItem("game-pics",f);
    x += fz.width;
  }
}

//////////////////////////////////////////////////////////////////////////
// Draw vertical wall
void GLayer::xv(const c::Size &fz, float x) {
  auto wz= cx::visRect();
  auto wb= cx::visBox();
  auto y= wb.bottom;
  y += HHZ(fz);

  while (y < wb.top) {
    auto f= cx::reifySprite("gray.png");
    f->setPosition(x, y);
    addAtlasItem("game-pics",f);
    y += fz.height;
  }
}

//////////////////////////////////////////////////////////////////////////
//
void GLayer::onceOnly_2(const c::Size &fz,
    const c::Size &bz,
    const f::Box4 &box) {

  auto blocks= CC_GNLF(BlockGrid, arena, "blocks");
  auto gbox= CC_GNLF(GridBox, arena, "gbox");

  initBlockMap(blocks, bz, box);
  gbox->box= box;

  XCFG()->resetCst("FENCE", CC_INT( (int) floor(fz.width)));
  XCFG()->resetCst("TILE", CC_INT( (int) floor(bz.width)));
  XCFG()->resetCst("CBOX", f::Box4R::create(box));

  CCLOG("collision tiles and blocks init'ed");
  CCLOG("fence size = %d", CC_CSV(c::Integer,"FENCE"));
  CCLOG("tile size = %d", CC_CSV(c::Integer,"TILE"));
}

//////////////////////////////////////////////////////////////////////////
// Create our own collision map using cells.
void GLayer::initBlockMap(BlockGrid *bks,
    const c::Size &bz, const f::Box4 &box) {

  auto hlen = (int) floor((box.top - box.bottom) / bz.height);
  auto wlen = (int) floor((box.right - box.left) / bz.width);

  assert(wlen == CC_CSV(c::Integer, "FIELD_W"));

  // use true to indicate wall tiles
  wlen += 2; // 2 side walls
  for (auto r = 0; r <= hlen; ++r) {
    FArrBrick rc(wlen);
    if (r == 0) {
      rc.map([](Brick* cur) -> Brick* {
            return mc_new1(Brick,true);
          });
    } else {
      rc.fill(nullptr);
    }
    if (r > 0) {
      rc.setFirst(mc_new1(Brick,true));
      rc.setLast(mc_new1(Brick,true));
    }
    bks->grid.push_back(rc);
  }
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
  auto motion= CC_GNLF(Gesture, arena, "motion");
  auto cpad= CC_GNLF(CtrlPad, arena, "cpad");
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
  onceOnly();
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::decorate() {

  this->engine = mc_new(GEngine);

  centerImage("game.bg");

  regoAtlas("game-pics");
  regoAtlas("lang-pics");
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::showMenu() {
  auto f= []() { CC_DTOR()->popScene(); };
  auto m= MMenu::reify(mc_new1(MCX,f));
  CC_DTOR()->pushScene(m);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::endGame() {
  surcease();
  getHUD()->endGame();
  ELayer::reify(getSceneX(), 99999);
}

END_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic &topic, void *msg) {
  auto y = (GLayer*) getLayer(2);

  if ("/hud/score/update" == topic) {
    auto json= (j::json*) msg;
    auto v= JS_INT(json->operator[]("score"));
    y->getHUD()->updateScore(v);
  }

  if ("/hud/end" == topic) {
    y->endGame();
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



