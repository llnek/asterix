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

  virtual void postReify();

  DECL_PTR(a::NodeList, shared)
  s_vec<c::Sprite*> clouds;

};

//////////////////////////////////////////////////////////////////////////////
void GLayer::postReify() {
  shared = engine->getNodeList(SharedNode().typeId());
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

}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::createActions() {

  auto easeSwing = c::Sequence::create(
      c::EaseInOut::create(
        c::RotateTo::create(1.2f, -10), 2),
        c::EaseInOut::create(c::RotateTo::create(1.2f, 10), 2), CC_NIL);

  _swingHealth = c::RepeatForever::create( (c::ActionInterval *) easeSwing );
  _swingHealth->retain();

  _shockwaveSequence = c::Sequence::create(
      c::FadeOut::create(1.0f),
      c::CallFunc::create(s::bind(&GameLayer::shockwaveDone, this)),
                    nullptr);
    _shockwaveSequence->retain();


    _growBomb = ScaleTo::create(4.0f, 1);
    _growBomb->retain();


    auto  rotate = RotateBy::create(0.5f ,  -90);
    _rotateSprite = RepeatForever::create( rotate );
    _rotateSprite->retain();


    //animations
    auto animation = Animation::create();
    int i;
    for(i = 1; i <= 10; i++) {
        auto name = String::createWithFormat("boom%i.png", i);
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(name->getCString());
        animation->addSpriteFrame(frame);
    }

    animation->setDelayPerUnit(1 / 10.0f);
    animation->setRestoreOriginalFrame(true);
    _groundHit = Sequence::create(
                MoveBy::create(0, Vec2(0,_screenSize.height * 0.12f)),
                Animate::create(animation),
                CallFuncN::create(CC_CALLBACK_1(GameLayer::animationDone, this)),
                nullptr
                );
   _groundHit->retain();

    animation = Animation::create();
    for(i = 1; i <= 7; i++) {
        auto name = String::createWithFormat("explosion_small%i.png", i);
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(name->getCString());
        animation->addSpriteFrame(frame);
    }

    animation->setDelayPerUnit(0.5 / 7.0f);
    animation->setRestoreOriginalFrame(true);
    _explosion = Sequence::create(
            Animate::create(animation),
            CallFuncN::create(CC_CALLBACK_1(GameLayer::animationDone, this)),
            nullptr
            );

    _explosion->retain();

    //add ufo
    _ufo = Sprite::createWithSpriteFrameName("ufo_1.png");
    animation = Animation::create();
    for(i = 1; i <= 4; i++) {
        auto name = String::createWithFormat("ufo_%i.png", i);
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(name->getCString());
        animation->addSpriteFrame(frame);
    }

    animation->setDelayPerUnit(1.0 / 4.0f);
    animation->setLoops(-1);
    _ufoAnimation = Animate::create(animation);
    _ufoAnimation->retain();

    auto ray = Sprite::createWithSpriteFrameName("ray.png");
    ray->setAnchorPoint(Vec2(0.5, 1.0));
    ray->setPosition(Vec2(_ufo->getBoundingBox().size.width  * 0.52f, _ufo->getBoundingBox().size.height * 0.5f));

    _blinkRay = RepeatForever::create(Blink::create(4, 6));
    _blinkRay->retain();
    _ufo->addChild(ray, -1, kSpriteRay);

    _ufo->setVisible(false);
    _gameBatchNode->addChild(_ufo, 0, kSpriteUfo);

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




