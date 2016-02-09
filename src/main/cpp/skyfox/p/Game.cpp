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

  void onGUIXXX(f::ComObj *co, const c::Vec2 &tap);

  virtual void onMouseClick(f::ComObj*,const c::Vec2&);
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
    tMode= c::Touch::DispatchMode::ONE_BY_ONE;
  }
};

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onTouchStart(f::ComObj *co, c::Touch *touch) {
  if (ENP(touch)) { return false; }
  onGUIXXX(co, touch->getLocation());
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseClick(f::ComObj *co, const c::Vec2 &loc) {
  onGUIXXX(co,loc);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onGUIXXX(f::ComObj *co, const c::Vec2 &tap) {

  auto ss= CC_GNLF(GVars,shared,"slots");
  auto sp=co->sprite;

  if (sp->isVisible()) {

    auto child = sp->getChildByTag(kSpriteHalo);
    auto pos= co->pos();

    child->stopAllActions();
    sp->stopAllActions();
    child = sp->getChildByTag(kSpriteSparkle);
    child->stopAllActions();

    if (sp->getScale() > 0.25f) {
      ss->shockWave->setPosition(pos.x,pos.y);
      CC_SHOW(ss->shockWave);
      ss->shockWave->setOpacity(255);
      ss->shockWave->setScale(0.1f);
      ss->shockWave->runAction(
          c::ScaleTo::create(0.5f,
            sp->getScale() * 2.0f));
      ss->shockWave->runAction(
          ss->shockwaveSequence->clone());
      cx::sfxPlay("bombRelease");
    } else {
      cx::sfxPlay("bombFail");
    }
    ss->shockwaveHits = 0;
    CC_HIDE(sp);
  } else {
    sp->stopAllActions();
    sp->setScale(0.1f);
    co->setPos(tap.x,tap.y);
    CC_SHOW(sp);
    sp->setOpacity(50);
    sp->runAction(ss->growBomb->clone() );
    auto child = sp->getChildByTag(kSpriteHalo);
    child->runAction( ss->rotateSprite->clone());
    child = sp->getChildByTag(kSpriteSparkle);
    child->runAction( ss->rotateSprite->clone());
  }

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

  auto ss= CC_GNLF(GVars,shared,"slots");
  auto bomb=CC_GNLF(Bomb,bombs,"bomb");
  auto ufo=CC_GNLF(Ufo,ufos,"ufo");

  //_gameOverMessage->setVisible(true);
  cx::sfxPlay("fire_truck");
  MGMS()->stop();

  F__LOOP(it,ss->fallingObjects) {
    it->second->sprite->stopAllActions();
    CC_HIDE(it->second->sprite);
  }

  ss->fallingObjects.clear();

  if (bomb->sprite->isVisible()) {
      bomb->sprite->stopAllActions();
      CC_HIDE(bomb->sprite);
      auto child = bomb->sprite->getChildByTag(kSpriteHalo);
      child->stopAllActions();
      child = bomb->sprite->getChildByTag(kSpriteSparkle);
      child->stopAllActions();
  }
  if (ss->shockWave->isVisible()) {
      ss->shockWave->stopAllActions();
      CC_HIDE(ss->shockWave);
  }
  if (ufo->sprite->isVisible()) {
      ufo->sprite->stopAllActions();
      CC_HIDE(ufo->sprite);
      auto ray = ufo->sprite->getChildByTag(kSpriteRay);
      ray->stopAllActions();
      CC_HIDE(ray);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::postReify() {

  shared = engine->getNodeList(SharedNode().typeId());
  ufos = engine->getNodeList(UfoNode().typeId());
  bombs = engine->getNodeList(BombNode().typeId());

  auto ss=CC_GNLF(GVars,shared,"slots");
  auto bb= CC_GNLF(Bomb,bombs,"bomb");

  cx::sfxMusic("bg", true);
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

  createActions();

  getHUD()->updateEnergy(ss->energy);
  getHUD()->updateScore(0);

  this->motionees.push_back(bb);
}

//////////////////////////////////////////////////////////////////////////////
//
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
  if (topic=="/game/hud/updateenergy") {
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




