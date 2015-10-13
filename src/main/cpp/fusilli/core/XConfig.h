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

#include "platform/CCCommon.h"
#include "platform/CCGLView.h"
#include "Primitives.h"
#include <map>


#define XCFG() fusii::XConfig::Self()
NS_ALIAS(c, cocos2d)
NS_ALIAS(s, std)
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////
//
class CC_DLL XConfig {
protected:

  void AddAtlas(const stdstr& key, c::Ref*);
  void AddFont(const stdstr& key, c::Ref*);
  void AddTile(const stdstr& key, c::Ref*);
  void AddImage(const stdstr& key, c::Ref*);
  void AddEffect(const stdstr& key, c::Ref*);
  void AddMusic(const stdstr& key, c::Ref*);
  void AddCst(const stdstr& key, c::Ref*);

  c::Dictionary* dict;
  float lastMusicVol;
  float lastSfxVol;
  bool audioState;

  XConfig();

private:

  void AddXXX(const stdstr& frag, const stdstr& key, c::Ref*);
  c::Dictionary* GetFragment(const stdstr&);
  void SetCsts();
  NO__CPYASS(XConfig)

public:

  static void Bind(not_null<XConfig*>);
  static XConfig* Self();

  virtual void HandleResolution(const c::Size& ) {}
  virtual float GetScale() { return 1.0f; }
  virtual void RunOnce() {}

  virtual ResolutionPolicy GetPolicy() = 0;
  virtual const stdstr GetAppKey() = 0;
  virtual const stdstr AppId() = 0;
  virtual const stdstr GetColor() = 0;
  virtual const c::Size GetGameSize() = 0;

  virtual owner<c::Scene*> StartWith() = 0;
  virtual const stdstr GetWSUrl() = 0;

  virtual void SetGameId(const stdstr& ) = 0;
  virtual void SetRoomId(const stdstr& ) = 0;
  virtual const stdstr GetGameId() = 0;
  virtual const stdstr GetRoomId() = 0;

  const filepath GetAtlas(const stdstr& key);
  const filepath GetFont(const stdstr& key);
  const filepath GetTile(const stdstr& key);
  const filepath GetImage(const stdstr& key);

  const filepath GetEffect(const stdstr& key);
  const filepath GetMusic(const stdstr& key);

  c::Ref* GetCst(const stdstr& cst);
  int GetBtnPadding();

  void ToggleAudio(bool s);
  bool HasAudio();

  float SetMusicVolume(float);
  float SetVolume(float);

  float GetMusicVolume();
  float GetVolume();

  c::Dictionary* GetLevelCfg(const stdstr& n);
  c::Dictionary* GetLevel(const stdstr& n);

  c::Dictionary* AddLevel(const stdstr& n);

  const s::vector<filepath> GetEffectFiles();
  const s::vector<filepath> GetMusicFiles();


  virtual ~XConfig();

};

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
T* CstVal(const stdstr& key) {
  auto r= fusii::XConfig::Self()->GetCst(key);
  if (NNP(r)) {
    return static_cast<T*>( r);
  } else {
    return nullptr;
  }
}


NS_END(fusii)
#endif

