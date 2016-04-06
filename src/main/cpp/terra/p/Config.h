// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Ken Leung. All rights reserved.

#pragma once

#include "core/XConfig.h"
NS_BEGIN(terra)

//////////////////////////////////////////////////////////////////////////
//
class CC_DLL Config : public f::XConfig {

  void initLevels();
  void initCsts();
  void initAssets();
  Config() {}

public:

  virtual const CCT_SZ gameSize() { return CCT_SZ(480,720); }

  virtual void handleResolution(const CCT_SZ& );

  virtual float scaleFloat(float);

  virtual ResolutionPolicy policy() {
    return ResolutionPolicy::FIXED_HEIGHT;
  }

  virtual c::Scene* prelude();
  virtual void runOnce();

  static owner<Config*> reify();
};



NS_END



