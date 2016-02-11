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
NS_BEGIN(victorian)
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
};

//////////////////////////////////////////////////////////////////////////////
void GLayer::postReify() {
  shared = engine->getNodeList(SharedNode().typeId());
  auto pool= MGMS()->getPool("Clouds");
  auto wb=cx::visBox();

  //add clouds
  for (auto i = 0; i < 4; ++i) {
    auto cy = wb.top * (i % 2 == 0 ? 0.7f : * 0.8f);
    auto c = cx::reifySprite("cloud.png");
    c->setPosition(wb.right * 0.15f + i * wb.right * 0.25f, cy);
    addAtlasItem("game-pics",c, kBackground);
    pool->checkin(mc_new1(Cloud,c));
  }

  auto terrain = Terrain::create();
  addAtlasItem("game-pics",terrain, kMiddleground);
  auto player = Player::create();
  addAtlasItem("game-pics",player, kBackground);
}

//////////////////////////////////////////////////////////////////////////////
void GLayer::decorate() {
  centerImage("game-bg");
  regoAtlas("game-pics");

  auto background = cx::reifySprite("background.png");
  background->setAnchorPoint(cx::anchorLB());
  addAtlasItem("game-pics",background, kBackground);

  auto repeat = cx::reifySprite("background.png");
  auto rw= repeat->getContentSize().width;
  repeat->setAnchorPoint(cx::anchorLB());
  repeat->setPosition(rw - 1, 0);
  background->addChild(repeat, kBackground);

  repeat = cx::reifySprite("background.png");
  repeat->setAnchorPoint(cx::anchorLB());
  repeat->setPosition(2 * (rw - 1), 0);
  background->addChild(repeat, kBackground);

  auto foreground = cx::reifySprite("lamp.png");
  foreground->setAnchorPoint(cx::anchorLB());
  addAtlasItem("game-pics",foreground, kForeground);

  repeat = cx::reifySprite("lamp.png");
  repeat->setAnchorPoint(cx::anchorLB());
  repeat->setPosition(rw * 4, 0);
  foreground->addChild(repeat, kBackground);

  repeat = cx::reifySprite("lamp.png");
  repeat->setAnchorPoint(cx::anchorLB());
  repeat->setPosition(rw * 8, 0);
  foreground->addChild(repeat, kBackground);

  engine = mc_new(GEngine);
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




