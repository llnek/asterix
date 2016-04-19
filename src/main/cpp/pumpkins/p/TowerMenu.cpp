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
#include "core/COMP.h"
#include "core/CCSX.h"
#include "TowerMenu.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(pumpkins)

//////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////
//
owner<TowerMenu*> TowerMenu::create() {
  auto z = mc_new(TowerMenu);
  z->init();
  z->autorelease();
  return z;
}

//////////////////////////////////////////////////////////////////////////////
//
bool TowerMenu::init() {

  if (!c::Node::init()) {
  return false; }

  createPlacementMenu();
  createMaintenanceMenu();
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void TowerMenu::createPlacementMenu() {

  _placementNode = c::Node::create();
  _placementNode->setContentSize(XCFG()->fit(CCT_SZ(400, 400)));
  _placementNode->setScale(0);
  CC_HIDE(_placementNode);
  addChild(_placementNode);

  auto placement_menu = c::Menu::create();
  placement_menu->setPosition(CC_ZPT);
  _placementNode->addChild(placement_menu);

  // create a button to place the tower
  auto tower_btn = createButton("TD_tw updateicon_01.png",
      CCT_PT(-150, 50), 0,
      [=]() {
        this->onTowerButtonClicked();
      });
  placement_menu->addChild(tower_btn);
  _placementButtons.push_back(tower_btn);
  _placementNode->addChild(createLabel(ss->towerDataSets[0].towerData[0].cost, "dft", 24, CCT_PT(-150, 20)));

  // create a sprite to demonstrate the gesture needed to spawn this tower
  auto swipe_tutorial = cx::reifySprite("swipe_left.png");
  XCFG()->fit(swipe_tutorial);
  CC_POS2(swipe_tutorial, -180, 150);
  _placementNode->addChild(swipe_tutorial);

  // create a button to place the tower
  tower_btn = createButton("TD_tw updateicon_02.png",
      CCT_PT(0, 150), 1,
      [=]() {
        this->onTowerButtonClicked();
      });
  placement_menu->addChild(tower_btn);
  _placementButtons.push_back(tower_btn);
  _placementNode->addChild(createLabel(ss->towerDataSets[1].towerData[0].cost, "dft", 24, CCT_PT(0, 125)));

  // create a sprite to demonstrate the gesture needed to spawn this tower
  swipe_tutorial = cx::reifySprite("swipe_up.png");
  XCFG()->fit(swipe_tutorial);
  CC_POS2(swipe_tutorial, 0, 275);
  _placementNode->addChild(swipe_tutorial);

  // create a button to place the tower
  tower_btn = createButton("TD_tw updateicon_03.png",
      CCT_PT(150, 50), 2,
      [=]() {
        this->onTowerButtonClicked();
        });
  placement_menu->addChild(tower_btn);
  _placementButtons.push_back(tower_btn);
  _placementNode->addChild(createLabel(ss->towerDataSets[2].towerData[0].cost, "dft", 24, CCT_PT(150, 20)));

  // create a sprite to demonstrate the gesture needed to spawn this tower
  swipe_tutorial = cx::reifySprite("swipe_right.png");
  XCFG()->fit(swipe_tutorial);
  CC_POS2(swipe_tutorial, 180, 150);
  _placementNode->addChild(swipe_tutorial);

  // create a sprite to highlight the targeted tile
  auto highlight = cx::reifySprite("tw_adtoicon.png");
  XCFG()->fit(highlight);
  _placementNode->addChild(highlight);

}

//////////////////////////////////////////////////////////////////////////////
//
void TowerMenu::createMaintenanceMenu() {
  _maintenanceNode = c::Node::create();
  _maintenanceNode->setContentSize(XCFG()->fit(CCT_SZ(300, 300)));
  _maintenanceNode->setScale(0);
  CC_HIDE(_maintenanceNode);
  addChild(_maintenanceNode);

  auto maintenance_menu = c::Menu::create();
  CC_POS1(maintenance_menu, CC_ZPT);
  _maintenanceNode->addChild(maintenance_menu);

  // create a button to upgrade the tower
  _upgradeButton = createButton("TD_tupdate01.png",
      CCT_PT(0, 100), 0,
      [=]() {
      this->onUpgradeTowerClicked();
      });
  maintenance_menu->addChild(_upgradeButton);
  _upgradeLabel = createLabel(0, "dft", 24, CCT_PT(0, 70));
  _maintenanceNode->addChild(_upgradeLabel);

  // create a button to sell the tower
  _sellButton = createButton("TD_tucancel02.png",
      CCT_PT(0, -100), 0, [=](){
        this->onSellTowerClicked();
        });
  maintenance_menu->addChild(_sellButton);
  _sellLabel = createLabel(0, "dft", 24, CCT_PT(0, -130));
  _maintenanceNode->addChild(_sellLabel);
}

//////////////////////////////////////////////////////////////////////////////
//
void TowerMenu::showPlacementMenu(const CCT_PT &position) {
  _placementNode->runAction(
      c::Sequence::create(
        c::Show::create(),
        c::Place::create(position),
        c::EaseBackOut::create(
          c::ScaleTo::create(0.2, 1)),
        CC_NIL));
  enablePlacementButtons();
}

//////////////////////////////////////////////////////////////////////////////
//
void TowerMenu::hidePlacementMenu() {
  _placementNode->runAction(
      c::Sequence::create(
        c::EaseBackIn::create(
          c::ScaleTo::create(0.2, 0)),
        c::Hide::create(),
        c::Place::create(CC_ZPT),
        CC_NIL));
}

//////////////////////////////////////////////////////////////////////////////
//
void TowerMenu::enablePlacementButtons() {
  // check if a tower button should be enabled based on whether the player can afford it
  for (auto i = 0; i < 3; ++i) {
    bool enabled = getCash(ss) >= ss->towerDataSets[i].towerData[0].cost;
    _placementButtons[i]->setEnabled(enabled);
    // change colour since we don't have a different sprite for disabled state
    _placementButtons[i]->setColor(
        enabled ? c::ccc3(255, 255, 255) : c::ccc3(128, 128, 128));
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void TowerMenu::showMaintenanceMenu(
    const CCT_PT &position, int tower_index, int tower_type, int tower_level) {
  auto buf= "-";
  // set the appropriate price into the upgrade label
  if (tower_level < NUM_TOWER_UPGRADES - 1) {
    buf=FTOS(ss->towerDataSets[tower_type].towerData[tower_level + 1].cost);
  }
  _upgradeLabel->setString(buf);

  auto total_cost = 0;
  // calculate the price into the sell label
  for (auto i = 0; i <= tower_level; ++i) {
    // resale value will be half of the total expenditure
    // total expenditure includes initial price and upgrades
    total_cost += ss->towerDataSets[tower_type].towerData[i].cost;
  }
  _sellLabel->setString( FTOS((int)(total_cost / 2)));

  // save tower information into the upgrade & sell buttons
  _upgradeButton->setTag(tower_index);
  _sellButton->setTag(tower_index);

  _maintenanceNode->runAction(
      c::Sequence::create(
        c::Show::create(),
        c::Place::create(position),
        c::EaseBackOut::create(
          c::ScaleTo::create(0.2, 1)),
        CC_NIL));

  enableMaintenanceButtons(tower_type, tower_level);
}

//////////////////////////////////////////////////////////////////////////////
//
void TowerMenu::hideMaintenanceMenu() {
  _maintenanceNode->runAction(
      c::Sequence::create(
        c::EaseBackIn::create(
          c::ScaleTo::create(0.2, 0)),
        c::Hide::create(),
        c::Place::create(CC_ZPT),
        CC_NIL));
}

//////////////////////////////////////////////////////////////////////////////
//
void TowerMenu::enableMaintenanceButtons(
    int tower_type, int tower_level) {

  // check if the upgrade button should be enabled based on whether the player can afford it
  bool enabled = getCash(ss) >= ss->towerDataSets[tower_type].towerData[tower_level + 1].cost;
  _upgradeButton->setEnabled(enabled);
  _upgradeButton->setColor(
      enabled ? c::ccc3(255, 255, 255) : c::ccc3(128, 128, 128));
}

//////////////////////////////////////////////////////////////////////////////
//
c::MenuItem* TowerMenu::createButton(const sstr &sprite,
    const CCT_PT &position, int tag, VOIDFN handler) {
  auto button = cx::reifyMenuBtn(sprite);
  XCFG()->fit(button);
  button->setCallback(handler);
  button->setTag(tag);
  CC_POS1(button, position);
  return button;
}

//////////////////////////////////////////////////////////////////////////////
//
c::Label* TowerMenu::createLabel(int content, const sstr &font,
    float size, const CCT_PT &position) {
  auto label = cx::reifyBmfLabel(font, FTOS(content));
  XCFG()->scaleBmfont(label,size);
  CC_POS1(label, position);
  label->setColor(c::ccc3(0, 102, 0));
  return label;
}


NS_END



