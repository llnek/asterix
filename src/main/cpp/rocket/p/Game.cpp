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
#include "core/CCSX.h"
#include "s/GEngine.h"
#include "HUD.h"
#include "Game.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(rocket)
BEGIN_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
struct CC_DLL GLayer : public f::GameLayer {

  HUDLayer* getHUD() {
    return (HUDLayer*)getSceneX()->getLayer(3); }

  virtual void onTouchMotion(f::ComObj*, c::Touch*);
  virtual bool onTouchStart(f::ComObj*, c::Touch*);
  virtual void onTouchEnd(f::ComObj*, c::Touch*);

  STATIC_REIFY_LAYER(GLayer)
  MDECL_DECORATE()
  MDECL_GET_IID(2)

  virtual void postReify();

  void createParticles();
  void resetStar();
  void createStarGrid();

  DECL_PTR(a::NodeList, drawings)
  DECL_PTR(a::NodeList, shared)
  DECL_PTR(a::NodeList, rockets)

  GLayer() {
    tMode= c::Touch::DispatchMode::ONE_BY_ONE;
  }

};

static s_arr<c::Vec2,7> PPOS = {
  c::Vec2(0.25f,0.8f),
  c::Vec2(0.8f,0.45f),
  c::Vec2(0.75f,0.8f),
  c::Vec2(0.5f,0.5f),
  c::Vec2(0.18f,0.45f),
  c::Vec2(0.8f,0.15f),
  c::Vec2(0.18f,0.1f)
};

//////////////////////////////////////////////////////////////////////////////
void GLayer::postReify() {
  drawings=engine->getNodeList(LineDrawingNode().typeId());
  shared = engine->getNodeList(SharedNode().typeId());
  rockets=engine->getNodeList(RocketNode().typeId());
  auto dw= CC_GNLF(LineDrawing,drawings,"drawing");
  auto rock=CC_GNLF(Rocket,rockets,"rocket");
  auto ss=CC_GNLF(GVars,shared,"slots");
  auto pool= MGMS()->getPool("Planets");
  auto wb=cx::visBox();

  ss->minLineLength = wb.right * 0.07f;
  ss->drawing = false;
  ss->state = kGameIntro;

  //add planets
  for (auto i=0; i < PPOS.size(); ++i) {
    auto s= cx::reifySprite("planet_"+s::to_string(i+1)+".png");
    auto &pos= PPOS[i];
    s->setPosition(wb.right * pos.x, wb.top * pos.y);
    addAtlasItem("game-pics",s,kBackground,kSpritePlanet);
    pool->checkin(mc_new1(Planet,s));
  }

  createParticles();
  createStarGrid();

  rock->setPos(wb.cx, wb.top * 0.1f);
  rock->sprite->setOpacity(255);
  rock->show();
  rock->reset();

  ss->timeBetweenPickups = 0.0;
  ss->cometInterval = 4;
  ss->cometTimer = 0;

  //getHUD()->updateScore(0);

  dw->lines->reset();

  //shuffle grid cells
  s::random_shuffle(ss->grid.begin(), ss->grid.end());
  ss->gridPos = 0;

  resetStar();

  ss->warp->stopSystem();

  cx::sfxMusic("background.mp3", true);
  cx::sfxPlay("rocket.wav");
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::resetStar() {
  auto rock=CC_GNLF(Rocket,rockets,"rocket");
  auto ss=CC_GNLF(GVars,shared,"slots");
  auto rpos=rock->pos();

  while (true) {
    auto pos= ss->grid[ss->gridPos];
    ++ss->gridPos;
    if (ss->gridPos >= ss->grid.size()) { ss->gridPos=0; }
    if (pow(pos.x - rpos.x, 2) +
        pow(pos.y - rpos.y, 2) > rock->radius() * 6) {
      ss->star->setPosition(pos);
      CC_SHOW(ss->star);
      ss->star->resetSystem();
      return;
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onTouchStart(f::ComObj *co, c::Touch *touch) {

  auto dw=CC_GNLF(LineDrawing,drawings,"drawing");
  auto rock=CC_GNLF(Rocket,rockets,"rocket");
  auto ss=CC_GNLF(GVars,shared,"slots");
  auto tap = touch->getLocation();
  auto rpos=rock->pos();
  auto dx = rpos.x - tap.x;
  auto dy = rpos.y - tap.y;

  if (dx * dx + dy * dy <= pow(rock->radius(), 2)) {
    rock->rotationOrientation= ROTATE_NONE;
    dw->lines->lineType= LINE_NONE;
    ss->drawing = true;
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchMotion(f::ComObj *co, c::Touch *touch) {
  auto dw=CC_GNLF(LineDrawing,drawings,"drawing");
  auto rock=CC_GNLF(Rocket,rockets,"rocket");
  auto ss=CC_GNLF(GVars,shared,"slots");
  if (ss->drawing) {
    auto tap = touch->getLocation();
    auto rpos=rock->pos();
    auto dx = rpos.x - tap.x;
    auto dy = rpos.y - tap.y;

    if (dx * dx + dy * dy > pow(ss->minLineLength, 2)) {
      rock->select(true);
      dw->lines->pivot= tap;
      dw->lines->lineType= LINE_TEMP;
    } else {
      rock->select(false);
      dw->lines->lineType= LINE_NONE;
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchEnd(f::ComObj *co, c::Touch *touch) {
  auto dw= CC_GNLF(LineDrawing,drawings,"drawing");
  auto rock=CC_GNLF(Rocket,rockets,"rocket");
  auto ss=CC_GNLF(GVars,shared,"slots");
  auto tap = touch->getLocation();

  //track if tapping on ship
  ss->drawing = false;
  rock->select(false);

  //if we are showing a temp line
  if (dw->lines->lineType == LINE_TEMP) {
    //set up dashed line
    dw->lines->pivot= tap;
    dw->lines->lineLength= rock->pos().distance(tap);

    //set up rocket
    auto circlelen = dw->lines->lineLength * 2 * M_PI;
    auto iters = floor(circlelen / rock->speed.x);
    rock->pivot=tap;
    rock->angularSpeed= 2 * M_PI / iters;

    auto diff = rock->pos();
    diff.subtract(rock->pivot);
    auto clkwise = diff.getRPerp();
    auto dot =clkwise.dot(rock->vel);

    if (dot > 0) {
      rock->angularSpeed= rock->angularSpeed * -1;
      rock->rotationOrientation= ROTATE_CLOCKWISE;
      rock->targetRotation=
        CC_RADIANS_TO_DEGREES(atan2(clkwise.y, clkwise.x));
    } else {
      rock->rotationOrientation= ROTATE_COUNTER;
      rock->targetRotation=
        CC_RADIANS_TO_DEGREES(atan2(-1 * clkwise.y,
              -1 * clkwise.x));
    }
    dw->lines->lineType= LINE_DASHED;
  }

}

//////////////////////////////////////////////////////////////////////////////
void GLayer::decorate() {
  auto wb=cx::visBox();

  centerImage("game.bg");
  regoAtlas("game-pics");

  engine = mc_new(GEngine);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::createParticles() {
  auto rocket=CC_GNLF(Rocket,rockets,"rocket");
  auto ss=CC_GNLF(GVars,shared,"slots");
  auto wb=cx::visBox();

  ss->jet = c::ParticleSystemQuad::create("pics/jet.plist");
  ss->jet->setSourcePosition(c::Vec2(-rocket->radius() * 0.8f,0));
  ss->jet->setAngle(180);
  ss->jet->stopSystem();
  addItem(ss->jet, kBackground);

  ss->boom = c::ParticleSystemQuad::create("pics/boom.plist");
  ss->boom->stopSystem();
  addItem(ss->boom, kForeground);

  ss->comet = c::ParticleSystemQuad::create("pics/comet.plist");
  ss->comet->stopSystem();
  ss->comet->setPosition(c::Vec2(0, wb.top * 0.6f));
  CC_HIDE(ss->comet);
  addItem(ss->comet, kForeground);

  ss->pickup = c::ParticleSystemQuad::create("pics/plink.plist");
  ss->pickup->stopSystem();
  addItem(ss->pickup, kMiddleground);

  ss->warp = c::ParticleSystemQuad::create("pics/warp.plist");
  ss->warp->setPosition(rocket->pos());
  addItem(ss->warp, kBackground);

  ss->star = c::ParticleSystemQuad::create("pics/star.plist");
  ss->star->stopSystem();
  CC_HIDE(ss->star);
  addItem(ss->star, kBackground, kSpriteStar);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::createStarGrid() {
  auto ss=CC_GNLF(GVars,shared,"slots");
  auto p=MGMS()->getPool("Planets");
  auto wz=cx::visRect();
  auto &ps= p->list();
  auto tile = 32;
  auto gridFrame = wz.size.width * 0.1f;
  auto rows = (wz.size.height - 4 * gridFrame)/tile;
  auto cols = (wz.size.width - 2 * gridFrame)/tile;

  for (auto r = 0; r < rows; ++r) {
    for (auto c = 0; c < cols; ++c) {
      auto cell = c::Vec2(gridFrame + c * tile, 2 * gridFrame + r * tile);
      auto overlaps = false;
      F__LOOP(it,ps) {
        auto pt= *it;
        auto pos= pt->pos();
        if (pow(pos.x - cell.x, 2) + pow(pos.y - cell.y, 2) <=
            pow(pt->radius() * 1.2f, 2)) {
          overlaps = true;
        }
      }
      if (!overlaps) { ss->grid.push_back(cell); }
    }
  }

  CCLOG("POSSIBLE STARS: %i", (int)ss->grid.size());
}

END_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic &topic, void *m) {
  auto y= (GLayer*) getGLayer();
}

//////////////////////////////////////////////////////////////////////////////
void Game::decorate() {
  HUDLayer::reify(this, 3);
  GLayer::reify(this, 2);
  play();
}



NS_END




