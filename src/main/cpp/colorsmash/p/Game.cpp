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
  void updateScore(const c::Vec2 &);
  void removeTilesWithAnimation();
  void bringDownTiles();
  void addNewTiles();
  void cleanUpAfterMove();
  void onEnd();
  void createTileSprite(int);
  bool onGUI(const c::Vec2 &);
    void findTilesToShift();
    
  DECL_PTR(a::NodeList, players)
  DECL_PTR(a::NodeList, shared)
  DECL_BF(busySignal)

  s_vec<GameTile*> tileShifted;
  s_vec<GameTile*> tileSprites;
  s_vec<int> tileBin;
  s_vec<int> tileData;

  STATIC_REIFY_LAYER(GLayer)
  MDECL_DECORATE()
  MDECL_GET_IID(2)

  virtual void onMouseClick(const c::Vec2&);

  virtual void onTouchMotion(c::Touch*);
  virtual bool onTouchStart(c::Touch*);
  virtual void onTouchEnd(c::Touch*);
  virtual void onInited();

  virtual ~GLayer();
};

//////////////////////////////////////////////////////////////////////////////
//
GLayer::~GLayer() {
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onInited() {
  players = engine->getNodeList(PlayerNode().typeId());
  shared = engine->getNodeList(SharedNode().typeId());
  auto ss= CC_GNLF(GVars, shared, "slots");
  auto wz= cx::visRect();
  auto wb= cx::visBox();

  this->busySignal=true;

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
          this->busySignal=false;
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

  // generate tile data randomly
  auto len= NUM_COLS * NUM_ROWS;
  for (auto i = 0;  i < len; ++i) {
    tileData.push_back(1 + floor(cx::rand() * MAX_COLORS));
  }

  regoAtlas("game-pics");

  auto n=0;
  F__LOOP(it, tileData) {
    auto s= cx::reifySprite("tile.png");
    auto type = *it;
    auto co= mc_new1(GameTile,s);
    auto pos= getPositionForTile(n);

    s->setColor(getColorForTile(type));
    MGML()->addAtlasItem("game-pics", s);

    co->type= type;
    co->index=n;
    ++n;

    co->inflate(pos.x, pos.y);
    tileSprites.push_back(co);
  }

  engine = mc_new(GEngine);
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onTouchStart(c::Touch *touch) {
  return onGUI(touch->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseClick(const c::Vec2 &loc) {
  onGUI(loc);
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onGUI(const c::Vec2 &loc) {
  auto x=CC_CSV(c::Float,"GAMEPLAY+OFFSET+X");
  auto y=CC_CSV(c::Float,"GAMEPLAY+OFFSET+Y");

  if (busySignal) { return false; }

  // calculate touch within the grid
  auto touchWithinGrid = c::ccpSub(loc, c::Vec2(x,y));
  auto col = floor(touchWithinGrid.x / TILE_SIZE);
  auto row = floor(touchWithinGrid.y / TILE_SIZE);
  auto touchedTile = row * NUM_COLS + col;

  // simple bounds checking to ignore touches outside of the grid
  if (col < 0 || col >= NUM_COLS || row < 0 || row >= NUM_ROWS) {
    return false;
  }

  // disable touch so that the subsequent
  // functions have time to execute
  this->busySignal=true;

  findTilesToRemove(tileData, tileBin, col, row, tileData[touchedTile]);
  updateScore(loc);
  removeTilesWithAnimation();
  findTilesToShift();
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::findTilesToShift() {
  // first sort the tiles to be removed, in descending order
  s::sort(tileBin.begin(), tileBin.end(),
      [=](int a, int b) -> bool {
        //return a < b; // asc
        return a > b; // dec
      });

  // for each tile, bring down all the tiles
  // belonging to the same column that are above the current tile
  auto i=0;
  F__LOOP(it, tileBin) {
    auto z= *it;
    auto col = floor(z % NUM_COLS);
    auto row = floor(z / NUM_COLS);
    // iterate through each row above the current tile
    for (auto j = row+1; j < NUM_ROWS; ++j) {
      // each tile gets the data of the tile exactly above it
      auto cjm1= (j-1) * NUM_COLS + col;
      auto cj= j * NUM_COLS + col;
      tileData[cjm1] = tileData[cj];
      // each tile now refers to the sprite of the tile exactly above it
      tileSprites[cjm1] = tileSprites[cj];
      // null checking...this sprite may have already been nullified by removeTilesWithAnimation
      if (tileSprites[cjm1]) {
        // save the new index as user data
        tileSprites[cjm1]->index= cjm1;
        // save this tile's sprite so that it is animated, but only if it hasn't already been saved
          if (s::find(tileShifted.begin(),
                      tileShifted.end(), tileSprites[cjm1]) == tileShifted.end()) {
          tileShifted.push_back(tileSprites[cjm1]);
        }
      }
    }
    // after shifting the whole column down, the tile at the top of the column will be empty
    // set the data to -1...-1 means empty
    tileData[(NUM_ROWS-1) * NUM_COLS + col] = -1;
    // nullify the sprite's reference
    tileSprites[(NUM_ROWS-1) * NUM_COLS + col] = CC_NIL;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::removeTilesWithAnimation() {
  F__LOOP(it, tileBin) {
    auto pos= *it;
    // first clear the tile's data
    tileData[pos] = E_COLOR_NONE;
    // the tile should scale down with easing and then remove itself
    auto ptr= tileSprites[pos];
    ptr->node->runAction(
        c::Sequence::create(
          c::EaseBackIn::create(c::ScaleTo::create(0.25, 0.0)),
          c::RemoveSelf::create(true),
          c::CallFunc::create([=]() {
            delete ptr;
            }),
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
  F__LOOP(it, tileShifted) {
    auto t= *it;
    auto id = t->index;
    // the tiles should move to their new positions with an awesome looking bounce
    t->node->runAction(
        c::EaseBounceOut::create(
          c::MoveTo::create(0.25, getPositionForTile(id))));
  }
    // wait for the movement to finish then add new tiles
  this->runAction(
      c::Sequence::create(
        c::DelayTime::create(0.25),
        c::CallFunc::create([=]() {
          this->addNewTiles();
          }),CC_NIL));
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
    tileData[pos] = 1 + floor(cx::rand() * MAX_COLORS);
    // create tile sprite based on tile data
    createTileSprite(pos);
  }

  // animate the entry of the sprites
  F__LOOP(it, empty) {
    auto pos = *it;
    // set the scale to 0
    tileSprites[pos]->node->setScale(0);
    // scale the sprite up with a neat easing effect
    tileSprites[pos]->node->runAction(
        c::EaseBackOut::create(c::ScaleTo::create(0.125, 1)));
  }

  // the move has finally finished, do some cleanup
  cleanUpAfterMove();
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::cleanUpAfterMove() {
  // empty the arrays
  tileShifted.clear();
  tileBin.clear();
  this->busySignal= false;
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::createTileSprite(int tileId) {
  auto s= cx::reifySprite("tile.png");
  auto co= mc_new1(GameTile,s);
  auto t= tileData[tileId];
  auto pos= getPositionForTile(tileId);

  s->setColor(getColorForTile(t));
  co->index= tileId;
  co->type= t;
  tileSprites[tileId] = co;
  MGML()->addAtlasItem("game-pics", s);
  co->inflate(pos.x, pos.y);
}


//////////////////////////////////////////////////////////////////////////////
//
void GLayer::updateScore(const c::Vec2 &point) {

  // count the number of tiles the user just removed
  auto numTiles= tileBin.size();
  // calculate score for this move
  auto scoreToAdd = numTiles * SCORE_PER_TILE;
  auto cfg= MGMS()->getLCfg()->getValue();
  auto bonus= cfg["BONUS"];

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

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onEnd() {
  this->setOpacity(255 * 0.1);
  MGMS()->stop();
  surcease();
  getHUD()->pause();
  //Ende::reify(MGMS(),4);
}

END_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic &topic, void *m) {
  auto y= (GLayer*) getGLayer();

  if ("/game/hud/updatetimer" == topic) {
    auto msg= (j::json*)m;
    auto v=JS_INT(msg->operator[]("time"));
    y->getHUD()->updateTimer(v);
  }
  if ("/game/hud/redalert" == topic) {
    y->getHUD()->flashAlert();
  }
  if ("/game/hud/earnscore" == topic) {
    auto msg= (j::json*)m;
    auto v=JS_INT(msg->operator[]("score"));
    y->getHUD()->updateScore(v);
  }
  if ("/game/player/lose" == topic) {
    y->onEnd();
  }


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




