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

#include "support/XConfig.h"

NS_ALIAS(cc, cocos2d)
NS_ALIAS(f,fusilli)
NS_BEGIN(invaders)


//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Config : public XConfig {
protected:

  void InitAssets();
  void InitCsts();
  Config();

  float scale;

private:

  DISALLOW_COPYASSIGN(Config)

public:

  virtual void HandleResolution(const cc::Size& rs);

  virtual ResolutionPolicy GetPolicy();
  virtual const s::string GetAppKey();
  virtual const s::string AppId();
  virtual const s::string GetColor();
  virtual const cc::Size GetGameSize();

  virtual const s::string GetWSUrl();
  virtual cc::Scene* StartWith();

  virtual const s::string GetGameId();
  virtual const s::string GetRoomId();

  virtual float GetScale() ;
  virtual void RunOnce() ;

  virtual ~Config();

  DEFCREATE_FUNC(Config)
};


NS_END(invaders)

