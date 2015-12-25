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

#if !defined(__CONFIG_H__)
#define __CONFIG_H__

#include "core/XConfig.h"

NS_BEGIN(pong)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Config : public f::XConfig {
protected:

  void initAssets();
  void initCsts();
  void initLevels();

  Config() {}

public:

  virtual const sstr appKey() { return "fa0860f9-76dc-4135-8bc7-bd5af3147d55"; }

  virtual const sstr appId() { return "pong"; }

  virtual const sstr themeColor() { return "green"; }

  virtual ResolutionPolicy policy() { return ResolutionPolicy::FIXED_HEIGHT; }

  virtual void handleResolution(const c::Size&);

  virtual void runOnce();

  virtual const c::Size gameSize() { return c::Size(320,480); }

  virtual const sstr getWSUrl() { return ""; }
  virtual c::Scene* prelude();

  virtual void setGameId(const sstr& ) {}
  virtual void setRoomId(const sstr& ) {}
  virtual const sstr getGameId() {}
  virtual const sstr getRoomId() {}

  static owner<Config*> reify();

  virtual ~Config() {}
  NOCPYASS(Config);
};



NS_END(pong)
#endif


