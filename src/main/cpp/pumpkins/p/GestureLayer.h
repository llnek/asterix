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
//////////////////////////////////////////////////////////////////////////////

#include "core/XConfig.h"
#include "core/COMP.h"
#include "core/CCSX.h"

#define TILE_SIZE 90

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(pumpkins)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL GestureLayer : public f::XLayer {

  __decl_mv(EGestureType, _gestureType, E_GESTURE_NONE)
  // target to pass the gesture event to
  __decl_ptr(c::Node, _target)
  // function to call when gesture event occurs
  __decl_md(SEL_CallFuncO, _handler)
  __decl_bf(_touchActive)
  __decl_md(CCT_PT, _touchStart)
  __decl_md(CCT_PT, _touchEnd)

  bool inix(c::Node*, SEL_CallFuncO);
  void handleTouch();
  GestureLayer() {}

  virtual bool onTouchStart(c::Touch*);
  virtual void onTouchMotion(c::Touch*);
  virtual void onTouchEnd(c::Touch*);

  virtual bool onMouseStart(const CCT_PT&);
  virtual void onMouseMotion(const CCT_PT&);
  virtual void onMouseClick(const CCT_PT&);

public:

  static owner<GestureLayer*> create(c::Node*, SEL_CallFuncO );

  __decl_getr(EGestureType, _gestureType, GestureType)
  __decl_getr(CCT_PT,_touchStart,TapStart)
  __decl_getr(CCT_PT,_touchEnd,TapEnd)

};


NS_END



