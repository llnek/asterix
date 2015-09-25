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

#if !defined(__XLIVES_H__)
#define __XLIVES_H__

#include "platform/CCCommon.h"
#include "XHUDLayer.h"
#include <vector>

NS_ALIAS(c, cocos2d)
NS_ALIAS(s, std)
NS_BEGIN(fusilli)


//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL XLives {
protected:

  s::vector<c::Node*> icons;
  int totalLives;
  int curLives;
  int dir;
  c::Size lifeSize;
  c::Vec2 topLeft;
  stdstr frameId;
  XHUDLayer* hud;

  XLives();

private:

  NO__CPYASS(XLives)

public:

  static owner<XLives*> Create(not_null<XHUDLayer*>,
      const stdstr& frame,
      float x, float y, int dir= 1);

  virtual ~XLives();

  int GetLives() { return curLives; }
  void Reduce(int c);
  bool IsDead();
  void Reset();
  void Resurrect();
  void DrawLives();
  void Create();

};





NS_END(fusilli)
#endif

