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

#if !defined(__SPLASH_H__)
#define __SPLASH_H__

#include "2d/XScene.h"
NS_BEGIN(fusilli)


//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL SplashLayer : public XLayer {
public:
  virtual SplashLayer();
  virtual Layer* Realize() override;
  CREATE_FUNC(SplashLayer)

private:
  DISALLOW_COPYASSIGN(SplashLayer)
  SplashLayer();
}

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Splash : public XScene {
public:
  virtual XScene* Realize() override;
  virtual ~Splash();
  Splash();
  CREATE_FUNC(Splash)
private:
  DISALLOW_COPYASSIGN(Splash)
}




NS_END(fusilli)
#endif

