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
#include "Ende.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(skyfox)
BEGIN_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
struct CC_DLL GLayer : public f::GameLayer {

  HUDLayer* getHUD() {
    return (HUDLayer*)getSceneX()->getLayer(3); }

  __decl_create_layer(GLayer)
  __decl_deco_ui()
  __decl_get_iid(2)

  virtual bool onMouseStart(const CCT_PT&);
  virtual bool onTouchStart(c::Touch*);
  virtual void onInited();

  void createActions();
  void updateEnergy();
  void onEnd();

  __decl_ptr(ecs::Node, _shared)
  __decl_ptr(ecs::Node, _ufo)
  __decl_ptr(ecs::Node, _bomb)

};

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onTouchStart(c::Touch *touch) {
  return onMouseStart(touch->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onMouseStart(const CCT_PT &tap) {

  auto co= CC_GEC(f::CPixie,_bomb,"f/CPixie");
  auto ss= CC_GEC(GVars,_shared,"n/GVars");
  auto pos= co->getPosition();

  if (co->isVisible()) {

    CC_GCT(co,kSpriteSparkle)->stopAllActions();
    CC_GCT(co,kSpriteHalo)->stopAllActions();
    co->stopAllActions();

    if (co->getScale() > 0.25) {
      CC_POS2(ss->shockWave, pos.x,pos.y);
      CC_SHOW(ss->shockWave);
      ss->shockWave->setOpacity(255);
      ss->shockWave->setScale(0.1);
      ss->shockWave->runAction(
          c::ScaleTo::create(0.5, co->getScale() * 2));
      ss->shockWave->runAction(
          ss->shockwaveSequence->clone());
      cx::sfxPlay("bombRelease");
    } else {
      cx::sfxPlay("bombFail");
    }
    ss->shockwaveHits = 0;
    CC_HIDE(co);
  } else {
    co->stopAllActions();
    co->setScale(0.1);
    CC_POS2(co, tap.x,tap.y);
    CC_SHOW(co);
    co->setOpacity(50);
    co->runAction(ss->growBomb->clone() );
    CC_GCT(co,kSpriteHalo)->runAction(
        ss->rotateSprite->clone());
    CC_GCT(co,kSpriteSparkle)->runAction(
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
void GLayer::onEnd() {

  auto bomb=CC_GEC(f::CPixie,_bomb,"f/CPixie");
  auto ufo=CC_GEC(f::CPixie,_ufo,"f/CPixie");
  auto ss= CC_GEC(GVars,_shared,"n/GVars");
  auto wb=cx::visBox();

  cx::sfxPlay("fire_truck");
  MGMS()->stop();
  surcease();

  F__LOOP(it,ss->fallingObjects) {
    auto ui=CC_GEC(f::CPixie,it->second,"f/CPixie");
    ui->stopAllActions();
    CC_HIDE(ui);
  }

  ss->fallingObjects.clear();

  if (bomb->isVisible()) {
    CC_GCT(bomb,kSpriteSparkle)->stopAllActions();
    CC_GCT(bomb,kSpriteHalo)->stopAllActions();
    bomb->stopAllActions();
    CC_HIDE(bomb);
  }

  if (ss->shockWave->isVisible()) {
    ss->shockWave->stopAllActions();
    CC_HIDE(ss->shockWave);
  }

  if (ufo->isVisible()) {
    CC_GCT(ufo,kSpriteRay)->stopAllActions();
    ufo->stopAllActions();
    CC_HIDE(ufo);
  }

  Ende::reify(MGMS(),4);
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
  regoAtlas("cc-pics");

  //create cityscape
  for (auto i = 0; i < 2; ++i) {
    auto s= cx::reifySprite("city_dark.png");
    s->setAnchorPoint(cx::anchorB());
    CC_POS2(s, wb.right * (0.25+i*0.5), 0);
    addAtlasItem("game-pics", s, kMiddleground);
    //
    s= cx::reifySprite("city_light.png");
    s->setAnchorPoint(cx::anchorB());
    CC_POS2(s, wb.right * (0.25+i*0.5), wb.top * 0.1);
    addAtlasItem("game-pics", s, kBackground);
  }

  //add trees
  for (auto i = 0; i < 3; ++i) {
    auto s= cx::reifySprite("trees.png");
    s->setAnchorPoint(cx::anchorB());
    CC_POS2(s, wb.right * (0.2+i*0.3), 0);
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
      c::FadeOut::create(1),
      c::CallFunc::create([=]() { CC_HIDE(ss->shockWave); }),
      CC_NIL);
  CC_KEEP(ss->shockwaveSequence)

  ss->growBomb = c::ScaleTo::create(4.0, 1);
  CC_KEEP(ss->growBomb)

  ss->rotateSprite = c::RepeatForever::create(
      c::RotateBy::create(0.5,  -90));
  CC_KEEP(ss->rotateSprite)

  //animations
  auto anim= cx::createAnimation(1.0/10.0,true,0);
  for (auto i = 1; i <= 10; ++i) {
    anim->addSpriteFrame(
        cx::getSpriteFrame("boom"+FTOS(i)+".png"));
  }
  ss->groundHit = c::Sequence::create(
              c::MoveBy::create(0, CCT_PT(0,wb.top * 0.12)),
              c::Animate::create(anim),
              c::CallFuncN::create(
                [](c::Node *p) { CC_HIDE(p); }),
              CC_NIL);
  CC_KEEP(ss->groundHit);

  anim= cx::createAnimation(0.5/7.0,true,0);
  for (auto i = 1; i <= 7; ++i) {
    anim->addSpriteFrame(
        cx::getSpriteFrame(
          "explosion_small"+FTOS(i)+".png"));
  }
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
    y->onEnd();
  }
}

//////////////////////////////////////////////////////////////////////////////
void Game::decoUI() {
  HUDLayer::reify(this, 3);
  GLayer::reify(this, 2);
  play();
}



NS_END




