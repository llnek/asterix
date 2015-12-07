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

#if !defined(__XCONFIG_H__)
#define __XCONFIG_H__

#define XCFG() fusii::XConfig::self()

#include "platform/CCCommon.h"
#include "platform/CCGLView.h"
#include "Primitives.h"
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////
//
class CC_DLL XConfig {
protected:

  void addXXX(const sstr& frag, const sstr& key, c::Ref*);
  c::Dictionary* getFragment(const sstr&);

  void addAtlas(const sstr& key, c::Ref*);
  void addFont(const sstr& key, c::Ref*);
  void addTile(const sstr& key, c::Ref*);
  void addImage(const sstr& key, c::Ref*);
  void addEffect(const sstr& key, c::Ref*);
  void addMusic(const sstr& key, c::Ref*);
  void addCst(const sstr& key, c::Ref*);

  c::Dictionary* frags;
  c::Dictionary* l10n;
  j::json seed;

  float lastMusicVol;
  float lastSfxVol;
  bool audioState;

  void loadL10NStrings();
  void setCsts();

  static const sstr ATLASES;
  static const sstr LEVELS;
  static const sstr FONTS;
  static const sstr TILES;
  static const sstr IMAGES;
  static const sstr MUSIC;
  static const sstr EFX;
  static const sstr CSTS;
  static const sstr CFG;

public:

  virtual void handleResolution(const c::Size& ) {}
  virtual float getScale() { return 1.0f; }
  virtual void runOnce() {}

  virtual ResolutionPolicy policy() = 0;
  virtual const sstr appKey() = 0;
  virtual const sstr appId() = 0;
  virtual const sstr color() = 0;
  virtual const c::Size gameSize() = 0;

  virtual c::Scene* startWith() = 0;
  virtual const sstr getWSUrl() = 0;

  virtual void setGameId(const sstr& ) = 0;
  virtual void setRoomId(const sstr& ) = 0;
  virtual const sstr getGameId() = 0;
  virtual const sstr getRoomId() = 0;

  const filepath getAtlas(const sstr& key);
  const filepath getFont(const sstr& key);
  const filepath getTile(const sstr& key);
  const filepath getImage(const sstr& key);

  const filepath getEffect(const sstr& key);
  const filepath getMusic(const sstr& key);

  const sstr getL10NStr(const sstr& key,
      const s::vector<sstr>& pms);
  const sstr getL10NStr(const sstr& key);

  c::Ref* getCst(const sstr& cst);
  int getBtnPadding();

  void toggleAudio(bool s);
  bool hasAudio();

  void setMusicVolume(float);
  void setVolume(float);

  float getMusicVolume();
  float getVolume();

  c::Dictionary* getLevel(const sstr& n);
  c::Dictionary* addLevel(const sstr& n);
  f::JsonObj* getLevelCfg(const sstr& n);

  const s_vec<filepath> getEffectFiles();
  const s_vec<filepath> getMusicFiles();

  void setSeedData(j::json&);
  j::json& getSeedData();

  NOCPYASS(XConfig)
  DECLCZ(XConfig)

  static void bind(not_null<XConfig*>);
  static XConfig* self();
};

//////////////////////////////////////////////////////////////////////////////
//
template<typename T> T* cstVal(const sstr& key) {
  auto r= fusii::XConfig::self()->getCst(key);
  if (NNP(r)) {
    return static_cast<T*>( r);
  } else {
    return nullptr;
  }
}

NS_END(fusii)
#endif

