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
  void addColor(const sstr& , c::Ref*);

  DECL_PTR(c::Dictionary ,frags)
  DECL_PTR(c::Dictionary ,l10n)

  DECL_TV(float, lastMusicVol,0.5f)
  DECL_TV(float, lastSfxVol, 0.5f)
  DECL_TV(float, scale, 1.0f)
  DECL_BT(audioState)

  void loadL10NStrings();
  void setCsts();

  static const sstr ATLASES;
  static const sstr LEVELS;
  static const sstr FONTS;
  static const sstr TILES;
  static const sstr IMAGES;
  static const sstr COLORS;
  static const sstr MUSIC;
  static const sstr CSTS;
  static const sstr EFX;
  static const sstr CFG;

public:

  virtual void handleResolution(const c::Size& ) {}
  virtual float getScale() { return scale; }
  virtual void runOnce() {}

  virtual ResolutionPolicy policy() = 0;
  virtual const c::Size gameSize() = 0;

  virtual const sstr getWSUrl() {
    return "";
  }

  virtual c::Scene* prelude() = 0;

  virtual bool isPortrait() { return true; }

  void setRoomId(const sstr &s) {
    room_id=s;
  }

  const sstr getGameId() { return game_id; }
  const sstr getRoomId() { return room_id; }

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

  DECL_TD(sstr, room_id)
  DECL_TD(sstr, app_id)
  DECL_TD(sstr, game_id)

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

