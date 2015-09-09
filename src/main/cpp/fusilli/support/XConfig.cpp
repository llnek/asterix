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

#include "XConfig.h"
NS_BEGIN(fusilli)

//////////////////////////////////////////////////////////////////////////////
//
static XConfig* singleton;

//////////////////////////////////////////////////////////////////////////////
//
static const string getXXX(Dictionary* d, const string& key ) {
  auto r= SCAST(String*,d->objectForKey(key));
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
}

//////////////////////////////////////////////////////////////////////////////
//
XConfig::XConfig() {
  dict = Dictionary::create();
  dict->retain();
  dict->setObject(Dictionary::create(), ATLASES);
  dict->setObject(Dictionary::create(), TILES);
  dict->setObject(Dictionary::create(), CSTS);
  dict->setObject(Dictionary::create(), IMAGES);
  dict->setObject(Dictionary::create(), FONTS);
  dict->setObject(Dictionary::create(), SOUNDS);
  dict->setObject(Dictionary::create(), LEVELS);
  AddLevel("1");
}

//////////////////////////////////////////////////////////////////////////////
//
Ref* XConfig::GetCst(const string& key) {
  return GetFragment(CSTS)->objectForKey(key);
}

//////////////////////////////////////////////////////////////////////////////
//
const string XConfig::GetAtlas(const string& key) {
  return getXXX(GetFragment(ATLASES), key);
}

//////////////////////////////////////////////////////////////////////////////
//
const string XConfig::GetFont(const string& key) {
  return getXXX(GetFragment(FONTS), key);
}

//////////////////////////////////////////////////////////////////////////////
//
const string XConfig::GetTile(const string& key) {
  return getXXX(GetFragment(TILES), key);
}

//////////////////////////////////////////////////////////////////////////////
//
const string XConfig::GetImage(const string& key) {
  return getXXX(GetFragment(IMAGES), key);
}

//////////////////////////////////////////////////////////////////////////////
//
const string XConfig::GetSound(const string& key) {
  return getXXX(GetFragment(SOUNDS), key);
}

//////////////////////////////////////////////////////////////////////////////
//
Dictionary* XConfig::GetLevel(const string& n) {
  auto d= GetFragment(LEVELS);
  return SCAST(Dictionary*, d->objectForKey(n));
}

//////////////////////////////////////////////////////////////////////////////
//
Dictionary* XConfig::GetLevelCfg(const string& n) {
  auto r= GetLevel(n);
  if (NNP(r)) {
    return SCAST(Dictionary*, r->objectForKey(CFG));
  } else {
    return nullptr;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
Dictionary* XConfig::AddLevel(const string& level) {

  auto d2= Dictionary::create();
  GetFragment(LEVELS)->setObject(d2, level);

  d2->setObject(Dictionary::create(), IMAGES);
  d2->setObject(Dictionary::create(), TILES);
  d2->setObject(Dictionary::create(), CFG);
  return d2;
}

//////////////////////////////////////////////////////////////////////////////
//
void XConfig::HandleResolution(const Size& rs) {
}

//////////////////////////////////////////////////////////////////////////////
//
void XConfig::RunOnce() {
}

//////////////////////////////////////////////////////////////////////////////
//
float XConfig::GetScale() { return 1; }



NS_END(fusilli)

