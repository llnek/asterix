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
#include "x2d/XScene.h"
NS_BEGIN(invaders)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Config : public f::XConfig {
protected:

  void initLevels();
  void initAssets();
  void initCsts();
  void initPools();

public:

  virtual const c::Size gameSize() { return c::Size(320,480); }
  virtual void handleResolution(const c::Size& );

  virtual const sstr appKey() { return "d39bf198-518a-4de7-88a0-5e28c88571b0"; }
  virtual ResolutionPolicy policy() { return ResolutionPolicy::FIXED_HEIGHT; }
  virtual const sstr appId() { return "invaders"; }
  virtual const sstr themeColor() { return "red"; }

  virtual const sstr getWSUrl();
  virtual c::Cscene* prelude();

  virtual void setGameId(const sstr& s) {}
  virtual void setRoomId(const sstr& s) {}

  virtual const sstr getGameId();
  virtual const sstr getRoomId();

  virtual void runOnce() ;

  virtual ~Config() {}
  Config() {}
  NOCPYASS(Config)

  static owner<Config*> reify();
};


NS_END(invaders)
#endif


