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
#include "dbox/json11.hpp"
#include "Primitives.h"
NS_ALIAS(j, json11)
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////
//
class CC_DLL XConfig {
protected:

  void addXXX(const stdstr& frag, const stdstr& key, c::Ref*);
  c::Dictionary* getFragment(const stdstr&);

  void addAtlas(const stdstr& key, c::Ref*);
  void addFont(const stdstr& key, c::Ref*);
  void addTile(const stdstr& key, c::Ref*);
  void addImage(const stdstr& key, c::Ref*);
  void addEffect(const stdstr& key, c::Ref*);
  void addMusic(const stdstr& key, c::Ref*);
  void addCst(const stdstr& key, c::Ref*);

  c::Dictionary* frags;
  c::Dictionary* l10n;
  j::Json seed;

  float lastMusicVol;
  float lastSfxVol;
  bool audioState;

  void loadL10NStrings();
  void setCsts();

  NO__CPYASS(XConfig)
  XConfig();

public:

  virtual void handleResolution(const c::Size& ) {}
  virtual float getScale() { return 1.0f; }
  virtual void runOnce() {}

  virtual ResolutionPolicy policy() = 0;
  virtual const stdstr appKey() = 0;
  virtual const stdstr appId() = 0;
  virtual const stdstr color() = 0;
  virtual const c::Size gameSize() = 0;

  virtual c::Scene* startWith() = 0;
  virtual const stdstr getWSUrl() = 0;

  virtual void setGameId(const stdstr& ) = 0;
  virtual void setRoomId(const stdstr& ) = 0;
  virtual const stdstr getGameId() = 0;
  virtual const stdstr getRoomId() = 0;

  const filepath getAtlas(const stdstr& key);
  const filepath getFont(const stdstr& key);
  const filepath getTile(const stdstr& key);
  const filepath getImage(const stdstr& key);

  const filepath getEffect(const stdstr& key);
  const filepath getMusic(const stdstr& key);

  const stdstr getL10NStr(const stdstr& key,
      const s::vector<stdstr>& pms);
  const stdstr getL10NStr(const stdstr& key);

  c::Ref* getCst(const stdstr& cst);
  int getBtnPadding();

  void toggleAudio(bool s);
  bool hasAudio();

  void setMusicVolume(float);
  void setVolume(float);

  float getMusicVolume();
  float getVolume();

  c::Dictionary* getLevelCfg(const stdstr& n);
  c::Dictionary* getLevel(const stdstr& n);
  c::Dictionary* addLevel(const stdstr& n);

  const s::vector<filepath> getEffectFiles();
  const s::vector<filepath> getMusicFiles();

  void setSeedData(j::Json&);
  j::Json& getSeedData();

  virtual ~XConfig();

  static void bind(not_null<XConfig*>);
  static XConfig* self();
};

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
T* CstVal(const stdstr& key) {
  auto r= fusii::XConfig::self()->getCst(key);
  if (NNP(r)) {
    return static_cast<T*>( r);
  } else {
    return nullptr;
  }
}





NS_END(fusii)
#endif

