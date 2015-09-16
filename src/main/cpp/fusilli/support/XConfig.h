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
#include "support/XPool.h"
#include "Primitives.h"
#include <map>
NS_ALIAS(cc, cocos2d)
NS_ALIAS(s, std)
NS_BEGIN(fusilli)


//////////////////////////////////////////////////////////////////////////
//
class CC_DLL XConfig {
protected:

  static const s::string ATLASES= "atlases";
  static const s::string LEVELS= "levels";
  static const s::string FONTS= "fonts";
  static const s::string TILES= "tiles";
  static const s::string IMAGES= "images";
  static const s::string SOUNDS= "sounds";
  static const s::string CSTS= "csts";
  static const s::string CFG= "cfg";

  cc::Dictionary* GetFragment(const s::string& key);
  XPool* AddPool(const s::string& p);

  cc::Dictionary* dict;
  XConfig();

private:

  s::map<s::string, XPool*> pools;
  DISALLOW_COPYASSIGN(XConfig)

public:

  static void SetInstance(XConfig*);
  static XConfig* GetInstance();

  virtual void HandleResolution(const cc::Size& );
  virtual float GetScale();
  virtual void RunOnce();

  virtual ResolutionPolicy GetPolicy() = 0;
  virtual const s::string GetAppKey() = 0;
  virtual const s::string AppId() = 0;
  virtual const s::string GetColor() = 0;
  virtual const cc::Size GetGameSize() = 0;

  virtual const s::string GetWSUrl() = 0;
  virtual cc::Scene* StartWith() = 0;

  virtual void SetGameId(const s::string& ) = 0;
  virtual void SetRoomId(const s::string& ) = 0;

  virtual const s::string GetGameId() = 0;
  virtual const s::string GetRoomId() = 0;

  const s::string GetAtlas(const s::string& key);
  const s::string GetFont(const s::string& key);
  const s::string GetTile(const s::string& key);
  const s::string GetImage(const s::string& key);
  const s::string GetSound(const s::string& key);


  cc::Ref* GetCst(const s::string& cst);
  XPool* GetPool(const s::string& n);

  void ToggleAudio(bool s);
  bool HasAudio();

  cc::Dictionary* GetLevelCfg(const s::string& n);
  cc::Dictionary* GetLevel(const s::string& n);

  cc::Dictionary* AddLevel(const s::string& n);

  virtual ~XConfig();

};

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
T* CstVal(const s::string& key) {
  auto r= XConfig::GetInstance()->GetCst(key);
  if (NNP(r)) {
    return static_cast<T*>(r);
  } else {
    return nullptr;
  }
}







NS_END(fusilli)
#endif

