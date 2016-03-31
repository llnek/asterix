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
NS_BEGIN(skyfox)
BEGIN_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
struct CC_DLL GLayer : public f::GameLayer {

  HUDLayer* getHUD() {
    return (HUDLayer*)getSceneX()->getLayer(3); }

  STATIC_REIFY_LAYER(GLayer)
  __decl_deco_ui()
  __decl_get_iid(2)

  virtual void onMouseClick(const c::Vec2&);
  virtual bool onTouchStart(c::Touch*);
  virtual void onInited();

  void updateEnergy();
  void createActions();
  void onDone();

  __decl_ptr(ecs::Node, _shared)
  __decl_ptr(ecs::Node, _ufo)
  __decl_ptr(ecs::Node, _bomb)

};

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onTouchStart(c::Touch *touch) {
  onMouseClick(touch->getLocation());
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseClick(const c::Vec2 &tap) {

  auto co=CC_GEC(f::CPixie,_bomb,"f/CPixie");
  auto ss= CC_GEC(GVars,_shared,"n/GVars");
  auto pos= co->pos();

  if (co->isOvert()) {

    CC_GCT(co->node,kSpriteSparkle)->stopAllActions();
    CC_GCT(co->node,kSpriteHalo)->stopAllActions();
    co->node->stopAllActions();

    if (co->node->getScale() > 0.25) {
      ss->shockWave->setPosition(pos.x,pos.y);
      CC_SHOW(ss->shockWave);
      ss->shockWave->setOpacity(255);
      ss->shockWave->setScale(0.1);
      ss->shockWave->runAction(
          c::ScaleTo::create(0.5,
            co->node->getScale() * 2.0));
      ss->shockWave->runAction(
          ss->shockwaveSequence->clone());
      cx::sfxPlay("bombRelease");
    } else {
      cx::sfxPlay("bombFail");
    }
    ss->shockwaveHits = 0;
    co->hide();
  } else {
    co->node->stopAllActions();
    co->node->setScale(0.1);
    co->setPos(tap.x,tap.y);
    co->show();
    co->node->setOpacity(50);
    co->node->runAction(ss->growBomb->clone() );
    CC_GCT(co->node,kSpriteHalo)->runAction(
        ss->rotateSprite->clone());
    CC_GCT(co->node,kSpriteSparkle)->runAction(
        ss->rotateSprite->clone());
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::updateEnergy() {
  auto ss= CC_GEC(GVars,_shared,"n/GVars");
  getHUD()->updateEnergy(ss->energy);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onDone() {

  auto bomb=CC_GEC(f::CPixie,_bomb,"f/CPixie");
  auto ufo=CC_GEC(f::CPixie,_ufo,"f/CPixie");
  auto ss= CC_GEC(GVars,_shared,"n/GVars");
  auto wb=cx::visBox();

  //_gameOverMessage->setVisible(true);
  this->setOpacity(0.1 * 255);
  cx::sfxPlay("fire_truck");
  MGMS()->stop();
  surcease();

  F__LOOP(it,ss->fallingObjects) {
    auto ui=CC_GEC(f::CPixie,it->second,"f/CPixie");
    ui->node->stopAllActions();
    ui->hide();
  }

  ss->fallingObjects.clear();

  if (bomb->isOvert()) {
    CC_GCT(bomb->node,kSpriteSparkle)->stopAllActions();
    CC_GCT(bomb->node,kSpriteHalo)->stopAllActions();
    bomb->node->stopAllActions();
    bomb->hide();
  }
  if (ss->shockWave->isVisible()) {
    ss->shockWave->stopAllActions();
    CC_HIDE(ss->shockWave);
  }
  if (ufo->isOvert()) {
    CC_GCT(ufo->node,kSpriteRay)->stopAllActions();
    ufo->node->stopAllActions();
    ufo->hide();
  }

  auto btn=cx::reifyMenuBtn("gameover.png");
  auto mnu=cx::mkMenu(btn);
  btn->setPosition(wb.cx,wb.top*0.65);
  btn->setCallback([=](c::Ref*){
    cx::runEx(Game::reify(mc_new(f::GCX)));
  });
  addItem(mnu);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onInited() {

  _shared = _engine->getNodes("n/GVars")[0];
  _ufo = _engine->getNodes("f/CAutoma")[0];
  _bomb = _engine->getNodes("f/CGesture")[0];

  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  ss->energy = 100;

  ss->meteorInterval = 3.5;
  ss->meteorTimer = ss->meteorInterval * 0.99;
  ss->meteorSpeed = 10;//in seconds to reach ground

  ss->ufoInterval = 20;
  ss->ufoTimer = - ss->ufoInterval;
  ss->ufoKilled = false;

  ss->healthInterval = 25;
  ss->healthTimer = - ss->healthInterval;
  ss->healthSpeed = 15;//in seconds to reach ground

  ss->difficultyInterval = 60;
  ss->difficultyTimer = 0;

  createActions();

  getHUD()->updateEnergy(ss->energy);
  getHUD()->updateScore(0);

  cx::sfxMusic("bg", true);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::decoUI() {

  auto wb= cx::visBox();

  centerImage("game.bg");
  regoAtlas("game-pics");

  //create cityscape
  for (auto i = 0; i < 2; ++i) {
    auto s= cx::reifySprite("city_dark.png");
    s->setAnchorPoint(cx::anchorB());
    s->setPosition(wb.right * (0.25+i*0.5), 0);
    addAtlasItem("game-pics", s, kMiddleground);
    //
    s= cx::reifySprite("city_light.png");
    s->setAnchorPoint(cx::anchorB());
    s->setPosition(wb.right * (0.25+i*0.5), wb.top * 0.1);
    addAtlasItem("game-pics", s, kBackground);
  }

  //add trees
  for (auto i = 0; i < 3; ++i) {
    auto s= cx::reifySprite("trees.png");
    s->setAnchorPoint(cx::anchorB());
    s->setPosition(wb.right * (0.2+i*0.3), 0);
    addAtlasItem("game-pics",s, kMiddleground);
  }

  _engine = mc_new(GEngine);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::createActions() {

  auto ss= CC_GEC(GVars,_shared,"n/GVars");
  auto wb= cx::visBox();
  auto easeSwing = c::Sequence::create(
      c::EaseInOut::create(
        c::RotateTo::create(1.2, -10), 2),
        c::EaseInOut::create(
          c::RotateTo::create(1.2, 10), 2),
        CC_NIL);

  ss->swingHealth = c::RepeatForever::create(
      (c::ActionInterval*) easeSwing );
  CC_KEEP(ss->swingHealth)

  ss->shockwaveSequence = c::Sequence::create(
      c::FadeOut::create(1.0),
      c::CallFunc::create([=]() { CC_HIDE(ss->shockWave); }),
      CC_NIL);
  CC_KEEP(ss->shockwaveSequence)

  ss->growBomb = c::ScaleTo::create(4.0, 1);
  CC_KEEP(ss->growBomb)

  ss->rotateSprite = c::RepeatForever::create(
      c::RotateBy::create(0.5,  -90));
  CC_KEEP(ss->rotateSprite)

  //animations
  auto anim= c::Animation::create();
  for (auto i = 1; i <= 10; ++i) {
    anim->addSpriteFrame(
        cx::getSpriteFrame("boom"+FTOS(i)+".png"));
  }
  anim->setRestoreOriginalFrame(true);
  anim->setDelayPerUnit(1 / 10.0);
  ss->groundHit = c::Sequence::create(
              c::MoveBy::create(0, c::Vec2(0,wb.top * 0.12)),
              c::Animate::create(anim),
              c::CallFuncN::create(
                [](c::Node *p) { CC_HIDE(p); }),
              CC_NIL);
  CC_KEEP(ss->groundHit);

  anim= c::Animation::create();
  for (auto i = 1; i <= 7; ++i) {
    anim->addSpriteFrame(
        cx::getSpriteFrame(
          "explosion_small"+FTOS(i)+".png"));
  }
  anim->setRestoreOriginalFrame(true);
  anim->setDelayPerUnit(0.5 / 7.0);
  ss->explosion = c::Sequence::create(
          c::Animate::create(anim),
          c::CallFuncN::create(
            [](c::Node *p) { CC_HIDE(p); }),
          CC_NIL);
  CC_KEEP(ss->explosion);
}

END_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic &topic, void *m) {
  auto y= (GLayer*) getGLayer();

  if (topic=="/game/player/earnscore") {
    auto msg=(j::json*)m;
    y->getHUD()->updateScore(
        JS_INT(msg->operator[]("score")));
  }
  else
  if (topic=="/game/hud/updateenergy") {
    y->updateEnergy();
  }
  else
  if (topic=="/game/end") {
    y->onDone();
  }
}

//////////////////////////////////////////////////////////////////////////////
void Game::decoUI() {
  HUDLayer::reify(this, 3);
  GLayer::reify(this, 2);
  play();
}



NS_END




