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

#include "core/XConfig.h"
NS_ALIAS(c, cocos2d)
NS_ALIAS(f,fusilli)
NS_BEGIN(invaders)


//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Config : public f::XConfig {
private:

  NO__COPYASSIGN(Config)

  void InitAssets();
  void InitCsts();
  void InitPools();

  float scale;

public:

  virtual void HandleResolution(const c::Size& rs);
  Config();

  virtual ResolutionPolicy GetPolicy();
  virtual const stdstr GetAppKey();
  virtual const stdstr AppId();
  virtual const stdstr GetColor();
  virtual const c::Size GetGameSize();

  virtual const stdstr GetWSUrl();
  virtual owner<c::Scene*> StartWith();

  virtual void SetGameId(const stdstr& s) {}
  virtual void SetRoomId(const stdstr& s) {}

  virtual const stdstr GetGameId() { return ""; }
  virtual const stdstr GetRoomId() { return ""; }

  virtual float GetScale() ;
  virtual void RunOnce() ;

  virtual ~Config() {}

};


NS_END(invaders)

