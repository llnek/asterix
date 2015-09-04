// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2015, Ken Leung. All rights reserved.

#include "Config.h"
USING_NS_CC;
NS_FI_BEGIN

//////////////////////////////////////////////////////////////////////////////
//
static XConfig* singleton;

//////////////////////////////////////////////////////////////////////////////
//
static void init_assets(Dictionary* dict) {

  Dictionary* d;

  d= Dictionary::create();
  d->setObject("res/l10n/en/images.png", "lang-pics");
  d->setObject("res/pics/images.png", "game-pics");
  dict->setObject(d, "atlases");

  d= Dictionary::create();
  dict->setObject(d, "tiles");

  d= Dictionary::create();
  d->setObject("res/pics/bg.png", "gui.mmenus.menu.bg");
  d->setObject("res/pics/bg.png", "game.bg");
  dict->setObject(d, "images");

  d= Dictionary::create();
  d->setObject("res/sfx/MineExplosion", "game_end");
  d->setObject("res/sfx/Death", "game_quit");
  d->setObject("res/sfx/missile", "ship-missile");
  d->setObject("res/sfx/march", "bugs-march");
  d->setObject("res/sfx/explode", "xxx-explode");
  dict->setObject(d, "sounds");

  d= Dictionary::create();
  d->setObject("res/font/en/SmallTypeWriting", "font.SmallTypeWriting");
  d->setObject("res/font/en/AutoMission", "font.AutoMission");
  d->setObject("res/font/en/Subito", "font.Subito");
  d->setObject("res/font/en/CoffeeBuzzed", "font.CoffeeBuzzed");
  dict->setObject(d, "fonts");

}

//////////////////////////////////////////////////////////////////////////////
//
static void init_csts(Dictionary* d) {

  d->setObject("live-missiles", "P_LMS");
  d->setObject("missiles", "P_MS");
  d->setObject("bombs", "P_BS");
  d->setObject("explosions", "P_ES");
  d->setObject("live-bombs", "P_LBS");

  d->setObject(6, "COLS");
  d->setObject(7, "ROWS");
  d->setObject(42, "CELLS");

  d->setObject(2, "LEFT");
  d->setObject(6, "TOP");
  d->setObject(4, "OFF_X");
  d->setObject(2, "OFF_Y");
}

//////////////////////////////////////////////////////////////////////////////
//
static void init_levels(Dictionary* dict) {

  auto d2= Dictionary::create();
  Dictionary* d3;
  auto d= Dictionary::create();
  dict->setObject(d, "levels");
  d->setObject(d2, "1");

  d3= Dictionary::create();
  d2->setObject(d3, "tiles");

  d3= Dictionary::create();
  d2->setObject(d3, "images");

  d3= Dictionary::create();
  d2->setObject(d3, "cfg");

}

//////////////////////////////////////////////////////////////////////////////
//
static const string getXXX(const string& group, const string& key) {
  auto d= static_cast<Dictionary*>(m_dict->objectForKey(group));
  auto r= static_cast<String*>(d->objectForKey(key));
  return r != nullptr ? r->getCString() : "";
}

//////////////////////////////////////////////////////////////////////////////
//
const string Config::GetAppKey() {
  return "d39bf198-518a-4de7-88a0-5e28c88571b0";
}

//////////////////////////////////////////////////////////////////////////////
//
const string Config::AppId() {
  return "invaders";
}

//////////////////////////////////////////////////////////////////////////////
//
const string Config::GetColor() {
  return "red";
}

//////////////////////////////////////////////////////////////////////////////
//
ResolutionPolicy Config::GetResolution() {
  return ResolutionPolicy::FIXED_HEIGHT;
}

//////////////////////////////////////////////////////////////////////////////
//
Config::~Config() {
  m_dict->release();
  singleton=nullptr;
}

//////////////////////////////////////////////////////////////////////////////
//
Config::Config() {
  m_dict = Dictionary::create();
  m_scale=1;
  init_assets();
  init_csts();
  init_levels();
}

//////////////////////////////////////////////////////////////////////////////
//
Ref* Config::GetCst(const string& key) {
  m_dict->objectForKey(key);
}

//////////////////////////////////////////////////////////////////////////////
//
const string Config::GetAtlas(const string& key) {
  return getXXX("atlases", key);
}

//////////////////////////////////////////////////////////////////////////////
//
const string GetFont(const string& key) {
  return getXXX("fonts", key);
}

//////////////////////////////////////////////////////////////////////////////
//
const string GetTile(const string& key) {
  return getXXX("tiles", key);
}

//////////////////////////////////////////////////////////////////////////////
//
const string GetImage(const string& key) {
  return getXXX("images", key);
}

//////////////////////////////////////////////////////////////////////////////
//
const string GetSound(const string& key) {
  return getXXX("sounds", key);
}

//////////////////////////////////////////////////////////////////////////////
//
Dictionary* Config::GetLevel(const string& n) {
  auto d= static_cast<Dictionary*>(m_dict->objectForKey("levels"));
  auto r= static_cast<Dictionary*>(d->objectForKey(n));
  return r;
}

//////////////////////////////////////////////////////////////////////////////
//
Dictionary* Config::GetLevelCfg(const string& n) {
  auto d= static_cast<Dictionary*>(m_dict->objectForKey("levels"));
  auto r= static_cast<Dictionary*>(d->objectForKey(n));
  if (r != nullptr) {
    return static_cast<Dictionary*>(r->objectForKey("cfg"));
  } else {
    return nullptr;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
const Size Config::GetGameSize() {
  return Size(320, 480);
}

//////////////////////////////////////////////////////////////////////////////
//
float Config::GetScale() {
  return m_scale;
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::HandleResolution(const Size& rs) {
  //for default font, we use 48pt
  m_scale = 52/256 * rs.width /320;
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::RunOnce() {
  auto c= SpriteFrameCache::getInstance();
  c->addSpriteFrames( GetAtlas("game-pics"));
  c->addSpriteFrames( GetAtlas("lang-pics"));
}

//////////////////////////////////////////////////////////////////////////////
//
XConfig* Config::getInstance() {
  if (singleton == nullptr) {
    singleton= new Config();
  }
  return singleton;
}







NS_FI_END

