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
#pragma once
//////////////////////////////////////////////////////////////////////////////

#include "core/XConfig.h"
NS_BEGIN(asteroids)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Config : public f::XConfig {

  void initLevels();
  void initCsts();
  void initAssets();

  Config() {}

public:

  virtual const c::Size gameSize() { return c::Size(480,320);  }

  virtual void handleResolution(const c::Size&);
  virtual void runOnce();

  virtual ResolutionPolicy policy() {
    return ResolutionPolicy::FIXED_HEIGHT;
  }

  virtual bool isPortrait() { return false; }
  virtual c::Scene* prelude();

  static owner<Config*> reify();

};


NS_END(asteroids)

