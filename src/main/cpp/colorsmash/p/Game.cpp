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
#include "n/lib.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(colorsmash)
BEGIN_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
struct CC_DLL GLayer : public f::GameLayer {

  HUDLayer* getHUD() { return (HUDLayer*)getSceneX()->getLayer(3); }
  void updateScore(const c::Vec2 &point, FArrInt*);
  void removeTilesWithAnimation(FArrInt*);
  void bringDownTiles();
  void addNewTiles();
  void cleanUpAfterMove();

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

  ss->countDown=true;

    s_vec<c::Label*> labels;
  for (auto i = 0; i < 4; ++i) {
    auto b= cx::reifyLabel("dft",52,"");
    b->setPosition(wb.cx, wb.cy);
    b->setOpacity(0);
    b->setScale(3);
    addItem(b);
    labels.push_back(b);
  }

  labels[0]->setString("3");
  labels[1]->setString("2");
  labels[2]->setString("1");
  labels[3]->setString("Start");

  for (auto i = 0; i < 4; ++i) {
      // since the labels should appear one after the other,
      // we give them increasing delays before they appear
    auto countdownAnimation = c::Sequence::create(
        c::DelayTime::create(i),
        c::Spawn::create( c::FadeIn::create(0.25),
          c::EaseBackOut::create(c::ScaleTo::create(0.25, 1)),CC_NIL),
        c::DelayTime::create(0.75),
        c::RemoveSelf::create(true),
        CC_NIL);
    labels[i]->runAction(countdownAnimation);
  }

  // after the animation has finished, start the game
  this->runAction(c::Sequence::create(
        c::DelayTime::create(4),
        c::CallFunc::create([=]() {
          ss->countDown=false;
          }),
        CC_NIL));
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
  auto x=CC_CSV(c::Float,"GAMEPLAY+OFFSET+X");
  auto y=CC_CSV(c::Float,"GAMEPLAY+OFFSET+Y");
  auto loc= touch->getLocation();
  // calculate touch within the grid
  auto touchWithinGrid = c::ccpSub(loc, c::Vec2(x,y));
  auto col = floor(touchWithinGrid.x / TILE_SIZE);
  auto row = floor(touchWithinGrid.y / TILE_SIZE);
  auto touchedTile = row * NUM_COLS + col;

  // simple bounds checking to ignore touches outside of the grid
  if (col < 0 || col >= NUM_COLS || row < 0 || row >= NUM_ROWS) {
    return false;
  }

  // disable touch so that the subsequent functions have time to execute
  this.setTouchEnabled(false);
  FArrInt garbo(tileData.size());
  garbo.fill(-1);
  findTilesToRemove(tileData, &garbo, col, row, tileData[touchedTile]);
  updateScore(touch, &garbo);
  removeTilesWithAnimation(&garbo);
  this.findTilesToShift();
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::removeTilesWithAnimation(FArrInt *garbo) {
  for (auto n=0; n < garbo->size(); ++n) {
    auto pos= garbo->get(n);
    if (pos < 0) { continue; }
    // first clear the tile's data
    tileData[pos] = E_COLOUR_NONE;
    // the tile should scale down with easing and then remove itself
    tileSprites[pos]->runAction(
        c::Sequence::create(
          c::EaseBackIn::create(c::ScaleTo::create(0.25, 0.0)),
          c::RemoveSelf::create(true),
          CC_NIL));
    // nullify the tile's sprite
    tileSprites[pos] = CC_NIL;
  }
  // wait for the scale down animation to finish then bring down the tiles from above
  this->runAction(
      c::Sequence::create(
        c::DelayTime::create(0.25),
        c::CallFunc::create([=]() {
          this->bringDownTiles();
          }),
        CC_NIL));
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::bringDownTiles() {
  F__LOOP(it, tilesToShift) {
    auto t= *it;
    auto id = t->getUserData();
    // the tiles should move to their new positions with an awesome looking bounce
    t->runAction(
        c::EaseBounceOut::create(
          c::MoveTo::create(0.25, getPositionForTile(*id))));
  }
    // wait for the movement to finish then add new tiles
  this->runAction(
      c::Sequence::create(
        c::DelayTime::create(0.25),
        c::CallFunc::create([=]() {
          this->addNewTiles();
          })));
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::addNewTiles() {
  // first search for all tiles having value -1...-1 means empty
  s_vec<int> empty;
  auto n = 0;
  F__LOOP(it, tileData) {
    auto v= *it;
    if (v < 0) { empty.push_back(n); }
    ++n;
  }

  // now create tile data and sprites
  F__LOOP(it, empty) {
    auto pos= *it;
    // generate tile data randomly
    tileData[pos] = 1 + floor(cx::rand() * MAX_COLOURS);
    // create tile sprite based on tile data
    createTileSprite(pos);
  }

  // animate the entry of the sprites
  F__LOOP(it, empty) {
    auto pos = *it;
    // set the scale to 0
    tileSprites[pos]->setScale(0);
    // scale the sprite up with a neat easing effect
    tileSprites[pos]->runAction(
        c::EaseBackOut::create(c::ScaleTo::create(0.125, 1)));
  }

  // the move has finally finished, do some cleanup
  cleanUpAfterMove();
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::cleanUpAfterMove() {
  // empty the arrays
  this.tilesToShift = [];
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::updateScore(const c::Vec2 &point, FArrInt *garbo) {

  // count the number of tiles the user just removed
  auto numTiles=0;
  for (auto i=0; i < garbo->size(); ++i) {
    if (garbo->get(i) >= 0) {
      ++numTiles;
    }
  }
  // calculate score for this move
  auto scoreToAdd = numTiles * SCORE_PER_TILE;
  auto cfg= MGMS()->getLCfg();
  auto bonus= JS_ARR(cfg["BONUS"]);

  J__LOOP(it,bonus) {
    auto z= *it;
    auto n=JS_INT(z);
    if (numTiles >= n) {
        // add the bonus to the score for this move
        scoreToAdd += n * 20;
        break;
    }
  }

  getHUD()->updateScore(scoreToAdd);
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




