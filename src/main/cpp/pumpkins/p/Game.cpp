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
#include "tinyxml2/tinyxml2.h"
#include "HUD.h"
#include "MMenu.h"
#include "Ende.h"
#include "Game.h"
#include "Popups.h"
#include "TowerMenu.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(pumpkins)
BEGIN_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
struct CC_DLL GLayer : public f::GameLayer {

  HUDLayer* getHUD() {
    return (HUDLayer*)getSceneX()->getLayer(3); }

  void updateLogic();
  void createTiledMap();
  void createGrid();
  void createWalkPoints();
  void createWaves();
  void createPumpkin();
  void placeTower(int type, const CCT_PT &position);
  void sellTower(int index);
  void startNextWave(float dt);
  void spawnEnemy(float dt);
  void checkWaveCompletion();
  void onGesture(EGestureType gtype);
  void handleTap(const CCT_PT &position);
  void handleSwipeUp();
  void handleSwipeDown();
  void handleSwipeLeft();
  void handleSwipeRight();
  void onTowerButtonClicked(c::Ref*);
  void onUpgradeTowerClicked(c::Ref*);
  void onSellTowerClicked(c::Ref*);
  void onToggleSpeedClicked();
  void onPauseClicked();
  void resumeGame();
  void gameOver(bool is_level_complete);
  void handleTouch();

  __decl_mv(EGestureType, _gestureType, E_GESTURE_NONE)
  __decl_md(CCT_PT, _touchStart)
  __decl_bf(_touchActive)
  __decl_md(CCT_PT, _touchEnd)
  __decl_ptr(ecs::Node, _shared)
  __decl_ptr(ecs::Node, _player)

    __decl_create_layer(GLayer)
    __decl_deco_ui()
    __decl_get_iid(2)

  virtual void onMouseMotion(const CCT_PT&);
  virtual bool onMouseStart(const CCT_PT&);
  virtual void onMouseClick(const CCT_PT&);

  virtual void onTouchMotion(c::Touch*);
  virtual bool onTouchStart(c::Touch*);
  virtual void onTouchEnd(c::Touch*);

  virtual void onInited();

  virtual ~GLayer() {}
};

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onInited() {

  _shared= _engine->getNodes("n/GVars")[0];
  _player= _engine->getNodes("f/CHuman")[0];

  auto ss= CC_GEC(GVars,_shared,"n/GVars");
  auto wz= cx::visSize();
  auto wb= cx::visBox();

  initOnce(ss);
  ss->currWaveIndex = -1;
  ss->timeScale = 1;
  ss->wavesLabel= getHUD()->_wavesLabel;
  ss->cashLabel= getHUD()->_cashLabel;

  createTiledMap();
  createGrid();
  createWalkPoints();
  createWaves();
  createPumpkin();

  getHUD()->updateLabels(ss);
  getHUD()->createMenu();

  // create & add the tower placement & maaintenance menus
  ss->towerMenu = TowerMenu::create(ss);
  addChild(ss->towerMenu, E_LAYER_HUD + 1);

  // start the first wave in a few seconds
  scheduleOnce(schedule_selector(GLayer::startNextWave), 2);
  scheduleUpdate();

  // reset the scheduler's time scale
  CC_DTOR()->getScheduler()->setTimeScale(ss->timeScale);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseClick(const CCT_PT &tap) {
  _touchEnd = tap;
  handleTouch();
  _touchActive = false;
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseMotion(const CCT_PT &tap) {
  _touchEnd = tap;
  handleTouch();
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onMouseStart(const CCT_PT &tap) {
  _gestureType = E_GESTURE_NONE;
  _touchStart = tap;
  _touchEnd = CC_ZPT;
  _touchActive = true;
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onTouchStart(c::Touch *touch) {
  return onMouseStart(touch->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchMotion(c::Touch *touch) {
  onMouseMotion(touch->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchEnd(c::Touch *touch) {
  onMouseClick(touch->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::handleTouch() {

  if (!_touchActive) {
  return; }

  // check for a single tap
  if (c::ccpFuzzyEqual(_touchStart, _touchEnd, 1)) {
    onGesture(E_GESTURE_TAP);
    _touchActive = false;
    return;
  }

  // calculate distance between first and last touch
  auto touch_difference = c::ccpSub(_touchEnd, _touchStart);
  // horizontal swipe
  if (fabs(touch_difference.x) > MIN_GESTURE_DISTANCE) {
    onGesture( ((int)touch_difference.x > 0)
        ? E_GESTURE_SWIPE_RIGHT : E_GESTURE_SWIPE_LEFT);
    _touchActive = false;
    return;
  }

  // vertical swipe
  if (fabs(touch_difference.y) > MIN_GESTURE_DISTANCE) {
    onGesture( ((int)touch_difference.y > 0)
        ? E_GESTURE_SWIPE_UP : E_GESTURE_SWIPE_DOWN);
    _touchActive = false;
    return;
  }

}

//////////////////////////////////////////////////////////////////////////////
void GLayer::decoUI() {
  centerImage("game.bg", E_LAYER_BACKGROUND);
  regoAtlas("game-pics");
  _engine = mc_new(GEngine);
  //cx::sfxMusic("background", true);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::createTiledMap() {
    auto ss=CC_GEC(GVars,_shared,"n/GVars");
  auto ctx= (GameCtx*) MGMS()->getCtx();
    auto wb=cx::visBox();
  auto fn= c::StringUtils::format("misc/level_%02d.tmx", ctx->level+1);

  ss->tiledMap = c::TMXTiledMap::create(fn);
  //kenl - check if needed
  //XCFG()->fit(ss->tiledMap);
  addChild(ss->tiledMap, E_LAYER_BACKGROUND+1);

  auto sz= CC_CSIZE(ss->tiledMap);
  CC_POS2(ss->tiledMap, wb.cx - HWZ(sz), wb.cy - HHZ(sz));
  ss->tmxLayer = ss->tiledMap->layerNamed("EnemyPath");

    CCLOG("tmx loaded");
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::createGrid() {
    auto ss=CC_GEC(GVars,_shared,"n/GVars");
    auto wb= cx::visBox();
  // create a node to draw the grid
  ss->gridNode = c::DrawNode::create();
  CC_HIDE(ss->gridNode);
  // position it relative to the tiled map
  CC_POS1(ss->gridNode,
      c::ccpAdd(ss->tiledMap->getPosition(), CCT_PT(0, 100)));
  addChild(ss->gridNode, E_LAYER_BACKGROUND);

  auto map_size = ss->tiledMap->getMapSize();
  auto counter = 0;
  // run a loop to alternatively draw a
  // semi-transparent black square indicating a tile
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
      ss->gridNode->drawPolygon(vertices, 4,
          c::ccc4f(0.0f, 0.0f, 0.0f, 0.12f), 0,
          c::ccc4f(0.0f, 0.0f, 0.0f, 0.0f));
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::createWalkPoints() {
    auto ss=CC_GEC(GVars,_shared,"n/GVars");
  // parse the list of objects
  auto object_group = ss->tiledMap->getObjectGroup("PathObjects");
  auto objects = object_group->getObjects();
  auto num_objects = objects.size();

  for (auto i = 0; i < num_objects; ++i) {
    auto object = objects[i].asValueMap();

    // save each WalkPoint's position for enemies to use
    if (strstr(object.at("name").asString().c_str(), "WalkPoint") != CC_NIL) {
      ss->enemyWalkPoints.push_back(
          CCT_PT(object.at("x").asFloat(), object.at("y").asFloat()));
    }
  }
  ss->numEnemyWalkPoints = ss->enemyWalkPoints.size();
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::createWaves() {
  auto ctx = (GameCtx*) MGMS()->getCtx();
  auto fn= c::StringUtils::format("misc/level_%02d.xml", ctx->level + 1);
  ssize_t size;
  auto data = (char*)CC_FILER()->getFileData(fn, "rb", &size);

  tinyxml2::XMLDocument xml_document;
  tinyxml2::XMLError xml_result = xml_document.Parse(data, size);
  CC_SAFE_DELETE(data);

  // print the error if parsing was unsuccessful
  if (xml_result != tinyxml2::XML_SUCCESS) {
    CCLOGERROR("Error:%d while reading xml", xml_result);
    return;
  }

  tinyxml2::XMLNode *level_node = xml_document.FirstChild();
  auto ss= CC_GEC(GVars, _shared, "n/GVars");

  // save the initial cash for this level
  ss->cash = level_node->ToElement()->IntAttribute("cash");

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
    for (auto i = 0; i < wave.numEnemies; ++i) {
      auto enemy = Enemy::create(ss, enemy_list[i]);
      wave.enemies.push_back(enemy);
      addItem( enemy, E_LAYER_ENEMY);
    }

    ss->waves.push_back(wave);
    ++ss->numWaves;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::createPumpkin() {
  // fetch the Pumpkin object from the tiled map
  auto ss= CC_GEC(GVars, _shared, "n/GVars");
  auto object_group = ss->tiledMap->getObjectGroup("PathObjects");
  auto pumpkin_object = object_group->getObject("Pumpkin");


  // create the sprite for the pumpkin
  ss->pumpkin = f::CPixie::reifyFrame("TD_pumkin_01.png");
  XCFG()->fit(ss->pumpkin);
  CC_POS2(ss->pumpkin, pumpkin_object.at("x").asFloat(), pumpkin_object.at("y").asFloat());
  addItem(ss->pumpkin, E_LAYER_TOWER);

  // create the sprite for the lives
  auto life_frame = cx::reifySprite("TD_livebg.png");
  XCFG()->fit(life_frame);
  auto psz= CC_CSIZE(ss->pumpkin);
  CC_POS2(life_frame,
      ss->pumpkin->getPositionX() - HWZ(psz),
      ss->pumpkin->getPositionY() + HHZ(psz));
  addItem(life_frame, E_LAYER_TOWER);

  auto ph= CC_GEC(f::CHealth,_player, "f/CHealth");
  // create the label for the lives
  ss->livesLabel = cx::reifyBmfLabel("dft",
      c::StringUtils::format("%02d", ph->curHP));
  XCFG()->scaleBmfont(ss->livesLabel, 32);
  ss->livesLabel->setAnchorPoint(CCT_PT(0.15, 0.5));
  ss->livesLabel->setColor(c::ccc3(255, 255, 189));
  CC_POS1(ss->livesLabel, life_frame->getPosition());
  addItem(ss->livesLabel, E_LAYER_TOWER);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::placeTower(int type, const CCT_PT &position) {
  auto ss= CC_GEC(GVars,_shared,"n/GVars");

  // can the player afford this tower?
  if (ss->cash < ss->towerDataSets[type].towerData[0].cost) {
  return; }

  // create a new Tower object & add it into the vector of towers
  auto tower = Tower::create(ss, type, position);
  addItem(tower, E_LAYER_TOWER);
  ++ss->numTowers;
  ss->towers.push_back(tower);

  // save tower's information into the tile map
  auto pos= ss->tiledMap->convertToNodeSpace(position);
  auto tile_coord = CCT_PT(GET_COL_FOR_X(pos.x),
                           GET_ROW_FOR_Y(pos.y, MAX_ROWS));
  ss->tmxLayer->setTileGID(TOWER_GID + (ss->numTowers - 1), tile_coord);

  // debit cash
  updateCash(ss, - tower->getCost());
  getHUD()->updateLabels(ss);

  // show the range for this tower
  tower->showRange();

  // hide the grid now that the tower has been placed
  CC_HIDE(ss->gridNode);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::sellTower(int index) {
  auto ss= CC_GEC(GVars,_shared,"n/GVars");
  // calculate the net worth of this tower
  auto total_cost = 0;
  for (auto i = 0; i <= ss->towers[index]->getLevel(); ++i) {
    total_cost +=
      ss->towerDataSets[ss->towers[index]->getType()].towerData[i].cost;
  }
  // credit cash
  updateCash(ss, total_cost / 2);

  // erase tower's information from the tile map
  auto position = ss->tiledMap->convertToNodeSpace(ss->towers[index]->getPosition());
  auto tile_coord = CCT_PT(GET_COL_FOR_X(position.x),
                           GET_ROW_FOR_Y(position.y, MAX_ROWS));
  ss->tmxLayer->setTileGID(0, tile_coord);

  // sell the tower & erase it from the vector
  ss->towers[index]->sell();
  ss->towers.erase(ss->towers.begin() + index);
  --ss->numTowers;
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::startNextWave(float dt) {
  auto ss= CC_GEC(GVars,_shared,"n/GVars");
  ++ss->currWaveIndex;
  // are there any waves left?
  if (ss->currWaveIndex >= ss->numWaves) {
    // level has completed
    gameOver(true);
  } else {
    // start the next wave in a few seconds
    ss->currWave = ss->waves[ss->currWaveIndex];
    schedule(schedule_selector(GLayer::spawnEnemy), ss->currWave.spawnDelay);
    getHUD()->updateLabels(ss);
    scaleLabel(ss->wavesLabel);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::spawnEnemy(float dt) {
  auto ss= CC_GEC(GVars,_shared,"n/GVars");
  // have all enemies of this wave been spawned?
  if (ss->currWave.numEnemiesSpawned >= ss->currWave.numEnemies) {
    // wave has finished starting
    ss->waveStarting = false;
    // stop spawning enemies
    unschedule(schedule_selector(GLayer::spawnEnemy));
    return;
  }
  // fetch the next enemy in the list
  auto enemy = ss->currWave.enemies[ss->currWave.numEnemiesSpawned++];
  // tell the enemy where to go
  enemy->setWalkPoints(ss->numEnemyWalkPoints, ss->enemyWalkPoints);
  // tell the enemy to start walking
  enemy->startWalking();
  ++ss->currWave.numEnemiesWalking;
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::updateLogic() {
  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  checkWaveCompletion();
  for (auto i = 0; i < ss->numTowers; ++i) {
    ss->towers[i]->update();
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::checkWaveCompletion() {
  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  // wave has completed when all enemies have been spawned AND
  // when there are no enemies walking (cuz they're all dead!)
  if (!ss->waveStarting &&
      ss->currWave.numEnemiesSpawned >= ss->currWave.numEnemies &&
      ss->currWave.numEnemiesWalking <= 0) {
    // start the next wave
    ss->waveStarting = true;
    scheduleOnce(schedule_selector(GLayer::startNextWave), 2);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onGesture(EGestureType gtype) {
  _gestureType = gtype;
  switch (gtype) {
    case E_GESTURE_TAP:
      handleTap(_touchStart);
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
  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  // get the touch coordinates with respect to the tile map
  auto touch_point = ss->tiledMap->convertToNodeSpace(position);
  auto wb= cx::visBox();
  auto tile_coord = CCT_PT(GET_COL_FOR_X(touch_point.x),
                           GET_ROW_FOR_Y(touch_point.y, MAX_ROWS));
  touch_point = c::ccpMult(tile_coord, TILE_SIZE);
  touch_point.y = wb.top - touch_point.y;

  // check if the touched tile is empty
  auto tile_GID = ss->tmxLayer->tileGIDAt(tile_coord);
  // if the touched tile is empty, show the tower placement menu
  if (tile_GID == 0) {
    // check to ensure only one menu is visible at a time
    if (!ss->towerMenu->_placementNode->isVisible() &&
        !ss->towerMenu->_maintenanceNode->isVisible() ) {
      ss->towerMenu->showPlacementMenu(touch_point);
      // show the grid
      CC_SHOW(ss->gridNode);
    }
  }
  // a tower exists on the touched tile
  else if(tile_GID >= TOWER_GID) {
    auto tower_index = tile_GID - TOWER_GID;
    // first check bounds and then check to ensure only one menu is visible at a time
    if(tower_index >= 0 &&
        tower_index < ss->numTowers &&
        !ss->towerMenu->_maintenanceNode->isVisible() &&
        !ss->towerMenu->_placementNode->isVisible() ) {
      // show the tower's current range
      ss->towers[tower_index]->showRange();
      ss->towerMenu->showMaintenanceMenu(touch_point, tower_index,
          ss->towers[tower_index]->getType(),
          ss->towers[tower_index]->getLevel());
    }
  }

  // hide the tower placement menu if it is visible
  if (ss->towerMenu->_placementNode->isVisible()) {
    ss->towerMenu->hidePlacementMenu();
    CC_HIDE(ss->gridNode);
  }
  // hide the tower maintenance menu if it is visible
  if (ss->towerMenu->_maintenanceNode->isVisible()) {
    ss->towerMenu->hideMaintenanceMenu();
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::handleSwipeUp() {
    auto ss=CC_GEC(GVars,_shared,"n/GVars");
  // return if the tower placement menu is not active
  if (!ss->towerMenu->_placementNode->isVisible() ) {
  return; }

  // place the tower with specified type at specified position
  placeTower(1, ss->towerMenu->_placementNode->getPosition());
  ss->towerMenu->hidePlacementMenu();
  CC_HIDE(ss->gridNode);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::handleSwipeDown() {
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::handleSwipeLeft() {
  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  // return if the tower placement menu is not active
  if (!ss->towerMenu->_placementNode->isVisible() ) {
  return; }

  // place the tower with specified type at specified position
  placeTower(0, ss->towerMenu->_placementNode->getPosition());
  ss->towerMenu->hidePlacementMenu();
  CC_HIDE(ss->gridNode);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::handleSwipeRight() {
  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  // return if the tower placement menu is not active
  if (!ss->towerMenu->_placementNode->isVisible() ) {
  return; }

  // place the tower with specified type at specified position
  placeTower(2, ss->towerMenu->_placementNode->getPosition());
  ss->towerMenu->hidePlacementMenu();
  CC_HIDE(ss->gridNode);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTowerButtonClicked(c::Ref *sender) {
  // place the appropriate tower based on which button was pressed
  auto tag = ((c::Node*)sender)->getTag();
  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  placeTower(tag, ss->towerMenu->_placementNode->getPosition());
  ss->towerMenu->hidePlacementMenu();
  CC_HIDE(ss->gridNode);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onUpgradeTowerClicked(c::Ref *sender) {
  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  auto index = ((c::Node*)sender)->getTag();
  // check bounds and upgrade the tower
  if (index >= 0 && index < ss->numTowers) {
    ss->towers[index]->upgrade();
  }
  ss->towerMenu->hideMaintenanceMenu();
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onSellTowerClicked(c::Ref *sender) {
  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  auto index = ((c::Node*)sender)->getTag();
  // check bounds and sell the tower
  if (index >= 0 && index < ss->numTowers) {
    sellTower(index);
  }
  ss->towerMenu->hideMaintenanceMenu();
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onToggleSpeedClicked() {
  auto ss= CC_GEC(GVars,_shared,"n/GVars");
  ss->timeScale = ((int)ss->timeScale == 1) ? 2 : 1;
  // toggle the scheduler's time scale
  CC_DTOR()->getScheduler()->setTimeScale(ss->timeScale);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onPauseClicked() {
  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  // this prevents multiple pause popups
  if (ss->popupActive) {
  return; }

  // reset the scheduler's time scale to what it was
  CC_DTOR()->getScheduler()->setTimeScale(1);
  //kenl
  //hud_menu_->setEnabled(false);
  pauseSchedulerAndActions();

  // pause game elements here

    for (auto i = 0; i < ss->currWave.numEnemies; ++i) {
      ss->currWave.enemies[i]->pauseSchedulerAndActions();
    }


  for (auto i = 0; i < ss->numTowers; ++i) {
    ss->towers[i]->pauseSchedulerAndActions();
  }

  // create & add the pause popup
  auto pause_popup = PausePopup::create(ss);
  addChild(pause_popup, E_LAYER_POPUP);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::resumeGame() {
  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  ss->popupActive = false;

  // set the scheduler's time scale to what it was
  CC_DTOR()->getScheduler()->setTimeScale(ss->timeScale);
  //kenl
  //hud_menu_->setEnabled(true);
  resumeSchedulerAndActions();

  // resume game elements here

    for (auto i = 0; i < ss->currWave.numEnemies; ++i) {
      ss->currWave.enemies[i]->resumeSchedulerAndActions();
    }


  for (auto i = 0; i < ss->numTowers; ++i) {
    ss->towers[i]->resumeSchedulerAndActions();
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::gameOver(bool is_level_complete) {
  auto ss= CC_GEC(GVars,_shared,"n/GVars");
  // this prevents multiple pause popups
  if (ss->popupActive) {
  return; }

  ss->popupActive = true;

  // reset the scheduler's time scale
  CC_DTOR()->getScheduler()->setTimeScale(1);
  // stop GameWorld update
  unscheduleAllSelectors();

  // stop game elements here
  for (auto i = 0; i < ss->currWave.numEnemies; ++i) {
    ss->currWave.enemies[i]->stopAllActions();
    ss->currWave.enemies[i]->unscheduleAllSelectors();
  }

  for (auto i = 0; i < ss->numTowers; ++i) {
    ss->towers[i]->stopAllActions();
    ss->towers[i]->unscheduleAllSelectors();
  }

  // show the respective popup
  Popup *popup;
  if (is_level_complete) {
    popup = LevelCompletePopup::create(ss);
  } else {
    popup = GameOverPopup::create(ss);
  }
  addChild(popup, E_LAYER_POPUP);
}

END_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic &topic, void *m) {
  auto y= (GLayer*) getGLayer();

  if (topic=="/game/hud/togglespeed") {
    y->onToggleSpeedClicked();
  }
  else
  if (topic=="/game/hud/pause") {
     y->onPauseClicked();
  }
  else
  if (topic=="/game/menu/tower") {
     y->onTowerButtonClicked((c::Ref*) m);
  }
  else
  if (topic=="/game/player/lose") {
     y->gameOver(false);
  }
  else
  if (topic=="/game/menu/upgrade") {
    y->onUpgradeTowerClicked((c::Ref*)m);
  }
  else
  if (topic=="/game/tower/sell") {
    y->onSellTowerClicked((c::Ref*)m);
  }
  else
  if (topic=="/game/hud/resume") {
    y->resumeGame();
  }
  else
  if ("/game/logic/update" == topic) {
     y->updateLogic();
  }

}

//////////////////////////////////////////////////////////////////////////////
void Game::decoUI() {
  HUDLayer::reify(this, 3);
  GLayer::reify(this, 2);
  play();
}


NS_END




