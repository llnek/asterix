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

  void addXXX(const sstr &frag, const sstr &key, c::Ref*);
  c::Dictionary* getFragment(const sstr&);

  void addAtlas(const sstr& , c::Ref*);
  void addFont(const sstr& , c::Ref*);
  void addTile(const sstr& , c::Ref*);
  void addImage(const sstr& , c::Ref*);
  void addEffect(const sstr& , c::Ref*);
  void addMusic(const sstr& , c::Ref*);
  void addCst(const sstr& , c::Ref*);

  c::Dictionary *frags=nullptr;
  c::Dictionary *l10n=nullptr;

  float lastMusicVol=0.5f;
  float lastSfxVol=0.5f;
  float scale=1.0f;
  bool audioState=true;

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
  virtual float getScale() { return scale; }
  virtual void runOnce() {}

  virtual ResolutionPolicy policy() = 0;
  virtual const sstr appKey() = 0;
  virtual const sstr appId() = 0;
  virtual const sstr color() = 0;
  virtual const c::Size gameSize() = 0;

  virtual c::Scene* prelude() = 0;
  virtual const sstr getWSUrl() = 0;

  virtual void setGameId(const sstr& ) = 0;
  virtual void setRoomId(const sstr& ) = 0;
  virtual const sstr getGameId() = 0;
  virtual const sstr getRoomId() = 0;

  const filepath getAtlas(const sstr& );
  const filepath getFont(const sstr& );
  const filepath getTile(const sstr& );
  const filepath getImage(const sstr& );

  const filepath getEffect(const sstr& );
  const filepath getMusic(const sstr& );

  const sstr getL10NStr(const sstr& , const s_vec<sstr>& );
  const sstr getL10NStr(const sstr& );

  c::Ref* getCst(const sstr& );
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

  NOCPYASS(XConfig)

  static XConfig* self();
  static void bind(not_null<XConfig*>);
};

//////////////////////////////////////////////////////////////////////////////
//
template<typename T> T* cstVal(const sstr &key) {
  auto r= fusii::XConfig::self()->getCst(key);
  if (NNP(r)) {
    return static_cast<T*>( r);
  } else {
    return nullptr;
  }
}

NS_END(fusii)
#endif

