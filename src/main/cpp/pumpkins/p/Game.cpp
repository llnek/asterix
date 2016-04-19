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
#include "Popups.h"
#include "TowerMenu.h"
#include "GestureLayer.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(pumpkins)
BEGIN_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
struct CC_DLL GLayer : public f::GameLayer {

  HUDLayer* getHUD() { return (HUDLayer*)getSceneX()->getLayer(3); }

  DECL_PTR(ecs::Node, _shared)

  time_scale_ = 0.0f;
  tiled_map_ = NULL;
  tmx_layer_ = NULL;
  num_enemy_walk_points_ = 0;
  enemy_walk_points_.clear();
  num_towers_ = 0;
  towers_.clear();
  num_waves_ = 0;
  curr_wave_index_ = 0;
  _wave_starting_ = false;
  curr_wave_ = NULL;
  waves_.clear();
  waves_label_ = NULL;
  cash_ = 0;
  cash_label_ = NULL;
  lives_left_ = 0;
  pumpkin_position_ = CCPointZero;
  pumpkin_ = NULL;
  lives_label_ = NULL;
  tower_menu_ = NULL;
  grid_node_ = NULL;
  hud_menu_ = NULL;
  gesture_layer_ = NULL;
  is_popup_active_ = false;

  STATIC_REIFY_LAYER(GLayer)
  MDECL_DECORATE()
  MDECL_GET_IID(2)

  virtual void onMouseMotion(const c::Vec2&);
  virtual bool onMouseStart(const c::Vec2&);
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

  _shared= _engine->getNodes("n/GVars")[0];

  auto ss= CC_GEC(GVars,_shared,"n/GVars");
  auto wz= cx::visRect();
  auto wb= cx::visBox();

  ss->levelNumber = 0;

  _currWaveIndex = -1;
  _timeScale = 1;
  _livesLeft = 20;

  createTiledMap();
  createGrid();
  createWalkPoints();
  createWaves();
  createPumpkin();

  // create & add the tower placement & maaintenance menus
  _towerMenu = TowerMenu::create(this);
  addChild(_towerMenu, E_LAYER_HUD + 1);

  // create & add the gesture layer
  _gestureLayer = GestureLayer::create(this,
      callfuncO_selector(GLayer::onGestureReceived));
  addChild(_gestureLayer);

  // start the first wave in a few seconds
  scheduleOnce(schedule_selector(GLayer::startNextWave), 2);
  scheduleUpdate();

  // reset the scheduler's time scale
  CC_DTOR()->getScheduler()->setTimeScale(_timeScale);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseClick(const c::Vec2 &loc) {
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseMotion(const c::Vec2 &loc) {
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onMouseStart(const c::Vec2 &loc) {
  return true;
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

//////////////////////////////////////////////////////////////////////////////
void GLayer::decoUI() {
  centerImage("game.bg", E_LAYER_BACKGROUND);
  _engine = mc_new(GEngine);
  cx::sfxMusic("background", true);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::createTiledMap() {
  auto level= ss->levelNumber + 1;
  auto fn = "level_" + (level < 10 ? "0" : "") + FTOS(level) + ".tmx";
  _tiledMap = c::TMXTiledMap::create(fn);
  addChild(_tiledMap, E_LAYER_BACKGROUND+1);

  auto sz= CC_CSIZE(_tiledMap);
  CC_POS2(_tiledMap, wb.cx - HWZ(sz), wb.cy - HHZ(sz));
  _tmxLayer = _tiledMap->layerNamed("EnemyPath");
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::createGrid() {
  // create a node to draw the grid
  _gridNode = c::DrawNode::create();
  CC_HIDE(_gridNode);
  // position it relative to the tiled map
  CC_POS1(_gridNode,
      c::ccpAdd(_tiledMap->getPosition(), CCT_PT(0, 100)));
  addChild(_gridNode, E_LAYER_BACKGROUND);

  auto map_size = _tiledMap->getMapSize();
  auto counter = 0;
  // run a loop to alternatively draw a semi-transparent black square indicating a tile
  for (auto i = 0; i < ((int)map_size.width); ++i) {
    for(auto j = 0; j < ((int)map_size.height); ++j) {
      ++counter;
      if ((i % 2 == 0 && counter % 2 == 0) ||
          (i % 2 != 0 && counter % 2 != 0) ) {
        continue;
      }
      CCT_PT vertices[4] = {
        CCT_PT( i * TILE_SIZE + 2, wb.top - j * TILE_SIZE - 2 ),
        CCT_PT((i+1) * TILE_SIZE - 2, wb.top - j * TILE_SIZE - 2 ),
        CCT_PT((i+1) * TILE_SIZE - 2, wb.top - (j+1) * TILE_SIZE + 2 ),
        CCT_PT(i * TILE_SIZE + 2, wb.top - (j+1) * TILE_SIZE + 2 )
      };
      _gridNode->drawPolygon(vertices, 4,
          c::ccc4f(0.0f, 0.0f, 0.0f, 0.12f), 0,
          c::ccc4f(0.0f, 0.0f, 0.0f, 0.0f));
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::createWalkPoints() {
  // parse the list of objects
  auto object_group = _tiledMap->objectGroupNamed("PathObjects");
  auto objects = object_group->getObjects();
  auto num_objects = objects->count();
  CCDictionary *object = CC_NIL;

  for (auto i = 0; i < num_objects; ++i) {
    object = (CCDictionary*) objects->objectAtIndex(i);
    // save each WalkPoint's position for enemies to use
    if (strstr(object->valueForKey("name")->getCString(), "WalkPoint") != CC_NIL) {
      _enemyWalkPoints.push_back(
          CCT_PT(object->valueForKey("x")->floatValue(), object->valueForKey("y")->floatValue()));
    }
  }
  _numEnemyWalkPoints = _enemyWalkPoints.size();
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::createWaves() {
  auto level= ss->levelNumber + 1;
  auto fn= "level_" + (level< 10 ? "0" : "") + FTOS(level) + ".xml";
  unsigned long size;
  auto data = (char*)CC_FILER()->getFileData(fn, "rb", &size);

  // parse file
  tinyxml2::XMLDocument xml_document;
  tinyxml2::XMLError xml_result = xml_document.Parse(data, size);

  CC_SAFE_DELETE(data);

  // print the error if parsing was unsuccessful
  if (xml_result != tinyxml2::XML_SUCCESS) {
    CCLOGERROR("Error:%d while reading %s", xml_result, buf);
    return;
  }

  tinyxml2::XMLNode *level_node = xml_document.FirstChild();
  // save the initial cash for this level
  _cash = level_node->ToElement()->IntAttribute("cash");

  tinyxml2::XMLElement *wave_element = CC_NIL;
  // loop through each Wave tag
  for (tinyxml2::XMLNode *wave_node = level_node->FirstChild(); wave_node != CC_NIL; wave_node = wave_node->NextSibling()) {
    wave_element = wave_node->ToElement();
    // get list of enemy indices
    s_vec<int> enemy_list = getIntListFromString(sstr(wave_element->Attribute("enemy_list")));
    // createa a new Wave object
    Wave wave;
    // save the spawn delay & list of enemies for this wave
    wave.numEnemies = enemy_list.size();
    wave.spawnDelay = wave_element->FloatAttribute("spawn_delay");
    // create all enemies in advance
    for (auto i = 0; i < wave->numEnemies; ++i) {
      auto enemy = Enemy::create(enemy_list[i]);
      wave.enemies.push_back(enemy);
      addAtlasItem("game-pics", enemy, E_LAYER_ENEMY);
    }

    _waves.push_back(wave);
    ++_numWaves;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::createPumpkin() {
  // fetch the Pumpkin object from the tiled map
  auto object_group = _tiledMap->objectGroupNamed("PathObjects");
  auto pumpkin_object = object_group->objectNamed("Pumpkin");

  if (E_NIL(pumpkin_object)) { return; }

  // create the sprite for the pumpkin
  _pumpkin = cx::reifySprite("TD_pumkin_01.png");
  CC_POS2(_pumpkin, pumpkin_object->valueForKey("x")->floatValue(), pumpkin_object->valueForKey("y")->floatValue());
  addAtlasItem("game-pics", _pumpkin, E_LAYER_TOWER);

  // create the sprite for the lives
  auto life_frame = cx::reifySprite("TD_livebg.png");
  auto psz= CC_CSIZE(_pumpkin);
  CC_POS2(life_frame, _pumpkin->getPositionX() -
      HWZ(psz), _pumpkin->getPositionY() +
      HHZ(psz));
  addAtlasItem("game-pics", life_frame, E_LAYER_TOWER);

  // create the label for the lives
  lives_label_ = cx::reifyBmfLabel("dft",
    "" + (_livesLeft < 10 ? "0" : "") + FTOS(_livesLeft));
  XCFG()->scaleBmfont(_livesLabel, 32);
  _livesLabel->setAnchorPoint(CCT_PT(0.15, 0.5));
  _livesLabel->setColor(c::ccc3(255, 255, 189));
  CC_POS1(_livesLabel, life_frame->getPosition());
  addItem(_livesLabel, E_LAYER_TOWER);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::placeTower(int type, const CCT_PT &position) {
  // can the player afford this tower?
  if (_cash < ss->towerDataSets[type].towerData[0].cost) {
    return;
  }

  // create a new Tower object & add it into the vector of towers
  auto tower = Tower::create(type, position);
  addAtlasItem("game-pics", tower, E_LAYER_TOWER);
  ++_numTowers;
  _towers.push_back(tower);

  // save tower's information into the tile map
  auto pos= _tiledMap->convertToNodeSpace(position);
  auto tile_coord = CCT_PT(GET_COL_FOR_X(pos.x), GET_ROW_FOR_Y(pos.y, MAX_ROWS));
  _tmxLayer->setTileGID(TOWER_GID + (_numTowers - 1), tile_coord);

  // debit cash
  updateCash(- tower->getCost());
  getHUD()->updateLabels();
  // show the range for this tower
  tower->showRange();

  // hide the grid now that the tower has been placed
  CC_HIDE(_gridNode);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::sellTower(int index) {
  // calculate the net worth of this tower
  auto total_cost = 0;
  for (auto i = 0; i <= _towers[index]->getLevel(); ++i) {
    total_cost +=
      ss->towerDataSets[_towers[index]->getType()].towerData[i].cost;
  }
  // credit cash
  updateCash((int)(total_cost / 2));

  // erase tower's information from the tile map
  auto position = _tiledMap->convertToNodeSpace(_towers[index]->getPosition());
  auto tile_coord = CCT_PT(GET_COL_FOR_X(position.x), GET_ROW_FOR_Y(position.y, MAX_ROWS));
  _tmxLayer->setTileGID(0, tile_coord);

  // sell the tower & erase it from the vector
  _towers[index]->sell();
  _towers.erase(_towers.begin() + index);
  --_numTowers;
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::startNextWave(float dt) {
  ++_currWaveIndex;
  // are there any waves left?
  if (_currWaveIndex >= _numWaves) {
    // level has completed
    gameOver(true);
  } else {
    // start the next wave in a few seconds
    _currWave = _waves[_currWaveIndex];
    schedule(schedule_selector(GLayer::spawnEnemy), _currWave.spawnDelay);
    getHUD()->updateLabels();
    scaleLabel(_wavesLabel);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::spawnEnemy(float dt) {
  // have all enemies of this wave been spawned?
  if (_currWave.numEnemiesSpawned >= _currWave.numEnemies) {
    // wave has finished starting
    _waveStarting = false;
    // stop spawning enemies
    unschedule(schedule_selector(GLayer::spawnEnemy));
    return;
  }
  // fetch the next enemy in the list
  auto enemy = _currWave.enemies[_currWave.numEnemiesSpawned++];
  // tell the enemy where to go
  enemy->setWalkPoints(_numEnemyWalkPoints, _enemyWalkPoints);
  // tell the enemy to start walking
  enemy->startWalking();
  ++_currWave.numEnemiesWalking;
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::enemyAtTheGates(Enemy *enemy) {
  // this is called when the enemy has reached the pumpkin
  reduceLives(enemy->getDamage());
  --_currWave.numEnemiesWalking;
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::enemyDown(Enemy *enemy) {
  // this is called when the enemy
  updateCash(enemy->getReward());
  --_currWave.numEnemiesWalking;
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::reduceLives(int amount) {
  // reduce lives
  --_livesLeft;
  switch (_livesLeft) {
  // change the pumpkin's appearance based on how many lives are left
  case 15:
    _pumpkin->setSpriteFrame("TD_pumkin_02.png");
    break;
  case 10:
    _pumpkin->setSpriteFrame("TD_pumkin_03.png");
    break;
  case 5:
    _pumpkin->setSpriteFrame("TD_pumkin_04.png");
    break;
  case 0:
    // all lives over...level failed
    gameOver(false);
    break;
  }

  // update the lives label
  _livesLabel->setString(FTOS(_livesLeft));
  scaleLabel(_livesLabel);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::updateCash(int amount) {
  _cash += amount;
  getHUD()->updateLabels();
  scaleLabel(_cashLabel);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::update(float dt) {

  checkWaveCompletion();

  for (auto i = 0; i < _numTowers; ++i) {
    _towers[i]->update();
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::checkWaveCompletion() {
  // wave has completed when all enemies have been spawned AND
  // when there are no enemies walking (cuz they're all dead!)
  if (!_waveStarting &&
      _currWave &&
      _currWave.numEnemiesSpawned >= _currWave.numEnemies &&
      _currWave.numEnemiesWalking <= 0) {
    // start the next wave
    _wave_starting = true;
    scheduleOnce(schedule_selector(GLayer::startNextWave), 2);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onGestureReceived(c::Node *sender) {
  auto gesture_layer = (GestureLayer*)sender;
  // call the respective gesture's handler
  switch (gesture_layer->getGestureType()) {
    case E_GESTURE_TAP:
      handleTap(gesture_layer->getTouchStart());
    break;

    case E_GESTURE_SWIPE_UP:
      handleSwipeUp();
    break;

    case E_GESTURE_SWIPE_DOWN:
      handleSwipeDown();
    break;

    case E_GESTURE_SWIPE_LEFT:
      handleSwipeLeft();
    break;

    case E_GESTURE_SWIPE_RIGHT:
      handleSwipeRight();
    break;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::handleTap(const CCT_PT &position) {
  // get the touch coordinates with respect to the tile map
  auto touch_point = _tiledMap->convertToNodeSpace(position);
  auto wb= cx::visBox();
  auto tile_coord = CCT_PT(GET_COL_FOR_X(touch_point.x),
                           GET_ROW_FOR_Y(touch_point.y, MAX_ROWS));
  touch_point = c::ccpMult(tile_coord, TILE_SIZE);
  touch_point.y = wb.top - touch_point.y;

  // check if the touched tile is empty
  auto tile_GID = _tmxLayer->tileGIDAt(tile_coord);
  // if the touched tile is empty, show the tower placement menu
  if (tile_GID == 0) {
    // check to ensure only one menu is visible at a time
    if (!_towerMenu->placementNode->isVisible() &&
        !_towerMenu->maintenanceNode->isVisible() ) {
      _towerMenu->showPlacementMenu(touch_point);
      // show the grid
      CC_SHOW(_gridNode);
    }
  }
  // a tower exists on the touched tile
  else if(tile_GID >= TOWER_GID) {
    auto tower_index = tile_GID - TOWER_GID;
    // first check bounds and then check to ensure only one menu is visible at a time
    if(tower_index >= 0 &&
        tower_index < _numTowers &&
        !_towerMenu->maintenanceNode->isVisible() &&
        !_towerMenu->placementNode->isVisible() ) {
      // show the tower's current range
      _towers[tower_index]->showRange();
      _towerMenu->showMaintenanceMenu(touch_point, tower_index,
          _towers[tower_index]->getType(),
          _towers[tower_index]->getLevel());
    }
  }

  // hide the tower placement menu if it is visible
  if (_towerMenu->placementNode->isVisible()) {
    _towerMenu->hidePlacementMenu();
    CC_HIDE(_grid_node);
  }
  // hide the tower maintenance menu if it is visible
  if(_towerMenu->maintenanceNode->isVisible()) {
    _towerMenu->hideMaintenanceMenu();
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::handleSwipeUp() {
  // return if the tower placement menu is not active
  if (!_towerMenu->placementNode->isVisible() ) {
  return; }

  // place the tower with specified type at specified position
  placeTower(1, _towerMenu->placementNode->getPosition());
  _towerMenu->hidePlacementMenu();
  CC_HIDE(_gridNode);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::handleSwipeDown() {
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::handleSwipeLeft() {
  // return if the tower placement menu is not active
  if (!_towerMenu->placementNode->isVisible() ) {
  return; }

  // place the tower with specified type at specified position
  placeTower(0, _towerMenu->placementNode->getPosition());
  _towerMenu->hidePlacementMenu();
  CC_HIDE(_gridNode);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::handleSwipeRight() {
  // return if the tower placement menu is not active
  if (!_towerMenu->placementNode->isVisible() ) {
  return; }

  // place the tower with specified type at specified position
  placeTower(2, _towerMenu->placementNode->getPosition());
  _towerMenu->hidePlacementMenu();
  CC_HIDE(_gridNode);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTowerButtonClicked(c::Node *sender) {
  // place the appropriate tower based on which button was pressed
  auto tag = sender->getTag();
  placeTower(tag, _towerMenu->placementNode->getPosition());
  _towerMenu->hidePlacementMenu();
  CC_HIDE(_gridNode);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onUpgradeTowerClicked(c::Node *sender) {
  auto index = sender->getTag();
  // check bounds and upgrade the tower
  if (index >= 0 && index < _numTowers) {
    _towers[index]->upgrade();
  }
  _towerMenu->hideMaintenanceMenu();
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onSellTowerClicked(c::Node *sender) {
  auto index = sender->getTag();
  // check bounds and sell the tower
  if (index >= 0 && index < _numTowers) {
    sellTower(index);
  }
  _towerMenu->hideMaintenanceMenu();
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onToggleSpeedClicked(c::Node *sender) {
  _timeScale = ((int)_timeScale == 1) ? 2 : 1;
  // toggle the scheduler's time scale
  CC_DTOR()->getScheduler()->setTimeScale(_timeScale);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onPauseClicked(c::Node *sender) {
  // this prevents multiple pause popups
  if (_popupActive) {
  return; }

  // reset the scheduler's time scale to what it was
  CC_DTOR()->getScheduler()->setTimeScale(1);
  _gestureLayer->setTouchEnabled(false);
  //kenl
  //hud_menu_->setEnabled(false);
  pauseSchedulerAndActions();

  // pause game elements here
  if (_currWave) {
    for (auto i = 0; i < _currWave.numEnemies; ++i) {
      _currWave.enemies[i]->pauseSchedulerAndActions();
    }
  }

  for (auto i = 0; i < _numTowers; ++i) {
    _towers[i]->pauseSchedulerAndActions();
  }

  // create & add the pause popup
  auto pause_popup = PausePopup::create(this);
  addChild(pause_popup, E_LAYER_POPUP);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::resumeGame() {
  _popupActive = false;

  // set the scheduler's time scale to what it was
  CC_DTOR()->getScheduler()->setTimeScale(_timeScale);
  _gestureLayer->setTouchEnabled(true);
  //kenl
  //hud_menu_->setEnabled(true);
  resumeSchedulerAndActions();

  // resume game elements here
  if (_currWave) {
    for (auto i = 0; i < _currWave.numEnemies; ++i) {
      _currWave.enemies[i]->resumeSchedulerAndActions();
    }
  }

  for (auto i = 0; i < _numTowers; ++i) {
    _towers[i]->resumeSchedulerAndActions();
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::gameOver(bool is_level_complete) {
  // this prevents multiple pause popups
  if (_popupActive) {
  return; }

  _popupActive = true;

  // reset the scheduler's time scale
  CC_DTOR()->getScheduler()->setTimeScale(1);
  // stop GameWorld update
  unscheduleAllSelectors();
  _gestureLayer->setTouchEnabled(false);

  // stop game elements here
  if (_currWave) {
    for (auto i = 0; i < _currWave.numEnemies; ++i) {
      _currWave.enemies[i]->stopAllActions();
      _currWave.enemies[i]->unscheduleAllSelectors();
    }
  }

  for (auto i = 0; i < _numTowers; ++i) {
    _towers[i]->stopAllActions();
    _towers[i]->unscheduleAllSelectors();
  }

  // show the respective popup
  Popup *popup;
  if (is_level_complete) {
    popup = LevelCompletePopup::create(this);
  } else {
    popup = GameOverPopup::create(this);
  }
  addChild(popup, E_LAYER_POPUP);
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


NS_END




