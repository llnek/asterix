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
NS_BEGIN(stoneage)
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
}

//////////////////////////////////////////////////////////////////////////////
void GLayer::decorate() {

  self.schedulerID = nil
  self.grid = {}
  self.gridController = nil
  self.gridAnimations = nil
  self.objectPools = nil
  self.gridGemsColumnMap = {}
  self.allGems = {}
  self.gemsContainer = cc.Node:create()
  self.selectedGem = nil
  self.targetGem = nil
  self.selectedIndex = {x = 0, y = 0}
  self.targetIndex = {x = 0, y = 0}
  self.selectedGemPosition = {x = 0, y = 0}
  self.combos = 0
  self.addingCombos = false
  self.scoreLabel = nil
  self.diamondScoreLabel = nil
  self.diamondScore = 0
  self.gemsScore = 0
  self.running = true
  self:addTouchEvents()
  self:init()
  self:buildGrid()

    self.gridController = GridController:create()
    self.gridAnimations = GridAnimations:create()
    self.objectPools = ObjectPools.create()

    self.gridAnimations:setGameLayer(self)
    self.gridController:setGameLayer(self)
    self.objectPools:createPools(self)

    self:addChild( self.gemsContainer )
    self.gemsContainer:setPosition( 25, 80)

    --build interface
    local frame = cc.Sprite:create("frame.png")
    frame:setPosition(self.middle.x, self.middle.y)
    self:addChild(frame)

    local diamondScoreBg = cc.Sprite:create("diamondScore.png")
    diamondScoreBg:setPosition(100, constants.SCREEN_HEIGHT - 30)
    self:addChild(diamondScoreBg)

    local scoreBg = cc.Sprite:create("gemsScore.png")
    scoreBg:setPosition(280, constants.SCREEN_HEIGHT - 30)
    self:addChild(scoreBg)

    local ttfConfig = {}
    ttfConfig.fontFilePath="fonts/myriad-pro.ttf"
    ttfConfig.fontSize=20

    self.diamondScoreLabel = cc.Label:createWithTTF(ttfConfig, "0", cc.TEXT_ALIGNMENT_RIGHT , 150)
    self.diamondScoreLabel:setPosition (140, constants.SCREEN_HEIGHT - 30)
    self:addChild(self.diamondScoreLabel)

    self.scoreLabel = cc.Label:createWithTTF(ttfConfig, "0", cc.TEXT_ALIGNMENT_RIGHT , 150)
    self.scoreLabel:setPosition (330, constants.SCREEN_HEIGHT - 30)
    self:addChild(self.scoreLabel)


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




