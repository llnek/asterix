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
NS_BEGIN(terra)

//////////////////////////////////////////////////////////////////////////
//
class CC_DLL Config : public f::XConfig {
protected:

  void initAssets();
  void initCsts();
  void initLevels();

  Config() {}

public:

  virtual ResolutionPolicy policy() {
    return ResolutionPolicy::FIXED_HEIGHT;
  }

  virtual c::Scene* startWith();

  virtual const sstr getWSUrl() {
    return "";
  }

  virtual void setGameId(const sstr& ) {
  }

  virtual void setRoomId(const sstr& ) {
  }

  virtual const sstr getGameId() {
    return "";
  }

  virtual const sstr getRoomId() {
    return "";
  }

  virtual void handleResolution(const c::Size& );

  virtual void runOnce();

  virtual ~Config() {}
  NOCPYASS(Config)

  virtual const sstr appKey() { return "4d6b93c4-05d7-42f1-95cc-98ce8adeac0a"; }

  virtual const sstr color() { return "yellow"; }

  virtual const sstr appId() { return "terra"; }

  virtual const c::Size gameSize() { return c::Size(320,480); }


  static owner<Config*> reify();
};


NS_END(terra)
#endif



