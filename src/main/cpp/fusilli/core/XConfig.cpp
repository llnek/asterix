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
#include "CCSX.h"
#include "XConfig.h"
NS_ALIAS(den, CocosDenshion)
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(fusii)

const sstr XConfig::ATLASES= "atlases";
const sstr XConfig::LEVELS= "levels";
const sstr XConfig::FONTS= "fonts";
const sstr XConfig::TILES= "tiles";
const sstr XConfig::IMAGES= "images";
const sstr XConfig::COLORS= "colors";
const sstr XConfig::MUSIC= "music";
const sstr XConfig::EFX= "effects";
const sstr XConfig::CSTS= "csts";
const sstr XConfig::CFG= "cfg";

//////////////////////////////////////////////////////////////////////////////
//
BEGIN_NS_UNAMED()

XConfig *singleton;
//////////////////////////////////////////////////////////////////////////////
//
const sstr getXXX(not_null<c::Dictionary*> d, const sstr &key ) {
  auto r= f::dictVal<cocos2d::String>(d,key);
  return NNP(r) ? r->getCString() : "";
}


END_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
void XConfig::bind(not_null<XConfig*> c) {
  singleton=c;
}

//////////////////////////////////////////////////////////////////////////////
//
XConfig* XConfig::self() {
  return singleton;
}

//////////////////////////////////////////////////////////////////////////////
//
XConfig::~XConfig() {
  CC_DROP(frags)
  CC_DROP(l10n)
}

//////////////////////////////////////////////////////////////////////////////
//
XConfig::XConfig() {
  frags = CC_DICT();
  CC_KEEP(frags)

  frags->setObject(CC_DICT(), ATLASES);
  frags->setObject(CC_DICT(), TILES);
  frags->setObject(CC_DICT(), CSTS);
  frags->setObject(CC_DICT(), IMAGES);
  frags->setObject(CC_DICT(), COLORS);
  frags->setObject(CC_DICT(), FONTS);
  frags->setObject(CC_DICT(), MUSIC);
  frags->setObject(CC_DICT(), EFX);
  frags->setObject(CC_DICT(), LEVELS);

  loadL10NStrings();
  addLevel("1");
  setCsts();
  bind(this);
}

//////////////////////////////////////////////////////////////////////////////
//
void XConfig::loadL10NStrings() {
  auto b= cx::readXmlAsDict("i18n/base_strings.plist");
  auto d= cx::readXmlAsDict("i18n/strings.plist");
  NS_USING(cocos2d)
  DictElement *e= nullptr;
  CCDICT_FOREACH(d, e) {
    auto langObj = e->getObject();
    auto key= e->getStrKey();
    auto d2= b->objectForKey(key);
    if (ENP(d2)) {
      d2= CC_DICT();
      b->setObject(d2, key);
    }
    cx::mergeDict((c::Dictionary*)d2, (c::Dictionary*)langObj);
  }

  CC_KEEP(b)
  l10n = b;
  CCLOG("loaded L10N strings");
}

//////////////////////////////////////////////////////////////////////////////
//
const sstr XConfig::getL10NStr(const sstr &key, const s_vec<sstr> &pms) {
  auto vs = getL10NStr(key);
  if (pms.size() == 0) { return vs; }
  auto pos= vs.find("{}");
  int n= 0;
  while (pos != s::string::npos) {
    vs= vs.substr(0, pos) + pms[n] + vs.substr(pos+2);
    ++n;
    pos= vs.find("{}");
  }
  return vs;
}

//////////////////////////////////////////////////////////////////////////////
//
const sstr XConfig::getL10NStr(const sstr &key) {
  sstr lang= c::Application::getInstance()->getCurrentLanguageCode();
  auto d = dictVal<c::Dictionary>(l10n, lang);
  auto obj=NNP(d) ?
    dictVal<c::String>(d, key) : nullptr;

  if (NNP(obj)) {
    return obj->getCString();
  } else {
    return "";
  }
}

//////////////////////////////////////////////////////////////////////////////
//
int XConfig::getBtnPadding() {
  return CC_CSV(c::Integer, "BTN_PADDING");
}

//////////////////////////////////////////////////////////////////////////////
//
void XConfig::setCsts() {
  auto f = getFragment(CSTS);

  f->setObject(CC_INT(10), "BTN_PADDING");

  f->setObject(CC_BOOL(true), "showFPS");
  f->setObject(CC_INT(60), "FPS");

  f->setObject(CC_INT(4), "S_OFF");
  f->setObject(CC_INT(8), "TILE");

  f->setObject(CC_STR("O"), "P2_COLOR");
  f->setObject(CC_STR("X"), "P1_COLOR");

  f->setObject(CC_INT(79), "CV_O");
  f->setObject(CC_INT(88), "CV_X");

  f->setObject(CC_INT(9091), "NETP");
  f->setObject(CC_INT(9901), "BOT");
  f->setObject(CC_INT(1099), "HUMAN");

}

//////////////////////////////////////////////////////////////////////////////
//
void XConfig::toggleAudio(bool b) {
  audioState= b;
  if (! b) {
    setMusicVolume(0);
    setVolume(0);
  } else {
    setMusicVolume(lastMusicVol);
    setVolume(lastSfxVol);
  }
}

///////////////////////////////////////////////////////////////////////////////
//
bool XConfig::hasAudio() {
  return audioState;
}

//////////////////////////////////////////////////////////////////////////////
//
void XConfig::setMusicVolume(float v) {
  lastMusicVol= getMusicVolume();
  den::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(v);
}

//////////////////////////////////////////////////////////////////////////////
//
void XConfig::setVolume(float v) {
  lastSfxVol = getVolume();
  den::SimpleAudioEngine::getInstance()->setEffectsVolume(v);
}

//////////////////////////////////////////////////////////////////////////////
//
float XConfig::getMusicVolume() {
  return den::SimpleAudioEngine::getInstance()->getBackgroundMusicVolume();
}

//////////////////////////////////////////////////////////////////////////////
//
float XConfig::getVolume() {
  return den::SimpleAudioEngine::getInstance()->getEffectsVolume();
}

//////////////////////////////////////////////////////////////////////////////
//
c::Ref* XConfig::getCst(const sstr &key) {
  return getFragment(CSTS)->objectForKey(key);
}

//////////////////////////////////////////////////////////////////////////////
//
const filepath XConfig::getAtlas(const sstr &key) {
  return getXXX(getFragment(ATLASES), key);
}

//////////////////////////////////////////////////////////////////////////////
//
const filepath XConfig::getFont(const sstr &key) {
  return getXXX(getFragment(FONTS), key);
}

//////////////////////////////////////////////////////////////////////////////
//
const filepath XConfig::getTile(const sstr &key) {
  return getXXX(getFragment(TILES), key);
}

//////////////////////////////////////////////////////////////////////////////
//
const c::Color3B XConfig::getColor(const sstr &key) {
  return cx::colorRGB(  getXXX(getFragment(COLORS), key));
}

//////////////////////////////////////////////////////////////////////////////
//
const sstr XConfig::getColorStr(const sstr &key) {
  return getXXX(getFragment(COLORS), key);
}

//////////////////////////////////////////////////////////////////////////////
//
const filepath XConfig::getImage(const sstr &key) {
  return getXXX(getFragment(IMAGES), key);
}

//////////////////////////////////////////////////////////////////////////////
//
const filepath XConfig::getMusic(const sstr &key) {
  return getXXX(getFragment(MUSIC), key);
}

//////////////////////////////////////////////////////////////////////////////
//
const filepath XConfig::getEffect(const sstr &key) {
  return getXXX(getFragment(EFX), key);
}

//////////////////////////////////////////////////////////////////////////////
//
c::Dictionary* XConfig::getLevel(const sstr &n) {
  auto d= getFragment(LEVELS);
  return SCAST(c::Dictionary*,  d->objectForKey(n));
}

//////////////////////////////////////////////////////////////////////////////
//
f::JsonObj* XConfig::getLevelCfg(const sstr &n) {
  auto r= getLevel(n);
  if (NNP(r)) {
    return SCAST(f::JsonObj*, r->objectForKey(CFG));
  } else {
    return nullptr;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
c::Dictionary* XConfig::addLevel(const sstr &level) {

  auto d2= CC_DICT();

  d2->setObject(f::JsonObj::create(), CFG);
  d2->setObject(CC_DICT(), IMAGES);
  d2->setObject(CC_DICT(), TILES);

  getFragment(LEVELS)->setObject(d2, level);
  return d2;
}

///////////////////////////////////////////////////////////////////////////////
//
c::Dictionary* XConfig::getFragment(const sstr &key) {
  auto obj = frags->objectForKey(key);
  return NNP(obj) ? SCAST(c::Dictionary*, obj) : nullptr;
}

//////////////////////////////////////////////////////////////////////////////
//
void XConfig::addAtlas(const sstr &key, c::Ref *ref) {
  addXXX(ATLASES, key, ref);
}

//////////////////////////////////////////////////////////////////////////////
//
void XConfig::addFont(const sstr &key, c::Ref *ref) {
  addXXX(FONTS, key, ref);
}

//////////////////////////////////////////////////////////////////////////////
//
void XConfig::addTile(const sstr &key, c::Ref *ref) {
  addXXX(TILES, key, ref);
}

//////////////////////////////////////////////////////////////////////////////
//
void XConfig::addImage(const sstr &key, c::Ref *ref) {
  addXXX(IMAGES, key, ref);
}

//////////////////////////////////////////////////////////////////////////////
//
void XConfig::addEffect(const sstr &key, c::Ref *ref) {
  addXXX(EFX, key, ref);
}

//////////////////////////////////////////////////////////////////////////////
//
void XConfig::addMusic(const sstr &key, c::Ref *ref) {
  addXXX(MUSIC, key, ref);
}

//////////////////////////////////////////////////////////////////////////////
//
void XConfig::addCst(const sstr &key, c::Ref *ref) {
  addXXX(CSTS, key, ref);
}

//////////////////////////////////////////////////////////////////////////////
//
void XConfig::addColor(const sstr &key, c::Ref *ref) {
  addXXX(COLORS, key, ref);
}

//////////////////////////////////////////////////////////////////////////////
//
void XConfig::addXXX(const sstr &frag, const sstr &key, c::Ref *ref) {
  auto d = getFragment(frag);
  d->setObject(ref, key);
}

//////////////////////////////////////////////////////////////////////////////
//
const s_vec<filepath> XConfig::getEffectFiles() {
  NS_USING(cocos2d)
  DictElement *em = nullptr;
  auto d= getFragment(MUSIC);
  s_vec<filepath> rc;

  CCDICT_FOREACH(d, em) {
    rc.push_back( static_cast<String*>(em->getObject())->getCString());
  }
  return rc;
}

//////////////////////////////////////////////////////////////////////////////
//
const s_vec<filepath> XConfig::getMusicFiles() {
  NS_USING(cocos2d)
  DictElement *em = nullptr;
  auto d= getFragment(EFX);
  s_vec<filepath> rc;

  CCDICT_FOREACH(d, em) {
    rc.push_back( static_cast<String*>(em->getObject())->getCString());
  }
  return rc;
}





NS_END(fusii)

