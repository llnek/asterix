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

#include "fusilli/support/XConfig.h"
USING_NS_CC;
NS_FI_BEGIN

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Config : public XConfig {
public:

  static XConfig* getInstance();

  virtual const string GetAppKey() override;

  virtual const string AppId() override;

  virtual const string GetColor() override;

  virtual ResolutionPolicy GetPolicy() override;

  virtual Ref* GetCst(const string& csts) override;

  virtual const string GetAtlas(const string& key) override;
  virtual const string GetFont(const string& key) override;
  virtual const string GetTile(const string& key) override;
  virtual const string GetImage(const string& key) override;
  virtual const string GetSound(const string& key) override;

  virtual void HandleResolution(const Size& rs) override;

  virtual Dictionary* GetLevelCfg(const string& n) override;
  virtual Dictionary* GetLevel(const string& n) override;

  virtual const Size GetGameSize() override;
  virtual float GetScale() override;

  virtual void RunOnce() override;

private:
  DISALLOW_COPYASSIGN(Config)
  virtual ~Config();
  Config();
  Dictionary* m_dict;
  float m_scale;
};








NS_FI_END

