// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Kenneth Leung. All rights reserved.

#include "core/XConfig.h"
#include "core/CCSX.h"
#include "s/GEngine.h"
#include "HUD.h"
#include "MMenu.h"
#include "Ende.h"
#include "Game.h"
#include "n/C.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(colorsmash)
BEGIN_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
struct CC_DLL GLayer : public f::GameLayer {

  HUDLayer* getHUD() {
    return (HUDLayer*)getSceneX()->getLayer(3); }
  void updateScore(const CCT_PT&);
  void removeTilesWithAnimation();
  void bringDownTiles();
  void addNewTiles();
  void cleanUpAfterMove();
  void onEnd();
  void createTileSprite(int);
  bool onGUI(const CCT_PT&);
  void findTilesToShift();

  __decl_ptr(ecs::Node, _shared)
  __decl_bf(_busySignal)

  __decl_vec(GameTile*, _tileShifted)
  __decl_vec(GameTile*, _tileSprites)
  __decl_vec(int, _tileBin)
  __decl_vec(int, _tileData)

  __decl_create_layer(GLayer)
  __decl_deco_ui()
  __decl_get_iid(2)

  virtual bool onMouseStart(const CCT_PT&);
  virtual bool onTouchStart(c::Touch*);
  virtual void onInited();

  virtual ~GLayer() {}

};

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onInited() {
  _shared = _engine->getNodes("n/GVars")[0];
  auto ss= CC_GEC(GVars,_shared,"n/GVars");
  auto wz= cx::visSize();
  auto wb= cx::visBox();

  this->_busySignal=true;

  s_vec<c::Label*> labels;
  for (auto i = 0; i < 4; ++i) {
    auto b= cx::reifyBmfLabel("dft","0");
    auto k= XCFG()->scaleBmfont(b,52);
    CC_POS2(b, wb.cx, wb.cy);
    b->setOpacity(0);
    b->setScale(3*k);
    addItem(b);
    labels.push_back(b);
  }

  labels[3]->setString("Start");
  labels[2]->setString("1");
  labels[1]->setString("2");
  labels[0]->setString("3");

  for (auto i = 0; i < 4; ++i) {
      auto k= labels[i]->getScale();
      // since the labels should appear one after the other,
      // we give them increasing delays before they appear
    auto countdownAnimation = c::Sequence::create(
        c::DelayTime::create(i),
        c::Spawn::create( c::FadeIn::create(0.25),
          c::EaseBackOut::create(c::ScaleTo::create(0.25, k)),CC_NIL),
        c::DelayTime::create(0.75),
        c::RemoveSelf::create(true),
        CC_NIL);
    labels[i]->runAction(countdownAnimation);
  }

  // after the animation has finished, start the game
  this->runAction(c::Sequence::create(
        c::DelayTime::create(4),
        c::CallFunc::create([=]() {
          this->_busySignal=false;
          }),
        CC_NIL));
}

//////////////////////////////////////////////////////////////////////////////
void GLayer::decoUI() {

  auto offX= CC_CSV(c::Float, "GAMEPLAY+OFFSET+X");
  auto offY= CC_CSV(c::Float, "GAMEPLAY+OFFSET+Y");
  auto wz= cx::visSize();
  auto wb= cx::visBox();
  auto bg = c::LayerColor::create(
      c::Color4B(25, 0, 51, 255),
      CC_ZW(wz),
      CC_ZH(wz));

  addItem(bg,-2);

  // generate vertices for the gameplay frame
  CCT_PT offset(offX,offY);
  CCT_PT vs[4];
  vs[0] = c::ccpAdd(offset, CCT_PT(-1, -1));
  vs[1] = c::ccpAdd(offset, CCT_PT(-1, (NUM_ROWS * TILE_SIZE)+1));
  vs[2] = c::ccpAdd(offset, CCT_PT((NUM_COLS * TILE_SIZE)+1, (NUM_ROWS * TILE_SIZE)+1));
  vs[3] = c::ccpAdd(offset, CCT_PT((NUM_COLS * TILE_SIZE)+1, -1));

  // pass vertices, fill colour,
  // border width and border colour to get a nice bordered, coloured rectangle
  auto gamePlayFrame = c::DrawNode::create();
  gamePlayFrame->drawPolygon(
      &vs[0], 4,
      c::Color4F(0.375, 0.375, 0.375, 1), 2, c::Color4F(0.4, 0, 0, 1));
  addItem(gamePlayFrame);

  auto titleLabel = cx::reifyBmfLabel("title", "Color-Smash");
  XCFG()->scaleBmfont(titleLabel,64);
  CC_POS2(titleLabel, wb.cx, wb.top * 0.95);
  addItem(titleLabel);

  // generate tile data randomly
  auto len= NUM_COLS * NUM_ROWS;
  for (auto i = 0;  i < len; ++i) {
    _tileData.push_back(1 + cx::randInt(MAX_COLORS));
  }

  regoAtlas("game-pics",1);

  auto n=0;
  F__LOOP(it, _tileData) {
    auto pos= getPositionForTile(n);
    auto &type = *it;
    auto co= GameTile::create(type,n);
    MGML()->addAtlasItem("game-pics", co);
    CC_POS1(co, pos);
    _tileSprites.push_back(co);
    ++n;
  }

  _engine = mc_new(GEngine);
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onTouchStart(c::Touch *touch) {
  return onMouseStart(touch->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onMouseStart(const CCT_PT &loc) {
  auto x=CC_CSV(c::Float,"GAMEPLAY+OFFSET+X");
  auto y=CC_CSV(c::Float,"GAMEPLAY+OFFSET+Y");

  if (_busySignal) { return false; }

  // calculate touch within the grid
  auto touchWithinGrid = c::ccpSub(loc, CCT_PT(x,y));
  auto col = (int) floor(touchWithinGrid.x / TILE_SIZE);
  auto row = (int) floor(touchWithinGrid.y / TILE_SIZE);
  auto touchedTile = row * NUM_COLS + col;

  // simple bounds checking to ignore touches outside of the grid
  if (col < 0 || col >= NUM_COLS || row < 0 || row >= NUM_ROWS) {
  return false; }

  // disable touch so that the subsequent
  // functions have time to execute
  this->_busySignal=true;

  findTilesToRemove(_tileData, _tileBin, col, row, _tileData[touchedTile]);
  updateScore(loc);
  removeTilesWithAnimation();
  findTilesToShift();

  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::findTilesToShift() {
  // first sort the tiles to be removed, in descending order
  s::sort(_tileBin.begin(),_tileBin.end(),
      [=](int a, int b) -> bool {
        //return a < b; // asc
        return a > b; // dec
      });

  // for each tile, bring down all the tiles
  // belonging to the same column that are above the current tile
  auto i=0;
  F__LOOP(it, _tileBin) {
    auto &z= *it;
    auto col = (int) floor(z % NUM_COLS);
    auto row = (int) floor(z / NUM_COLS);
    // iterate through each row above the current tile
    for (auto j = row+1; j < NUM_ROWS; ++j) {
      // each tile gets the data of the tile exactly above it
      auto cjm1= (j-1) * NUM_COLS + col;
      auto cj= j * NUM_COLS + col;
      _tileData[cjm1] = _tileData[cj];
      // each tile now refers to the sprite of the tile exactly above it
      _tileSprites[cjm1] = _tileSprites[cj];
      // null checking...this sprite may have already been nullified by removeTilesWithAnimation
      if (_tileSprites[cjm1]) {
        // save the new index as user data
        _tileSprites[cjm1]->setIndex( cjm1);
        // save this tile's sprite so that it is animated, but only if it hasn't already been saved
        if (s::find(_tileShifted.begin(),
                    _tileShifted.end(), _tileSprites[cjm1]) == _tileShifted.end()) {
          _tileShifted.push_back(_tileSprites[cjm1]);
        }
      }
    }
    // after shifting the whole column down, the tile at the top of the column will be empty
    // set the data to -1...-1 means empty
    _tileData[(NUM_ROWS-1) * NUM_COLS + col] = -1;
    // nullify the sprite's reference
    _tileSprites[(NUM_ROWS-1) * NUM_COLS + col] = CC_NIL;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::removeTilesWithAnimation() {
  F__LOOP(it, _tileBin) {
    auto &pos= *it;
    // first clear the tile's data
    _tileData[pos] = E_COLOR_NONE;
    // the tile should scale down with easing and then remove itself
    auto ptr= _tileSprites[pos];
    ptr->runAction(
        c::Sequence::create(
          c::EaseBackIn::create(c::ScaleTo::create(0.25, 0.0)),
          c::RemoveSelf::create(true),
          CC_NIL));
    // nullify the tile's sprite
    _tileSprites[pos] = CC_NIL;
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
  F__LOOP(it, _tileShifted) {
    auto &t= *it;
    auto id = t->getIndex();
    // the tiles should move to their new positions with an awesome looking bounce
    t->runAction(
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
  F__LOOP(it, _tileData) {
    auto &v= *it;
    if (v < 0) { empty.push_back(n); }
    ++n;
  }

  // now create tile data and sprites
  F__LOOP(it, empty) {
    auto &pos= *it;
    // generate tile data randomly
    _tileData[pos] = 1 + cx::randInt(MAX_COLORS);
    // create tile sprite based on tile data
    createTileSprite(pos);
  }

  // animate the entry of the sprites
  F__LOOP(it, empty) {
    auto &pos = *it;
    auto k= _tileSprites[pos]->getScale();
    // set the scale to 0
    _tileSprites[pos]->setScale(0);
    // scale the sprite up with a neat easing effect
    _tileSprites[pos]->runAction(
        c::EaseBackOut::create(c::ScaleTo::create(0.125, k)));
  }

  // the move has finally finished, do some cleanup
  cleanUpAfterMove();
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::cleanUpAfterMove() {
  // empty the arrays
  _tileShifted.clear();
  _tileBin.clear();
  this->_busySignal= false;
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::createTileSprite(int tileId) {
  auto pos= getPositionForTile(tileId);
  auto t= _tileData[tileId];
  auto co= GameTile::create(t, tileId);
  _tileSprites[tileId] = co;
  MGML()->addAtlasItem("game-pics", co);
  CC_POS1(co, pos);
}


//////////////////////////////////////////////////////////////////////////////
//
void GLayer::updateScore(const CCT_PT &point) {

  // count the number of tiles the user just removed
  auto numTiles= _tileBin.size();
  // calculate score for this move
  auto scoreToAdd = numTiles * SCORE_PER_TILE;
  auto cfg= MGMS()->getLCfg()->getValue();
  auto bonus= cfg["BONUS"];

  J__LOOP(it,bonus) {
    auto &z= *it;
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
void GLayer::onEnd() {
  getHUD()->pause();
  MGMS()->stop();
  surcease();
  Ende::reify(MGMS(),4);
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
  if ("/game/hud/redzone" == topic) {
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


NS_END




