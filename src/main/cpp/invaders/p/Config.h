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

#include "x2d/XScene.h"
#include "core/XConfig.h"
NS_BEGIN(invaders)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Config : public f::XConfig {
protected:

  Config();

private:

  void initLevels();
  void initAssets();
  void initCsts();
  void initPools();

  float scale;

public:

  virtual void handleResolution(const c::Size& );
  virtual const c::Size gameSize();

  virtual ResolutionPolicy policy();
  virtual const sstr appKey();
  virtual const sstr appId();
  virtual const sstr color();

  virtual f::XScene* startWith();
  virtual const sstr getWSUrl();

  virtual void setGameId(const sstr& s) {}
  virtual void setRoomId(const sstr& s) {}

  virtual const sstr getGameId();
  virtual const sstr getRoomId();

  virtual float getScale()  { return scale; }
  virtual void runOnce() ;

  virtual ~Config() {}
  NOCPYASS(Config)

  static owner<f::XConfig*> reify();
};


NS_END(invaders)
#endif


