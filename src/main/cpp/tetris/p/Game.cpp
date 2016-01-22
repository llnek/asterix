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

  const f::Box4 initBlockMap(BlockGrid*, const c::Size&);
  void onceOnly();
  void doCtrl();

  DECL_PTR(a::NodeList, arena)
};

//////////////////////////////////////////////////////////////////////////
//
void GLayer::onceOnly() {
  auto blocks= CC_GNLF(BlockGrid, arena, "blocks");
  auto gbox= CC_GNLF(GridBox, arena, "gbox");
  auto fld_w = CC_CSV(c::Integer, "FIELD_W");
  auto bz= cx::calcSize("gray.png");
  auto box= initBlockMap(blocks, bz);

  XCFG()->resetCst("CBOX", f::Box4R::create(box));
  XCFG()->resetCst("TILE", CC_FLOAT( bz.width));
  gbox->box= box;

  CCLOG("brick: w= %d, h= %d", (int)bz.width, (int)bz.height);
  CCLOG("tile size = %f", CC_CSV(c::Float,"TILE"));
  CCLOG("gridbox: t=%d, r=%d, b=%d, l=%d",
      (int)box.top,(int)box.right,
      (int)box.bottom,(int)box.left);
  CCLOG("collision tiles and blocks init'ed");

  doCtrl();
}

//////////////////////////////////////////////////////////////////////////
//
void GLayer::doCtrl() {
  auto cpad= CC_GNLF(CtrlPad, arena, "cpad");
  auto& hsps= cpad->hotspots;
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
// Create our own collision map using cells.
const f::Box4
GLayer::initBlockMap(BlockGrid *bks, const c::Size &bz) {

  auto wlen= CC_CSV(c::Integer, "FIELD_W") + 2; // 2 side walls
  auto wz= MGMS()->getEnclosureRect();
  auto wb= MGMS()->getEnclosureBox();
  auto hlen = (int) floor(wz.size.height / bz.height);
  auto png= "gray.png";
  auto last= wlen-1;
  float x,y;

  ++hlen; // add one more to the top
  for (auto r = 1; r <= hlen; ++r) {
    FArrBrick rc(wlen);
    rc.fill(nullptr);
    auto y= wb.bottom + r * bz.height;
    auto x= wb.left;
    auto b= Brick::reify(c::Vec2(x,y), png);
    MGML()->addAtlasItem("game-pics", b->sprite);
    rc.setFirst(b);
    x += bz.width * (wlen-1);
    b= Brick::reify( c::Vec2(x,y), png);
    MGML()->addAtlasItem("game-pics", b->sprite);
    rc.setLast(b);
/* test to fill up entire box
    y= wb.bottom + r * bz.height;
    x= wb.left;
    for (int i=1; i < (wlen-1); ++i) {
      x += bz.width;
      b= Brick::reify( c::Vec2(x,y), "0.png");
      MGML()->addAtlasItem("game-pics", b->sprite);
      rc.set(i,b);
    }
*/
    bks->grid.push_back(rc);
  }
  //pop off the extra row
  --hlen;

  // fill in first row == row[0]
  y = wb.bottom + bz.height;
  auto &rc=bks->grid[0];
  for (auto c = 1; c < last; ++c) {
    auto b= Brick::reify(
        c::Vec2(wb.left + c * bz.width, y), png);
    MGML()->addAtlasItem("game-pics", b->sprite);
    rc.set(c, b);
  }

  return f::Box4(
wb.bottom + hlen * bz.height,
wb.left + wlen * bz.width,
wb.bottom,
wb.left
      );
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



