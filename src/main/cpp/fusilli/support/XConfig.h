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

#include "core/fusilli.h"
#include "cocos2d.h"
NS_USING(cocos2d)
NS_USING(std)
NS_BEGIN(fusilli)


//////////////////////////////////////////////////////////////////////////////
//
class Ref;

class CC_DLL XConfig {

public:

  static void SetInstance(XConfig*);
  static XConfig* GetInstance();

  virtual const string GetAppKey() = 0;
  virtual const string AppId() = 0;
  virtual const string GetColor() = 0;
  virtual ResolutionPolicy GetPolicy() = 0;
  virtual const Size GetGameSize() = 0;


  virtual const string GetAtlas(const string& key);
  virtual const string GetFont(const string& key);
  virtual const string GetTile(const string& key);
  virtual const string GetImage(const string& key);
  virtual const string GetSound(const string& key);

  virtual void HandleResolution(const Size& rs);
  virtual Ref* GetCst(const string& cst);

  virtual Dictionary* GetLevelCfg(const string& n);
  virtual Dictionary* GetLevel(const string& n);

  virtual float GetScale();
  virtual void RunOnce();

  virtual ~XConfig();

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

  Dictionary* dict;
  XConfig();

private:

  DISALLOW_COPYASSIGN(XConfig)

};








NS_END(fusilli)
#endif

