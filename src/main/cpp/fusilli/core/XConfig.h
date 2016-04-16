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

#pragma once
//////////////////////////////////////////////////////////////////////////////

#define XCFG() fusii::XConfig::self()
#include "platform/CCCommon.h"
#include "platform/CCGLView.h"
#include "Primitives.h"
NS_BEGIN(fusii)

//ipod touch 1136 * 640

//////////////////////////////////////////////////////////////////////////
//
class CC_DLL XConfig {
protected:

  void addXXX(const sstr &frag, const sstr &key, c::Ref*);
  c::Dictionary* getFragment(const sstr&);

  void addAtlas(const sstr& , c::Ref*);
  void addFont(const sstr& , c::Ref*);
  void addTile(const sstr& , c::Ref*);
  void addImage(const sstr& , c::Ref*);
  void addEffect(const sstr& , c::Ref*);
  void addMusic(const sstr& , c::Ref*);
  void addCst(const sstr& , c::Ref*);
  void addColor(const sstr& , c::Ref*);

  __decl_ptr(c::Dictionary, _frags)
  __decl_ptr(c::Dictionary, _l10n)
  __decl_nocpyass(XConfig)

  __decl_md(c::ApplicationProtocol::Platform, _platform)
  __decl_mv(float, _lastMusicVol,0.5)
  __decl_mv(float, _lastSfxVol, 0.5)
  __decl_mv(float, _scale, 1)
  __decl_md(sstr, _room_id)
  __decl_md(sstr, _app_id)
  __decl_md(sstr, _game_id)
  __decl_bt(_audioState)
  __decl_md(CCT_SZ, _frame)

  void cacheSprites(const sstr &plist);
  void loadL10NStrings();
  void setCsts();

  sstr ATLASES;
  sstr LEVELS;
  sstr FONTS;
  sstr TILES;
  sstr IMAGES;
  sstr COLORS;
  sstr MUSIC;
  sstr CSTS;
  sstr EFX;
  sstr CFG;

public:

  virtual float scaleBmfont(not_null<c::Node*>,float);
  virtual void handleResolution(const CCT_SZ& ) {}
  virtual void setFrameSize(const CCT_SZ&);

  virtual float scaleFont(float pt) = 0;

  virtual float scaleTTF(float pt) {
    return _scale * pt; }

  virtual float getScale() { return _scale; }
  virtual void runOnce() {}

  virtual ResolutionPolicy policy() = 0;
  virtual const CCT_SZ gameSize() = 0;

  virtual const sstr getWSUrl() {
    return "";
  }

  virtual c::Scene* prelude() = 0;

  virtual bool isPortrait() { return gameSize().height > gameSize().width; }
  virtual const CCT_SZ fit(const CCT_SZ &z);

  __decl_gsms(c::ApplicationProtocol::Platform, _platform, Platform)
  __decl_gsms(sstr, _room_id, RoomId)
  __decl_getr(sstr, _app_id, AppId)
  __decl_getr(sstr, _game_id, GameId)

  const filepath getAtlas(const sstr& );
  const filepath getFont(const sstr& );
  const filepath getTile(const sstr& );
  const filepath getImage(const sstr& );

  const filepath getEffect(const sstr& );
  const filepath getMusic(const sstr& );

  const sstr getL10NStr(const sstr& , const s_vec<sstr>& );
  const sstr getL10NStr(const sstr& );

  const c::Color3B getColor(const sstr&);
  const sstr getColorStr(const sstr& );

  void resetCst(const sstr&, c::Ref*);
  c::Ref* getCst(const sstr& );

  virtual float fit(not_null<c::Node*> n) {
    n->setScale(_scale); return _scale;}

  int getBtnPadding();

  void toggleAudio(bool s);
  bool hasAudio();

  void setMusicVolume(float);
  void setVolume(float);

  float getMusicVolume();
  float getVolume();

  c::Dictionary* getLevel(const sstr& );
  c::Dictionary* addLevel(const sstr& );
  f::JsonObj* getLevelCfg(const sstr& );

  const s_vec<filepath> getEffectFiles();
  const s_vec<filepath> getMusicFiles();

  virtual ~XConfig();
  XConfig();

  static XConfig* self();
  static void bind(not_null<XConfig*>);
};

//////////////////////////////////////////////////////////////////////////////
//
template<typename T> T* cstVal(const sstr &key) {
  auto r= fusii::XConfig::self()->getCst(key);
  if (N_NIL(r)) {
    return static_cast<T*>( r);
  } else {
    return CC_NIL;
  }
}


NS_END




