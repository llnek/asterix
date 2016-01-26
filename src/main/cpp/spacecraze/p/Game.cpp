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
#include "BackDrop.h"
//#include "MMenu.h"
#include "HUD.h"
#include "Game.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(spacecraze)
BEGIN_NS_UNAMED


//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GLayer : public f::GameLayer {

  HUDLayer* getHUD() {
    return (HUDLayer*) getSceneX()->getLayer(3); }

  STATIC_REIFY_LAYER(GLayer)
  MDECL_DECORATE()
  MDECL_GET_IID(2)

  void createLevel();

};

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::decorate() {

  engine= mc_new(GEngine);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::createLevel() {

  auto ctx = (GCXX*) MGMS()->getCtx();
  auto wb = cx::visBox();

  //score_ = score_to_carry_;

  regoAtlas("game-pics");

  right_side_enemy_position_ = wb.cx;
  left_side_enemy_position_ = wb.cx;

  // generate level filename
  char level_file[16] = {0};
  sprintf(level_file, "Level%02d.xml", current_level_);

  // fetch level file data
  unsigned long size;
  char* data = (char*)CCFileUtils::sharedFileUtils()->getFileData(level_file, "rb", &size);

  // parse the level file
  tinyxml2::XMLDocument xml_document;
  tinyxml2::XMLError xml_result = xml_document.Parse(data, size);

  CC_SAFE_DELETE(data);

  // print the error if parsing was unsuccessful
  if(xml_result != tinyxml2::XML_SUCCESS)
  {
    CCLOGERROR("Error:%d while reading %s", xml_result, level_file);
    return;
  }

  // save player data
  tinyxml2::XMLNode* level_node = xml_document.FirstChild();
  player_fire_rate_ = level_node->ToElement()->FloatAttribute("player_fire_rate");

  // create set of enemies
  tinyxml2::XMLNode* enemy_set_node = level_node->FirstChild();
  CreateEnemies(enemy_set_node);

  // create set of bricks
  tinyxml2::XMLNode* brick_set_node = enemy_set_node->NextSibling();
  CreateBricks(brick_set_node);

  CreatePlayer();

  CreateHUD();

  // everything created, start updating
  scheduleUpdate();
}



END_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic &t, void *m) {

}

//////////////////////////////////////////////////////////////////////////////
//
void Game::decorate() {
  BackDrop::reify(this, 1);
  HUDLayer::reify(this, 3);
  GLayer::reify(this,2);
  play();
}



NS_END



