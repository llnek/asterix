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

#include "audio/include/SimpleAudioEngine.h"
#include "XConfig.h"
NS_ALIAS(den, CocosDenshion)
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////////
//
const stdstr XConfig::ATLASES= "atlases";
const stdstr XConfig::LEVELS= "levels";
const stdstr XConfig::FONTS= "fonts";
const stdstr XConfig::TILES= "tiles";
const stdstr XConfig::POOLS= "pools";
const stdstr XConfig::IMAGES= "images";
const stdstr XConfig::SOUNDS= "sounds";
const stdstr XConfig::CSTS= "csts";
const stdstr XConfig::CFG= "cfg";

static XConfig* singleton;
//////////////////////////////////////////////////////////////////////////////
//
static const stdstr getXXX(not_null<c::Dictionary*> d,
    const stdstr& key ) {

  auto r= DictVal<c::String>(d,key);
  return NNP(r) ? r->getCString() : "";
}

//////////////////////////////////////////////////////////////////////////////
//
void XConfig::Bind(not_null<XConfig*> c) {
  singleton=c;
}

//////////////////////////////////////////////////////////////////////////////
//
XConfig* XConfig::Self() {
  return singleton;
}

//////////////////////////////////////////////////////////////////////////////
//
XConfig::~XConfig() {
  for (auto it=pools.begin(); it != pools.end(); ++it) {
    delete it->second;
  }
  CC_DROP(dict)
  //pools.clear();
}

//////////////////////////////////////////////////////////////////////////////
//
XConfig::XConfig() {
  dict = c::Dictionary::create();
  CC_KEEP(dict)
  dict->setObject(c::Dictionary::create(), ATLASES);
  dict->setObject(c::Dictionary::create(), TILES);
  dict->setObject(c::Dictionary::create(), CSTS);
  dict->setObject(c::Dictionary::create(), IMAGES);
  dict->setObject(c::Dictionary::create(), FONTS);
  dict->setObject(c::Dictionary::create(), SOUNDS);
  dict->setObject(c::Dictionary::create(), LEVELS);

  AddLevel("1");
  SetCsts();
  den::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.5);
  den::SimpleAudioEngine::getInstance()->setEffectsVolume(0.5);
}

//////////////////////////////////////////////////////////////////////////////
//
void XConfig::SetCsts() {
  auto f = GetFragment(CSTS);

  f->setObject(c::Bool::create(true), "showFPS");
  f->setObject(c::Integer::create(60), "FPS");
  f->setObject(c::Integer::create(8), "TILE");
  f->setObject(c::Integer::create(4), "S_OFF");

  f->setObject(c::String::create("O"), "P2_COLOR");
  f->setObject(c::String::create("X"), "P1_COLOR");

  f->setObject(c::Integer::create(79), "CV_O");
  f->setObject(c::Integer::create(88), "CV_X");
}

//////////////////////////////////////////////////////////////////////////////
//
void XConfig::ToggleAudio(bool b) {
  audioState= b;
  if (! b) {
    SetMusicVolume(0);
    SetVolume(0);
  } else {
    SetMusicVolume(lastMusicVol);
    SetVolume(lastSfxVol);
  }
}

///////////////////////////////////////////////////////////////////////////////
//
bool XConfig::HasAudio() {
  return audioState;
}

//////////////////////////////////////////////////////////////////////////////
//
float XConfig::SetMusicVolume(float v) {
  lastMusicVol= GetMusicVolume();
  den::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(v);
}

//////////////////////////////////////////////////////////////////////////////
//
float XConfig::SetVolume(float v) {
  lastSfxVol = GetVolume();
  den::SimpleAudioEngine::getInstance()->setEffectsVolume(v);
}

//////////////////////////////////////////////////////////////////////////////
//
float XConfig::GetMusicVolume() {
  return den::SimpleAudioEngine::getInstance()->getBackgroundMusicVolume();
}

//////////////////////////////////////////////////////////////////////////////
//
float XConfig::GetVolume() {
  return den::SimpleAudioEngine::getInstance()->getEffectsVolume();
}

//////////////////////////////////////////////////////////////////////////////
//
c::Ref* XConfig::GetCst(const stdstr& key) {
  return GetFragment(CSTS)->objectForKey(key);
}

//////////////////////////////////////////////////////////////////////////////
//
const stdstr XConfig::GetAtlas(const stdstr& key) {
  return getXXX(GetFragment(ATLASES), key);
}

//////////////////////////////////////////////////////////////////////////////
//
const stdstr XConfig::GetFont(const stdstr& key) {
  return getXXX(GetFragment(FONTS), key);
}

//////////////////////////////////////////////////////////////////////////////
//
const stdstr XConfig::GetTile(const stdstr& key) {
  return getXXX(GetFragment(TILES), key);
}

//////////////////////////////////////////////////////////////////////////////
//
const stdstr XConfig::GetImage(const stdstr& key) {
  return getXXX(GetFragment(IMAGES), key);
}

//////////////////////////////////////////////////////////////////////////////
//
const stdstr XConfig::GetSound(const stdstr& key) {
  return getXXX(GetFragment(SOUNDS), key);
}

//////////////////////////////////////////////////////////////////////////////
//
XPool* XConfig::GetPool(const stdstr& key) {
  auto it = pools.find(key);
  if (it != pools.end()) {
    return it->second;
  } else {
    return nullptr;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
owner<XPool*> XConfig::CreatePool(const stdstr& key) {
  auto p = new XPool();
  pools.insert(s::pair<stdstr, XPool*>(key, p));
  return p;
}

//////////////////////////////////////////////////////////////////////////////
//
c::Dictionary* XConfig::GetLevel(const stdstr& n) {
  auto d= GetFragment(LEVELS);
  return SCAST(c::Dictionary*,  d->objectForKey(n));
}

//////////////////////////////////////////////////////////////////////////////
//
c::Dictionary* XConfig::GetLevelCfg(const stdstr& n) {
  auto r= GetLevel(n);
  if (NNP(r)) {
    return SCAST(c::Dictionary*, r->objectForKey(CFG));
  } else {
    return nullptr;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
c::Dictionary* XConfig::AddLevel(const stdstr& level) {

  auto d2= c::Dictionary::create();
  GetFragment(LEVELS)->setObject(d2, level);

  d2->setObject(c::Dictionary::create(), IMAGES);
  d2->setObject(c::Dictionary::create(), TILES);
  d2->setObject(c::Dictionary::create(), CFG);
  return d2;
}
///////////////////////////////////////////////////////////////////////////////
//
c::Dictionary* XConfig::GetFragment(const stdstr& key) {
  auto obj = dict->objectForKey(key);
  return NNP(obj) ? SCAST(c::Dictionary*, obj) : nullptr;
}

NS_END(fusii)

