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

#include "platform/CCCommon.h"
#include "support/XPool.h"
#include <map>
NS_USING(std)
NS_BEGIN(fusilli)


//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
T* CstVal(const string& key) {
  auto r= XConfig::GetInstance()->GetCst(key);
  if (NNP(r)) {
    return static_cast<T*>(r);
  } else {
    return nullptr;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL XConfig {
protected:

  static const string ATLASES= "atlases";
  static const string LEVELS= "levels";
  static const string FONTS= "fonts";
  static const string TILES= "tiles";
  static const string IMAGES= "images";
  static const string SOUNDS= "sounds";
  static const string CSTS= "csts";
  static const string CFG= "cfg";

  Dictionary* GetFragment(const string& key);
  XPool* AddPool(const string& p);

  Dictionary* dict;
  XConfig();

private:

  DISALLOW_COPYASSIGN(XConfig)
  map<string,XPool*> pools;

public:

  static void SetInstance(XConfig*);
  static XConfig* GetInstance();

  virtual void HandleResolution(const Size& rs);
  virtual float GetScale();
  virtual void RunOnce();

  virtual const string GetAppKey() = 0;
  virtual const string AppId() = 0;
  virtual const string GetColor() = 0;
  virtual ResolutionPolicy GetPolicy() = 0;
  virtual const Size GetGameSize() = 0;

  virtual Scene* StartWith() = 0;

  const string GetAtlas(const string& key);
  const string GetFont(const string& key);
  const string GetTile(const string& key);
  const string GetImage(const string& key);
  const string GetSound(const string& key);


  Ref* GetCst(const string& cst);
  XPool* GetPool(const string& n);

  Dictionary* GetLevelCfg(const string& n);
  Dictionary* GetLevel(const string& n);

  virtual ~XConfig();

};








NS_END(fusilli)
#endif

