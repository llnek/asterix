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

#include "../core/fusilli.h"
USING_NS_CC;
NS_FI_BEGIN


class CC_DLL Config : public XConfig {

  virtual const string GetAppKey() = 0;

  virtual const string AppId() = 0;

  virtual const string GetColor() = 0;

  virtual ResolutionPolicy GetPolicy() = 0;

  virtual Ref* GetCst(const string& csts) = 0;

  virtual const string GetAtlas(const string& key) = 0;
  virtual const string GetFont(const string& key) = 0;
  virtual const string GetTile(const string& key) = 0;
  virtual const string GetImage(const string& key) = 0;
  virtual const string GetSound(const string& key) = 0;

  virtual void HandleResolution(const Size& rs) = 0;

  virtual Dictionary* GetLevelCfg(const string& n) = 0;
  virtual Dictionary* GetLevel(const string& n) = 0;

  virtual const Size GetGameSize() = 0;
  virtual float GetScale() = 0;

  virtual void RunOnce() = 0;

};








NS_FI_END

