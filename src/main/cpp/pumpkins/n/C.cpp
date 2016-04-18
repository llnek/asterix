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
#include "C.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(pumpkins)

//////////////////////////////////////////////////////////////////////////////
//
void initOnce() {
  loadData();
}

//////////////////////////////////////////////////////////////////////////////
//
void loadData() {
  loadAnimations();
  loadTowerData();
  loadEnemyData();
}

//////////////////////////////////////////////////////////////////////////////
//
void loadAnimations() {
  for (auto i=1; i <= 8; ++i) {
    auto anim= cx::createAnimation(0.25);
    for (auto j=1; j <=2; ++j) {
      auto png= "enemy_0" + FTOS(i) + "000" + FTOS(j) + ".png";
      auto sp= cx::reifySprite(png);
      XCFG()->fit(sp);
      anim->addSpriteFrame(sp->displayFrame());
    }
    CC_ACAC()->addAnimation(anim, "enemy_" + FTOS(i));
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void loadTowerData(GVars *ss) {
  unsigned long size;
  auto data = (char*) CC_FILER()->getFileData("tower_data.xml", "rb", &size);

  // parse file
  tinyxml2::XMLDocument xml_document;
  tinyxml2::XMLError xml_result = xml_document.Parse(data, size);

  CC_SAFE_DELETE(data);

  // print the error if parsing was unsuccessful
  if (xml_result != tinyxml2::XML_SUCCESS) {
    CCLOGERROR("Error:%d while reading tower_data.xml", xml_result);
    return;
  }

  tinyxml2::XMLNode *tower_data_set_list = xml_document.FirstChild();
  tinyxml2::XMLElement *tower_data_set_element = CC_NIL;
  tinyxml2::XMLElement *tower_data_element = CC_NIL;
  // loop through each TowerDataSet tag
  for (tinyxml2::XMLNode *tower_data_set_node = tower_data_set_list->FirstChild(); tower_data_set_node != CC_NIL; tower_data_set_node = tower_data_set_node->NextSibling())
  {
    tower_data_set_element = tower_data_set_node->ToElement();
    TowerDataSet tdset;
    tdset.bullet= tower_data_set_element->Attribute("bullet_name");
    tdset.lightning = tower_data_set_element->BoolAttribute("is_lightning");
    tdset.rotating = tower_data_set_element->BoolAttribute("is_rotating");

    // loop through each TowerData tag
    for (tinyxml2::XMLNode *tower_data_node = tower_data_set_node->FirstChild(); tower_data_node != CC_NIL; tower_data_node = tower_data_node->NextSibling())
    {
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
  unsigned long size;
  auto data = (char*) CC_FILER()->getFileData("enemy_data.xml", "rb", &size);

  tinyxml2::XMLDocument xml_document;
  tinyxml2::XMLError xml_result = xml_document.Parse(data, size);

  CC_SAFE_DELETE(data);

  // print the error if parsing was unsuccessful
  if (xml_result != tinyxml2::XML_SUCCESS) {
    CCLOGERROR("Error:%d while reading enemy_data.xml", xml_result);
    return;
  }

  tinyxml2::XMLNode *enemy_data_list = xml_document.FirstChild();
  tinyxml2::XMLElement *enemy_data_element = CC_NIL;
  // loop through each EnemyData tag
  for (tinyxml2::XMLNode *enemy_data_node = enemy_data_list->FirstChild(); enemy_data_node != CC_NIL; enemy_data_node = enemy_data_node->NextSibling())
  {
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
s_vec<int> getIntListFromString(const sstr &input) {
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


NS_END




