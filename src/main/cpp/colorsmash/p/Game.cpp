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
#include "MMenu.h"
#include "Ende.h"
#include "Game.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(colorsmash)
BEGIN_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
struct CC_DLL GLayer : public f::GameLayer {

  HUDLayer* getHUD() { return (HUDLayer*)getSceneX()->getLayer(3); }

  DECL_PTR(a::NodeList, players)
  DECL_PTR(a::NodeList, shared)
  DECL_PTR(int, tileIndex)

  s_vec<c::Sprite*> tileSprites;
  s_vec<int> tileData;

  STATIC_REIFY_LAYER(GLayer)
  MDECL_DECORATE()
  MDECL_GET_IID(2)

  virtual void onMouseMotion(const c::Vec2&);

  virtual void onTouchMotion(c::Touch*);
  virtual bool onTouchStart(c::Touch*);
  virtual void onTouchEnd(c::Touch*);
  virtual void onInited();

  virtual ~GLayer();
};

//////////////////////////////////////////////////////////////////////////////
//
GLayer::~GLayer() {
  mc_del_arr(tileIndex);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onInited() {
  players = engine->getNodeList(PlayerNode().typeId());
  shared = engine->getNodeList(SharedNode().typeId());
  auto ss= CC_GNLF(GVars, shared, "slots");
  auto wz= cx::visRect();
  auto wb= cx::visBox();

}

//////////////////////////////////////////////////////////////////////////////
void GLayer::decoUI() {

  auto offX= CC_CSV(c::Float, "GAMEPLAY+OFFSET+X");
  auto offY= CC_CSV(c::Float, "GAMEPLAY+OFFSET+Y");
  auto wz= cx::visRect();
  auto wb= cx::visBox();
  auto bg = c::LayerColor::create(
      c::Color4B(25, 0, 51, 255), wz.size.width,wz.size.height);
  addItem(bg);

  // generate vertices for the gameplay frame
  c::Vec2 offset(offX,offY);
  c::Vec2 vs[4];
  vs[0] = c::ccpAdd(offset, c::Vec2(-1, -1));
  vs[1] = c::ccpAdd(offset, c::Vec2(-1, (NUM_ROWS * TILE_SIZE)+1));
  vs[2] = c::ccpAdd(offset, c::Vec2((NUM_COLS * TILE_SIZE)+1, (NUM_ROWS * TILE_SIZE)+1));
  vs[3] = c::ccpAdd(offset, c::Vec2((NUM_COLS * TILE_SIZE)+1, -1));

  // pass vertices, fill colour,
  // border width and border colour to get a nice bordered, coloured rectangle
  auto gamePlayFrame = c::DrawNode::create();
  gamePlayFrame->drawPolygon(
      &vs[0], 4,
      c::Color4F(0.375, 0.375, 0.375, 1), 2, c::Color4F(0.4, 0, 0, 1));
  addItem(gamePlayFrame);

  auto titleLabel = cx::reifyLabel("dft", 52, "Color-Smash");
  titleLabel->setPosition(wb.cx, wb.top * 0.95);
  addItem(titleLabel);

  auto len= NUM_COLS * NUM_ROWS;
  tileIndex= new int[len];
  // generate tile data randomly
  for (auto i = 0;  i < len; ++i) {
    tileData.push_back(1 + floor(cx::rand() * MAX_COLORS));
    tileIndex[i]=i;
  }

  auto spriteBatchNode = c::SpriteBatchNode::create( "pics/tile.png", len + NUM_ROWS);
  addItem(spriteBatchNode);

  auto pi= &tileIndex[0];
  auto n=0;
  F__LOOP(it, tileData) {
    auto s= cx::createSprite("tile");
    auto type = *it;

    s->setColor(getColorForTile(type));
    s->setPosition(getPositionForTile(n));
    s->setUserData(pi);

    ++pi;
    ++n;

    spriteBatchNode->addChild(s);
    tileSprites.push_back(s);
  }

  engine = mc_new(GEngine);
}


//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseMotion(const c::Vec2 &loc) {
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onTouchStart(c::Touch *touch) {
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchMotion(c::Touch *touch) {
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchEnd(c::Touch *touch) {
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

//////////////////////////////////////////////////////////////////////////////
//
Game::Game()
  : f::GameScene(true) {
}

NS_END




