// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Kenneth Leung. All rights reserved.

#include "CCSX.h"
#include "XConfig.h"
NS_ALIAS(den, CocosDenshion)
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(fusii)

  /*
sstr XConfig::ATLASES= "atlases";
sstr XConfig::LEVELS= "levels";
sstr XConfig::FONTS= "fonts";
sstr XConfig::TILES= "tiles";
sstr XConfig::IMAGES= "images";
sstr XConfig::COLORS= "colors";
sstr XConfig::MUSIC= "music";
sstr XConfig::EFX= "effects";
sstr XConfig::CSTS= "csts";
sstr XConfig::CFG= "cfg";
*/

//////////////////////////////////////////////////////////////////////////////
//
BEGIN_NS_UNAMED

XConfig *SGT;

//////////////////////////////////////////////////////////////////////////////
//
const sstr getXXX(not_null<c::Dictionary*> d, const sstr &key) {
  auto r= f::dictVal<cocos2d::String>(d,key);
  return N_NIL(r) ? r->getCString() : "";
}

END_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
//
void XConfig::bind(not_null<XConfig*> c) {
  SGT=c;
}

//////////////////////////////////////////////////////////////////////////////
//
XConfig* XConfig::self() {
  return SGT;
}

//////////////////////////////////////////////////////////////////////////////
//
XConfig::~XConfig() {
  CC_DROP(_frags)
  CC_DROP(_l10n)
}

//////////////////////////////////////////////////////////////////////////////
//
XConfig::XConfig() {
  _frags = CC_DICT();
  CC_KEEP(_frags)

  ATLASES= "atlases";
  LEVELS= "levels";
  FONTS= "fonts";
  TILES= "tiles";
  IMAGES= "images";
  COLORS= "colors";
  MUSIC= "music";
  EFX= "effects";
  CSTS= "csts";
  CFG= "cfg";

  _frags->setObject(CC_DICT(), ATLASES);
  _frags->setObject(CC_DICT(), TILES);
  _frags->setObject(CC_DICT(), CSTS);
  _frags->setObject(CC_DICT(), IMAGES);
  _frags->setObject(CC_DICT(), COLORS);
  _frags->setObject(CC_DICT(), FONTS);
  _frags->setObject(CC_DICT(), MUSIC);
  _frags->setObject(CC_DICT(), EFX);
  _frags->setObject(CC_DICT(), LEVELS);

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
  DictElement *e= CC_NIL;
  CCDICT_FOREACH(d, e) {
    auto langObj = e->getObject();
    auto key= e->getStrKey();
    auto d2= b->objectForKey(key);
    if (E_NIL(d2)) {
      d2= CC_DICT();
      b->setObject(d2, key);
    }
    cx::mergeDict((c::Dictionary*)d2, (c::Dictionary*)langObj);
  }

  CC_KEEP(b)
  _l10n = b;
  CCLOG("loaded L10N strings");
}

//////////////////////////////////////////////////////////////////////////////
//
const sstr XConfig::getL10NStr(const sstr &key, const s_vec<sstr> &pms) {
  auto vs = getL10NStr(key);
  if (pms.size() == 0) { return vs; }
  auto pos= vs.find("{}");
  auto n= 0;
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
  auto d = dictVal<c::Dictionary>(_l10n, lang);
  auto obj=N_NIL(d) ? dictVal<c::String>(d, key) : CC_NIL;

  if (N_NIL(obj)) {
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

  f->setObject(CC_FLOAT(0.35), "SCENE_DELAY");
  f->setObject(CC_INT(10), "BTN_PADDING");

  f->setObject(CC_BOOL(false), "showFPS");
  f->setObject(CC_INT(60), "FPS");

  f->setObject(CC_FLOAT(4.0f), "S_OFF");
  f->setObject(CC_FLOAT(8.0f), "TILE");

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
  _audioState= b;
  if (! b) {
    setMusicVolume(0);
    setVolume(0);
  } else {
    setMusicVolume(_lastMusicVol);
    setVolume(_lastSfxVol);
  }
}

///////////////////////////////////////////////////////////////////////////////
//
bool XConfig::hasAudio() {
  return _audioState;
}

//////////////////////////////////////////////////////////////////////////////
//
void XConfig::setMusicVolume(float v) {
  _lastMusicVol= getMusicVolume();
  CC_AUDIO()->setBackgroundMusicVolume(v);
}

//////////////////////////////////////////////////////////////////////////////
//
void XConfig::setVolume(float v) {
  _lastSfxVol = getVolume();
  CC_AUDIO()->setEffectsVolume(v);
}

//////////////////////////////////////////////////////////////////////////////
//
float XConfig::getMusicVolume() {
  return CC_AUDIO()->getBackgroundMusicVolume();
}

//////////////////////////////////////////////////////////////////////////////
//
float XConfig::getVolume() {
  return CC_AUDIO()->getEffectsVolume();
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
  return cx::colorRGB(getXXX(getFragment(COLORS), key));
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
  if (N_NIL(r)) {
    return SCAST(f::JsonObj*, r->objectForKey(CFG));
  } else {
    return CC_NIL;
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
  auto obj = _frags->objectForKey(key);
  return N_NIL(obj) ? SCAST(c::Dictionary*, obj) : CC_NIL;
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

//////////////////////////////////////////////////////////////////////////
//
void XConfig::resetCst(const sstr &key, c::Ref *ref) {
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
  auto d= getFragment(MUSIC);
  s_vec<filepath> rc;

  DictElement *em = CC_NIL;
  CCDICT_FOREACH(d, em) {
    rc.push_back( static_cast<String*>(em->getObject())->getCString());
  }
  return rc;
}

//////////////////////////////////////////////////////////////////////////////
//
void XConfig::setFrameSize(const CCT_SZ &sz) {
  _scale= CC_DTOR()->getContentScaleFactor();
  _frame=sz;
  handleResolution(sz);
}

//////////////////////////////////////////////////////////////////////////////
//
float XConfig::scaleBmfont(not_null<c::Node*> n, float sz) {
  auto pt= scaleFont(sz);
  n->setScale(pt);
  return pt;
}

//////////////////////////////////////////////////////////////////////////////
//
const CCT_SZ XConfig::fit(const CCT_SZ &z) {
  return CCT_SZ(z.width * _scale,
                z.height * _scale);
}

//////////////////////////////////////////////////////////////////////////////
//
const s_vec<filepath> XConfig::getMusicFiles() {
  NS_USING(cocos2d)
  auto d= getFragment(EFX);
  s_vec<filepath> rc;

  DictElement *em = CC_NIL;
  CCDICT_FOREACH(d, em) {
    rc.push_back( static_cast<String*>(em->getObject())->getCString());
  }
  return rc;
}

//////////////////////////////////////////////////////////////////////////////
//
void XConfig::cacheSprites(const sstr &plist) {
  auto fp= getAtlas(plist);
  CC_SCAC()->addSpriteFramesWithFile( fp);
  CCLOG("loaded sprite-sheet: %s", fp.c_str());
}



NS_END



