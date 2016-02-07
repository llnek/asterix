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

  void updateEnergy();
  void createActions();
  void onDone();
  virtual void postReify();

  DECL_PTR(c::RepeatForever,rotateSprite)
  DECL_PTR(c::RepeatForever,swingHealth)
  DECL_PTR(c::RepeatForever,blinkRay)
  DECL_PTR(c::Sequence,shockwaveSequence)
  DECL_PTR(c::Sequence,groundHit)
  DECL_PTR(c::Sequence,explosion)
  DECL_PTR(c::Node,shockWave)
  DECL_PTR(c::ScaleTo,growBomb)
  DECL_PTR(c::Animate,ufoAnimation)
  DECL_PTR(c::Sprite,ufo)
  DECL_PTR(a::NodeList, shared)
  s_vec<c::Sprite*> clouds;

  GLayer() {
    tmode= c::Touch::DispatchMode::ONE_BY_ONE;
  }
};

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::updateEnergy() {
  auto ss= CC_GNLF(GVars,shared,"slots");
  getHUD()->updateEnergy(ss->energy);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onDone() {
  cx::sfxEffect("fire_truck");
  //_gameOverMessage->setVisible(true);
  MGMS()->stop();
}

//////////////////////////////////////////////////////////////////////////////
void GLayer::postReify() {
  shared = engine->getNodeList(SharedNode().typeId());
  cx::sfxMusic("bg", true);
}

//////////////////////////////////////////////////////////////////////////////
void GLayer::decorate() {
  auto wb= cx::visBox();

  centerImage("game.bg");
  regoAtlas("game-pics");

  engine = mc_new(GEngine);

  //create cityscape
  for (auto i = 0; i < 2; ++i) {
    auto s= cx::reifySprite("city_dark.png");
    s->setAnchorPoint(c::Vec2(0.5,0));
    s->setPosition(wb.right * (0.25f + i * 0.5f), 0);
    addAtlasItem("game-pics", s, kMiddleground);

    s= cx::reifySprite("city_light.png");
    s->setAnchorPoint(c::Vec2(0.5,0));
    s->setPosition(wb.right * (0.25f + i * 0.5f), wb.top * 0.1f);
    addAtlasItem("game-pics", s, kBackground);
  }

  //add trees
  for (auto i = 0; i < 3; ++i) {
    auto s= cx::reifySprite("trees.png");
    s->setAnchorPoint(c::Vec2(0.5f, 0.0f));
    s->setPosition(wb.right * (0.2f + i * 0.3f), 0);
    addAtlasItem("game-pics",s, kMiddleground);
  }

  //add clouds
  for (auto i = 0; i < 4; ++i) {
    auto cloud_y = wb.top * (i % 2 == 0 ? 0.4f : 0.5f);
    auto cloud = cx::reifySprite("cloud.png");
    cloud->setPosition(wb.right * 0.1f + i * wb.right * 0.3f, cloud_y);
    addAtlasItem("game-pics", cloud, kBackground);
    clouds.push_back(cloud);
  }

  createActions();
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::createActions() {

  auto easeSwing = c::Sequence::create(
      c::EaseInOut::create(
        c::RotateTo::create(1.2f, -10), 2),
        c::EaseInOut::create(c::RotateTo::create(1.2f, 10), 2), CC_NIL);
  auto wb= cx::visBox();

  this->swingHealth = c::RepeatForever::create( (c::ActionInterval*) easeSwing );
  CC_KEEP(this->swingHealth)

  this->shockwaveSequence = c::Sequence::create(
      c::FadeOut::create(1.0f),
      c::CallFunc::create([=]() {
        this->shockWave->setVisible(false);
        }),
      CC_NIL);
  CC_KEEP(this->shockwaveSequence)

  this->growBomb = c::ScaleTo::create(4.0f, 1);
  CC_KEEP(this->growBomb)

  this->rotateSprite = c::RepeatForever::create(
      c::RotateBy::create(0.5f ,  -90));
  CC_KEEP(this->rotateSprite)

  //animations
  auto animation = c::Animation::create();
  for (auto i = 1; i <= 10; ++i) {
    animation->addSpriteFrame(
        cx::getSpriteFrame( "boom"+s::to_string(i)+".png"));
  }
  animation->setRestoreOriginalFrame(true);
  animation->setDelayPerUnit(1 / 10.0f);
  this->groundHit = c::Sequence::create(
              c::MoveBy::create(0, c::Vec2(0,wb.top * 0.12f)),
              c::Animate::create(animation),
              c::CallFuncN::create([](c::Node *p) {
                p->setVisible(false);
                }),
              CC_NIL);
  CC_KEEP(this->groundHit)

  animation = c::Animation::create();
  for (auto i = 1; i <= 7; ++i) {
    animation->addSpriteFrame(
        cx::getSpriteFrame(
          "explosion_small"+s::to_string(i)+".png"));
  }
  animation->setRestoreOriginalFrame(true);
  animation->setDelayPerUnit(0.5 / 7.0f);
  this->explosion = c::Sequence::create(
          c::Animate::create(animation),
          c::CallFuncN::create([](c::Node *p) {
            p->setVisible(false);
            }),
          CC_NIL);
  CC_KEEP(this->explosion)

  //add ufo
  this->ufo = cx::reifySprite("ufo_1.png");
  animation = c::Animation::create();
  for (auto i = 1; i <= 4; ++i) {
    animation->addSpriteFrame(
        cx::getSpriteFrame(
          "ufo_" + s::to_string(i) +".png"));
  }
  animation->setDelayPerUnit(1.0 / 4.0f);
  animation->setLoops(-1);
  this->ufoAnimation = c::Animate::create(animation);
  CC_KEEP(this->ufoAnimation)

  auto uz= this->ufo->getBoundingBox().size;
  auto ray = cx::reifySprite("ray.png");
  ray->setAnchorPoint(c::Vec2(0.5, 1.0));
  ray->setPosition(uz.width  * 0.52f, uz.height * 0.5f);

  this->blinkRay = c::RepeatForever::create(c::Blink::create(4, 6));
  CC_KEEP(this->blinkRay)
  this->ufo->addChild(ray, -1, kSpriteRay);

  this->ufo->setVisible(false);
  addAtlasItem("game-pics", this->ufo, 0, kSpriteUfo);
}

END_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic &topic, void *m) {
  auto y= (GLayer*) getGLayer();

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




