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

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/COMP.h"
#include "core/CCSX.h"
#include "tinyxml2/tinyxml2.h"
#include "C.h"
#include "Enemy.h"

NS_ALIAS(cx, fusii::ccsx)
NS_ALIAS(tx, tinyxml2)
NS_BEGIN(pumpkins)

//////////////////////////////////////////////////////////////////////////////
//
static void loadAnimations(GVars *ss) {
  for (auto i=1; i <= 8; ++i) {
    auto anim= cx::createAnimation(0.25);
    for (auto j=1; j <=2; ++j) {
      auto sp= cx::reifySprite(
          c::StringUtils::format("enemy_0%d000%d.png", i, j));
      XCFG()->fit(sp);
      anim->addSpriteFrame(sp->displayFrame());
    }
    CC_ACAC()->addAnimation(anim, "enemy_" + FTOS(i));
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void loadTowerData(GVars *ss) {
  ssize_t size;
  auto data = (char*) CC_FILER()->getFileData("misc/tower_data.xml", "rb", &size);
  tx::XMLDocument xdoc;
  tx::XMLError rc = xdoc.Parse(data, size);

  CC_SAFE_DELETE(data);

  if (rc != tx::XML_SUCCESS) {
    CCLOGERROR("Error:%d while reading tower_data.xml", rc);
    return;
  }

  auto tower_data_set_list = xdoc.FirstChild();
  tx::XMLElement *tower_data_set_element = CC_NIL;
  tx::XMLElement *tower_data_element = CC_NIL;
  // loop through each TowerDataSet tag
  for (auto tower_data_set_node = tower_data_set_list->FirstChild();
       tower_data_set_node != CC_NIL;
       tower_data_set_node = tower_data_set_node->NextSibling()) {
    tower_data_set_element = tower_data_set_node->ToElement();
    TowerDataSet tdset;
    tdset.bullet= tower_data_set_element->Attribute("bullet_name");
    tdset.lightning = tower_data_set_element->BoolAttribute("is_lightning");
    tdset.rotating = tower_data_set_element->BoolAttribute("is_rotating");
    // loop through each TowerData tag
    for (auto tower_data_node = tower_data_set_node->FirstChild();
         tower_data_node != CC_NIL;
         tower_data_node = tower_data_node->NextSibling()) {
      tower_data_element = tower_data_node->ToElement();
      TowerData tdata;
      tdata.sprite= tower_data_element->Attribute("sprite_name");
      tdata.range= tower_data_element->FloatAttribute("range");
      tdata.physicalDamage= tower_data_element->FloatAttribute("physical_damage");
      tdata.magicalDamage= tower_data_element->FloatAttribute("magical_damage");
      tdata.speedDamage= tower_data_element->FloatAttribute("speed_damage");
      tdata.speedDamageDuration = tower_data_element->FloatAttribute("speed_damage_duration");
      tdata.fireRate = tower_data_element->FloatAttribute("fire_rate");
      tdata.cost = tower_data_element->IntAttribute("cost");
      tdset.towerData.push_back(tdata);
    }

    ss->towerDataSets.push_back(tdset);
    ss->numTowerDataSets++;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void loadEnemyData(GVars *ss) {
  ssize_t size;
  auto data = (char*) CC_FILER()->getFileData("misc/enemy_data.xml", "rb", &size);

  tx::XMLDocument xdoc;
  tx::XMLError rc = xdoc.Parse(data, size);

  CC_SAFE_DELETE(data);

  if (rc != tx::XML_SUCCESS) {
    CCLOGERROR("Error:%d while reading enemy_data.xml", rc);
    return;
  }

  auto enemy_data_list = xdoc.FirstChild();
  tx::XMLElement *enemy_data_element = CC_NIL;
  // loop through each EnemyData tag
  for (auto enemy_data_node = enemy_data_list->FirstChild();
       enemy_data_node != CC_NIL;
       enemy_data_node = enemy_data_node->NextSibling()) {
    enemy_data_element = enemy_data_node->ToElement();
    EnemyData edata;
    edata.animation= enemy_data_element->Attribute("animation");
    edata.health= enemy_data_element->IntAttribute("health");
    edata.armor = enemy_data_element->IntAttribute("armor");
    edata.magicResistance= enemy_data_element->IntAttribute("magic_resistance");
    edata.speed= enemy_data_element->FloatAttribute("speed");
    edata.damage= enemy_data_element->IntAttribute("damage");
    edata.reward = enemy_data_element->IntAttribute("reward");
    ss->enemyData.push_back(edata);
    ss->numEnemyData++;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
static void loadData(GVars *ss) {
    loadAnimations(ss);
    loadTowerData(ss);
    loadEnemyData(ss);
}


//////////////////////////////////////////////////////////////////////////////
//
void scaleLabel(not_null<c::Label*> label) {
  auto k= label->getScale();
  label->runAction(
      c::Sequence::create(
        c::ScaleTo::create(0.1, 1.2*k),
        c::ScaleTo::create(0.1, k),
        c::ScaleTo::create(0.1, 1.2*k),
        c::ScaleTo::create(0.1, k),
        CC_NIL));
}

//////////////////////////////////////////////////////////////////////////////
//
// function takes comma separated string & returns vector of values
const s_vec<int> getIntListFromString(const sstr &input) {
  s_vec<int> result;

  if (input.length() == 0) {
  return result; }

  s::stringstream ssm(input);
  float i;
  while (ssm >> i) {
    result.push_back(i);
    if (ssm.peek() == ',')
      ssm.ignore();
  }
  return result;
}

//////////////////////////////////////////////////////////////////////////////
//
void reduceLives(not_null<GVars*> ss, int amount) {
  auto ph= ss->playerHealth;
  ph->hurt();
  switch (ph->curHP) {
    case 15:
      ss->pumpkin->setSpriteFrame("TD_pumkin_02.png");
    break;
    case 10:
      ss->pumpkin->setSpriteFrame("TD_pumkin_03.png");
    break;
    case 5:
      ss->pumpkin->setSpriteFrame("TD_pumkin_04.png");
    break;
  }
  ss->livesLabel->setString(FTOS(ph->curHP));
  scaleLabel(ss->livesLabel);
  if (!ph->alive()) {
    SENDMSG("/game/player/lose");
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void enemyAtTheGates(not_null<Enemy*> enemy) {
  // this is called when the enemy has reached the pumpkin
  auto ss= enemy->getGVars();
  reduceLives(ss,enemy->getDamage());
  ss->currWave.numEnemiesWalking -= 1;
}

//////////////////////////////////////////////////////////////////////////////
//
void enemyDown(not_null<Enemy*> enemy) {
  auto ss= enemy->getGVars();
  updateCash(ss, enemy->getReward());
  ss->currWave.numEnemiesWalking -= 1;
}

//////////////////////////////////////////////////////////////////////////////
//
void updateCash(not_null<GVars*> ss, int amount) {
  ss->cash += amount;
  SENDMSG("/game/hud/updatelabels");
  scaleLabel(ss->cashLabel);
}

//////////////////////////////////////////////////////////////////////////////
//
void initOnce(not_null<GVars*> ss) {
  loadData(ss);
}


NS_END




