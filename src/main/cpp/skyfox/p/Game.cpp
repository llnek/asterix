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
  MDECL_DECORATE()
  MDECL_GET_IID(2)

  virtual bool onTouchStart(f::ComObj*, c::Touch*);
  virtual void postReify();
  void updateScore(int);
  void updateEnergy();
  void createActions();
  void onDone();

  DECL_PTR(a::NodeList, shared)
  DECL_PTR(a::NodeList, ufos)
  DECL_PTR(a::NodeList, bombs)

  GLayer() {
    tmode= c::Touch::DispatchMode::ONE_BY_ONE;
  }
};

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onTouchStart(f::ComObj *co, c::Touch *touch) {

  auto ss= CC_GNLF(GVars,shared,"slots");

  if (ENP(touch)) { return false; }

  if (co->sprite->isVisible()) {

    auto child = co->sprite->getChildByTag(kSpriteHalo);
    auto pos= co->pos();

    co->sprite->stopAllActions();
    child->stopAllActions();
    child = co->sprite->getChildByTag(kSpriteSparkle);
    child->stopAllActions();

    if (co->sprite->getScale() > 0.25f) {

      ss->shockWave->setPosition(pos.x,pos.y);
      CC_SHOW(ss->shockWave);
      ss->shockWave->setOpacity(255);
      ss->shockWave->setScale(0.1f);
      ss->shockWave->runAction(
          c::ScaleTo::create(0.5f,
            co->sprite->getScale() * 2.0f));
      ss->shockWave->runAction(
          ss->shockwaveSequence->clone());
      cx::sfxPlay("bombRelease");
    } else {
      cx::sfxPlay("bombFail");
    }
    ss->shockwaveHits = 0;
    CC_HIDE(co->sprite);

  } else {

    auto tap = touch->getLocation();
    co->sprite->stopAllActions();
    co->sprite->setScale(0.1f);
    co->setPos(tap.x,tap.y);
    CC_SHOW(co->sprite)
    co->sprite->setOpacity(50);
    co->sprite->runAction(ss->growBomb->clone() );

    child = co->sprite->getChildByTag(kSpriteHalo);
    child->runAction( ss->rotateSprite->clone());
    child = co->sprite->getChildByTag(kSpriteSparkle);
    child->runAction( ss->rotateSprite->clone());
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::updateScore(int n) {
  getHUD()->updateScore(n);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::updateEnergy() {
  auto ss= CC_GNLF(GVars,shared,"slots");
  getHUD()->updateEnergy(ss->energy);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onDone() {

  //_gameOverMessage->setVisible(true);
  cx::sfxEffect("fire_truck");
  MGMS()->stop();

  //stop all actions currently running (meteors, health drops, animations,
  int i;
  int count = (int) _fallingObjects.size();

  for (i = count-1; i >= 0; i--) {
      auto sprite = _fallingObjects.at(i);
      sprite->stopAllActions();
      sprite->setVisible(false);
      _fallingObjects.erase(i);
  }
  if (_bomb->isVisible()) {
      _bomb->stopAllActions();
      _bomb->setVisible(false);
      auto child = _bomb->getChildByTag(kSpriteHalo);
      child->stopAllActions();
      child = _bomb->getChildByTag(kSpriteSparkle);
      child->stopAllActions();
  }
  if (_shockWave->isVisible()) {
      _shockWave->stopAllActions();
      _shockWave->setVisible(false);
  }
  if (_ufo->isVisible()) {
      _ufo->stopAllActions();
      _ufo->setVisible(false);
      auto ray = _ufo->getChildByTag(kSpriteRay);
      ray->stopAllActions();
      ray->setVisible(false);
  }

}

//////////////////////////////////////////////////////////////////////////////
void GLayer::postReify() {

  shared = engine->getNodeList(SharedNode().typeId());
  cx::sfxMusic("bg", true);

  auto ss=CC_GNLF(GVars,shared,"slots");
  ss->energy = 100;

  ss->meteorInterval = 3.5;
  ss->meteorTimer = ss->meteorInterval * 0.99f;
  ss->meteorSpeed = 10;//in seconds to reach ground

  ss->ufoInterval = 20;
  ss->ufoTimer = - ss->ufoInterval;
  ss->ufoKilled = false;

  ss->healthInterval = 25;
  ss->healthTimer = - ss->healthInterval;
  ss->healthSpeed = 15;//in seconds to reach ground

  ss->difficultyInterval = 60;
  ss->difficultyTimer = 0;

  getHUD()->updateEnergy(ss->energy);
  getHUD()->updateScore(0);
}

//////////////////////////////////////////////////////////////////////////////
void GLayer::decorate() {

  auto wb= cx::visBox();

  centerImage("game.bg");
  regoAtlas("game-pics");

  //create cityscape
  for (auto i = 0; i < 2; ++i) {
    auto s= cx::reifySprite("city_dark.png");
    s->setAnchorPoint(cx::anchorB());
    s->setPosition(wb.right * (0.25f+i*0.5f), 0);
    addAtlasItem("game-pics", s, kMiddleground);

    s= cx::reifySprite("city_light.png");
    s->setAnchorPoint(cx::anchorB());
    s->setPosition(wb.right * (0.25f+i*0.5f), wb.top * 0.1f);
    addAtlasItem("game-pics", s, kBackground);
  }

  //add trees
  for (auto i = 0; i < 3; ++i) {
    auto s= cx::reifySprite("trees.png");
    s->setAnchorPoint(cx::anchorB());
    s->setPosition(wb.right * (0.2f+i*0.3f), 0);
    addAtlasItem("game-pics",s, kMiddleground);
  }

  createActions();

  engine = mc_new(GEngine);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::createActions() {

  auto easeSwing = c::Sequence::create(
      c::EaseInOut::create(
        c::RotateTo::create(1.2f, -10), 2),
        c::EaseInOut::create(c::RotateTo::create(1.2f, 10), 2), CC_NIL);
  auto ss= CC_GNLF(GVars,shared,"slots");
  auto wb= cx::visBox();

  ss->swingHealth = c::RepeatForever::create( (c::ActionInterval*) easeSwing );
  CC_KEEP(ss->swingHealth)

  ss->shockwaveSequence = c::Sequence::create(
      c::FadeOut::create(1.0f),
      c::CallFunc::create([=]() { CC_HIDE(ss->shockWave); }),
      CC_NIL);
  CC_KEEP(ss->shockwaveSequence)

  ss->growBomb = c::ScaleTo::create(4.0f, 1);
  CC_KEEP(ss->growBomb)

  ss->rotateSprite = c::RepeatForever::create(
      c::RotateBy::create(0.5f ,  -90));
  CC_KEEP(ss->rotateSprite)

  //animations
  auto animation = c::Animation::create();
  for (auto i = 1; i <= 10; ++i) {
    animation->addSpriteFrame(
        cx::getSpriteFrame("boom"+s::to_string(i)+".png"));
  }
  animation->setRestoreOriginalFrame(true);
  animation->setDelayPerUnit(1 / 10.0f);
  ss->groundHit = c::Sequence::create(
              c::MoveBy::create(0, c::Vec2(0,wb.top * 0.12f)),
              c::Animate::create(animation),
              c::CallFuncN::create(
                [](c::Node *p) { CC_HIDE(p); }),
              CC_NIL);
  CC_KEEP(ss->groundHit)

  animation = c::Animation::create();
  for (auto i = 1; i <= 7; ++i) {
    animation->addSpriteFrame(
        cx::getSpriteFrame(
          "explosion_small"+s::to_string(i)+".png"));
  }
  animation->setRestoreOriginalFrame(true);
  animation->setDelayPerUnit(0.5 / 7.0f);
  ss->explosion = c::Sequence::create(
          c::Animate::create(animation),
          c::CallFuncN::create(
            [](c::Node *p) { CC_HIDE(p); }),
          CC_NIL);
  CC_KEEP(ss->explosion)
}

END_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic &topic, void *m) {
  auto y= (GLayer*) getGLayer();

  if (topic=="/game/player/earnscore") {
    auto msg=(j::json*)m;
    y->updateScore(
        JS_INT(msg->operator[]("score")));
  }
  else
  if (topic=="/game/hud/updateEnergy") {
    y->updateEnergy();
  }
  else
  if (topic=="/game/end") {
    y->onDone();
  }
}

//////////////////////////////////////////////////////////////////////////////
void Game::decorate() {
  HUDLayer::reify(this, 3);
  GLayer::reify(this, 2);
  play();
}



NS_END




