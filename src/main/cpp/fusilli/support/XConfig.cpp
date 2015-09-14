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

#include "support/XConfig.h"
NS_BEGIN(fusilli)

//////////////////////////////////////////////////////////////////////////////
//
static XConfig* singleton;

//////////////////////////////////////////////////////////////////////////////
//
static const s::string getXXX(cc::Dictionary* d,
    const s::string& key ) {

  auto r= DictVal<cc::String>(d,key);
  return NNP(r) ? r->getCString() : "";
}

//////////////////////////////////////////////////////////////////////////////
//
void XConfig::SetInstance(XConfig* c) {
  singleton=c;
}

//////////////////////////////////////////////////////////////////////////////
//
XConfig* XConfig::GetInstance() {
  return singleton;
}

//////////////////////////////////////////////////////////////////////////////
//
XConfig::~XConfig() {
  dict->release();
  for (auto it=pools.begin(); it != pools.end(); ++it) {
    delete it->second;
  }
  pools.clear();
}

//////////////////////////////////////////////////////////////////////////////
//
XConfig::XConfig() {
  dict = cc::Dictionary::create();
  dict->retain();
  dict->setObject(cc::Dictionary::create(), ATLASES);
  dict->setObject(cc::Dictionary::create(), TILES);
  dict->setObject(cc::Dictionary::create(), CSTS);
  dict->setObject(cc::Dictionary::create(), IMAGES);
  dict->setObject(cc::Dictionary::create(), FONTS);
  dict->setObject(cc::Dictionary::create(), SOUNDS);
  dict->setObject(cc::Dictionary::create(), LEVELS);
  AddLevel("1");
}

//////////////////////////////////////////////////////////////////////////////
//
cc::Ref* XConfig::GetCst(const s::string& key) {
  return GetFragment(CSTS)->objectForKey(key);
}

//////////////////////////////////////////////////////////////////////////////
//
const s::string XConfig::GetAtlas(const s::string& key) {
  return getXXX(GetFragment(ATLASES), key);
}

//////////////////////////////////////////////////////////////////////////////
//
const s::string XConfig::GetFont(const s::string& key) {
  return getXXX(GetFragment(FONTS), key);
}

//////////////////////////////////////////////////////////////////////////////
//
const s::string XConfig::GetTile(const s::string& key) {
  return getXXX(GetFragment(TILES), key);
}

//////////////////////////////////////////////////////////////////////////////
//
const s::string XConfig::GetImage(const s::string& key) {
  return getXXX(GetFragment(IMAGES), key);
}

//////////////////////////////////////////////////////////////////////////////
//
const s::string XConfig::GetSound(const s::string& key) {
  return getXXX(GetFragment(SOUNDS), key);
}

//////////////////////////////////////////////////////////////////////////////
//
XPool* XConfig::GetPool(const s::string& key) {
  auto it = pools.find(key);
  if (it != pools.end()) {
    return it->second;
  } else {
    return nullptr;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
XPool* XConfig::AddPool(const s::string& key) {
  auto p = XPool::Create();
  pools.insert(pair<s::string, XPool*>(key, p));
  return p;
}

//////////////////////////////////////////////////////////////////////////////
//
cc::Dictionary* XConfig::GetLevel(const s::string& n) {
  auto d= GetFragment(LEVELS);
  return SCAST(cc::Dictionary*, d->objectForKey(n));
}

//////////////////////////////////////////////////////////////////////////////
//
cc::Dictionary* XConfig::GetLevelCfg(const s::string& n) {
  auto r= GetLevel(n);
  if (NNP(r)) {
    return SCAST(cc::Dictionary*, r->objectForKey(CFG));
  } else {
    return nullptr;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
cc::Dictionary* XConfig::AddLevel(const s::string& level) {

  auto d2= cc::Dictionary::create();
  GetFragment(LEVELS)->setObject(d2, level);

  d2->setObject(cc::Dictionary::create(), IMAGES);
  d2->setObject(cc::Dictionary::create(), TILES);
  d2->setObject(cc::Dictionary::create(), CFG);
  return d2;
}

//////////////////////////////////////////////////////////////////////////////
//
void XConfig::ToggleAudio(bool b) {

}

///////////////////////////////////////////////////////////////////////////////
//
bool XConfig::HasAudio() {
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void XConfig::HandleResolution(const cc::Size& rs) {
}

//////////////////////////////////////////////////////////////////////////////
//
void XConfig::RunOnce() {
}

//////////////////////////////////////////////////////////////////////////////
//
float XConfig::GetScale() { return 1; }



NS_END(fusilli)

