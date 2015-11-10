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

//////////////////////////////////////////////////////////////////////////////
//
BEGIN_NS_UNAMED()
const stdstr ATLASES= "atlases";
const stdstr LEVELS= "levels";
const stdstr FONTS= "fonts";
const stdstr TILES= "tiles";
const stdstr IMAGES= "images";
const stdstr MUSIC= "music";
const stdstr EFX= "effects";
const stdstr CSTS= "csts";
const stdstr CFG= "cfg";
XConfig* singleton;
//////////////////////////////////////////////////////////////////////////////
//
const filepath getXXX(not_null<c::Dictionary*> d, const stdstr& key ) {
  auto r= f::DictVal<cocos2d::String>(d,key);
  return NNP(r) ? r->getCString() : "";
}
END_NS_UNAMED()

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
  frags->setObject(CC_DICT(), FONTS);
  frags->setObject(CC_DICT(), MUSIC);
  frags->setObject(CC_DICT(), EFX);
  frags->setObject(CC_DICT(), LEVELS);

  LoadL10NStrings();

  AddLevel("1");
  SetCsts();
}


//////////////////////////////////////////////////////////////////////////////
//
void XConfig::LoadL10NStrings() {
  auto b= cx::ReadXmlAsDict("i18n/base_strings.plist");
  auto d= cx::ReadXmlAsDict("i18n/strings.plist");
  NS_USING(cocos2d)
  DictElement* e= nullptr;
  CCDICT_FOREACH(d, e) {
    auto obj = e->getObject();
    auto key= e->getStrKey();
    b->setObject(obj, key);
  }

  CC_KEEP(b)
  l10n = b;
  CCLOG("loaded L10N strings");
}

//////////////////////////////////////////////////////////////////////////////
//
const stdstr XConfig::GetL10NStr(const stdstr& key,
    const s::vector<stdstr>& pms) {
  auto vs = GetL10NStr(key);
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
const stdstr XConfig::GetL10NStr(const stdstr& key) {
  stdstr lang= c::Application::getInstance()->getCurrentLanguageCode();
  auto d = DictVal<c::Dictionary>(l10n, lang);
  auto obj=NNP(d) ?
    DictVal<c::String>(d, key) : nullptr;

  if (NNP(obj)) {
    return obj->getCString();
  } else {
    return "";
  }
}

//////////////////////////////////////////////////////////////////////////////
//
int XConfig::GetBtnPadding() {
  return CC_CSV(c::Integer,"BTN_PADDING");
}

//////////////////////////////////////////////////////////////////////////////
//
void XConfig::SetCsts() {
  auto f = GetFragment(CSTS);

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
void XConfig::SetMusicVolume(float v) {
  lastMusicVol= GetMusicVolume();
  den::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(v);
}

//////////////////////////////////////////////////////////////////////////////
//
void XConfig::SetVolume(float v) {
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
const filepath XConfig::GetAtlas(const stdstr& key) {
  return getXXX(GetFragment(ATLASES), key);
}

//////////////////////////////////////////////////////////////////////////////
//
const filepath XConfig::GetFont(const stdstr& key) {
  return getXXX(GetFragment(FONTS), key);
}

//////////////////////////////////////////////////////////////////////////////
//
const filepath XConfig::GetTile(const stdstr& key) {
  return getXXX(GetFragment(TILES), key);
}

//////////////////////////////////////////////////////////////////////////////
//
const filepath XConfig::GetImage(const stdstr& key) {
  return getXXX(GetFragment(IMAGES), key);
}

//////////////////////////////////////////////////////////////////////////////
//
const filepath XConfig::GetMusic(const stdstr& key) {
  return getXXX(GetFragment(MUSIC), key);
}

//////////////////////////////////////////////////////////////////////////////
//
const filepath XConfig::GetEffect(const stdstr& key) {
  return getXXX(GetFragment(EFX), key);
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

//////////////////////////////////////////////////////////////////////////
//
void XConfig::SetSeedData(j::Json& j) {
  seed=j;
  j= j::Json();
}

//////////////////////////////////////////////////////////////////////////
//
j::Json& XConfig::GetSeedData() {
  return seed;
}

//////////////////////////////////////////////////////////////////////////////
//
c::Dictionary* XConfig::AddLevel(const stdstr& level) {

  auto d2= CC_DICT();

  d2->setObject(CC_DICT(), IMAGES);
  d2->setObject(CC_DICT(), TILES);
  d2->setObject(CC_DICT(), CFG);

  GetFragment(LEVELS)->setObject(d2, level);
  return d2;
}

///////////////////////////////////////////////////////////////////////////////
//
c::Dictionary* XConfig::GetFragment(const stdstr& key) {
  auto obj = frags->objectForKey(key);
  return NNP(obj) ? SCAST(c::Dictionary*, obj) : nullptr;
}

//////////////////////////////////////////////////////////////////////////////
//
void XConfig::AddAtlas(const stdstr& key, c::Ref* ref) {
  AddXXX(ATLASES, key, ref);
}

//////////////////////////////////////////////////////////////////////////////
//
void XConfig::AddFont(const stdstr& key, c::Ref* ref) {
  AddXXX(FONTS, key, ref);
}

//////////////////////////////////////////////////////////////////////////////
//
void XConfig::AddTile(const stdstr& key, c::Ref* ref) {
  AddXXX(TILES, key, ref);
}

//////////////////////////////////////////////////////////////////////////////
//
void XConfig::AddImage(const stdstr& key, c::Ref* ref) {
  AddXXX(IMAGES, key, ref);
}

//////////////////////////////////////////////////////////////////////////////
//
void XConfig::AddEffect(const stdstr& key, c::Ref* ref) {
  AddXXX(EFX, key, ref);
}

//////////////////////////////////////////////////////////////////////////////
//
void XConfig::AddMusic(const stdstr& key, c::Ref* ref) {
  AddXXX(MUSIC, key, ref);
}

//////////////////////////////////////////////////////////////////////////////
//
void XConfig::AddCst(const stdstr& key, c::Ref* ref) {
  AddXXX(CSTS, key, ref);
}

//////////////////////////////////////////////////////////////////////////////
//
void XConfig::AddXXX(const stdstr& frag, const stdstr& key, c::Ref* ref) {
  auto d = GetFragment(frag);
  d->setObject(ref, key);
}

//////////////////////////////////////////////////////////////////////////////
//
const s::vector<filepath> XConfig::GetEffectFiles() {
  NS_USING(cocos2d)
  DictElement* element = nullptr;
  auto d= GetFragment(MUSIC);
  s::vector<filepath> rc;

  CCDICT_FOREACH(d, element) {
    rc.push_back( static_cast<String*>(element->getObject())->getCString());
  }
  return rc;
}

//////////////////////////////////////////////////////////////////////////////
//
const s::vector<filepath> XConfig::GetMusicFiles() {
  NS_USING(cocos2d)
  DictElement* element = nullptr;
  auto d= GetFragment(EFX);
  s::vector<filepath> rc;

  CCDICT_FOREACH(d, element) {
    rc.push_back( static_cast<String*>(element->getObject())->getCString());
  }
  return rc;
}





NS_END(fusii)

