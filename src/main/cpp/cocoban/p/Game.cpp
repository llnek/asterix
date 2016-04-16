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

#include "core/XConfig.h"
#include "core/CCSX.h"
#include "s/GEngine.h"
#include "HUD.h"
#include "MMenu.h"
#include "Ende.h"
#include "Game.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(cocoban)
BEGIN_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
struct CC_DLL GLayer : public f::GameLayer {

  HUDLayer* getHUD() {
    return (HUDLayer*)getSceneX()->getLayer(3); }

  void move(int, int);
  void onSwipe();

  __decl_ptr(ecs::Node, _player)
  __decl_ptr(ecs::Node, _shared)

  __decl_create_layer(GLayer)
  __decl_deco_ui()
  __decl_get_iid(2)

  virtual void onMouseMotion(const CCT_PT&);
  virtual bool onMouseStart(const CCT_PT&);
  virtual void onMouseClick(const CCT_PT&);

  virtual void onTouchMotion(c::Touch*);
  virtual bool onTouchStart(c::Touch*);
  virtual void onTouchEnd(c::Touch*);

  virtual void onInited();

  virtual ~GLayer() {}
};

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onInited() {
  _player= _engine->getNodes("f/CGesture")[0];
  _shared= _engine->getNodes("n/GVars")[0];

  auto ss= CC_GEC(GVars,_shared,"n/GVars");
  auto wz= cx::visSize();
  auto wb= cx::visBox();

}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onSwipe() {
  auto ss= CC_GEC(GVars,_shared,"n/GVars");
  auto dx = ss->startTouch.x - ss->endTouch.x;
  auto dy = ss->startTouch.y - ss->endTouch.y;
  auto DX= abs(dx);
  auto DY= abs(dy);

  if (DX + DY  > ss->swipeTolerance) {
    if (DX > DY) {
      if (dx > 0){
        move(-1,0);
      } else {
        move(1,0);
      }
    } else {
      if (dy > 0){
        move(0,1);
      } else {
        move(0,-1);
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::move(int dx, int dy) {
  auto py=CC_GEC(f::CPixie,_player,"f/CPixie");
  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  auto pos= ss->playerPos;

  switch (ss->levels[pos.y+dy][pos.x+dx]) {
    case 0:
    case 2:
      ss->levels[pos.y][pos.x] -= 4;
      ss->playerPos.x += dx;
      ss->playerPos.y += dy;
      ss->levels[ss->playerPos.y][ss->playerPos.x] += 4;
      CC_POS2(py, 165+25*ss->playerPos.x,
                  185-25*ss->playerPos.y);
    break;
    case 3:
    case 5:
      if (ss->levels[ss->playerPos.y+dy*2][ss->playerPos.x+dx*2]==0 ||
          ss->levels[ss->playerPos.y+dy*2][ss->playerPos.x+dx*2]==2) {
        ss->levels[ss->playerPos.y][ss->playerPos.x] -= 4;
        ss->playerPos.x += dx;
        ss->playerPos.y += dy;
        ss->levels[ss->playerPos.y][ss->playerPos.x] += 1;
        CC_POS2(py, 165+25*ss->playerPos.x,
                    185-25*ss->playerPos.y);
        ss->levels[ss->playerPos.y+dy][ss->playerPos.x+dx] += 3;
        auto movingCrate =
          ss->crates[ss->playerPos.y]->get(ss->playerPos.x);
        CC_POS2(movingCrate, movingCrate->getPositionX()+25*dx,
                             movingCrate->getPositionY()-25*dy);
        ss->crates[ss->playerPos.y+dy]->set(ss->playerPos.x+dx,movingCrate);
        ss->crates[ss->playerPos.y]->set(ss->playerPos.x,CC_NIL);
      }
    break;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseClick(const CCT_PT &loc) {
  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  ss->endTouch=loc;
  onSwipe();
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseMotion(const CCT_PT &loc) {
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onMouseStart(const CCT_PT &loc) {
  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  ss->startTouch=loc;
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onTouchStart(c::Touch *touch) {
  return onMouseStart(touch->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchMotion(c::Touch *touch) {
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchEnd(c::Touch *touch) {
  onMouseClick(touch->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
void GLayer::decoUI() {
  auto bg = cx::reifySprite("background.png");
  auto wb= cx::visBox();

  regoAtlas("game-pics");

  bg->getTexture()->setAliasTexParameters();
  CC_POS2(bg, wb.cx,wb.cy);
  XCFG()->fit(bg);
  addAtlasItem("game-pics",bg);

  auto level= cx::reifySprite("level.png");
  CC_POS2(level, wb.cx, wb.cy);
  XCFG()->fit(level);
  addAtlasItem("game-pics",level);

  _engine = mc_new(GEngine);
  //cx::sfxMusic("background", true);
}

END_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic &topic, void *m) {
  auto y= (GLayer*) getGLayer();
}

//////////////////////////////////////////////////////////////////////////////
void Game::decoUI() {
  HUDLayer::reify(this, 3);
  GLayer::reify(this, 2);
  play();
}


NS_END




