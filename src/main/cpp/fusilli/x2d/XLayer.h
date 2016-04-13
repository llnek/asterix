// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Kenneth Leung. All rights reserved.

#pragma once
//////////////////////////////////////////////////////////////////////////////

#include "2d/CCLayer.h"
#include "XNode.h"
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////////
//
class XScene;
class CC_DLL XLayer : public XNode, public c::Layer {
protected:

  //c::Touch::DispatchMode _tMode= c::Touch::DispatchMode::ALL_AT_ONCE;
  c::Touch::DispatchMode _tMode= c::Touch::DispatchMode::ONE_BY_ONE;
  __decl_ptr(c::EventListenerMouse ,_mouseListener)
  __decl_mv(int, _mouseBtn, MOUSE_BUTTON_LEFT)
  __decl_bf(_mouseEnabled)
  __decl_bf(_mouseTarget)
  __decl_bf(_mouseDown)

  __decl_arr(bool, 256, _keys)
  __decl_nocpyass(XLayer)

  virtual void onTouchesBegan(const VecTouches&, c::Event*);
  virtual void onTouchesMoved(const VecTouches&, c::Event*);
  virtual void onTouchesEnded(const VecTouches&, c::Event*);

  virtual void onKeyReleased(KEYCODE k, c::Event*);
  virtual void onKeyPressed(KEYCODE k, c::Event*);

  virtual void onMouseScroll(c::Event*);
  virtual void onMouseDown(c::Event*);
  virtual void onMouseUp(c::Event*);
  virtual void onMouseMove(c::Event*);

  virtual bool onTouchStart(const VecTouches&);
  virtual void onTouchMotion(const VecTouches&);
  virtual void onTouchEnd(const VecTouches&);

  virtual bool onTouchStart(c::Touch*);
  virtual void onTouchMotion(c::Touch*);
  virtual void onTouchEnd(c::Touch*);

  virtual void onMouseMotion(const CCT_PT&);
  virtual bool onMouseStart(const CCT_PT&);
  virtual void onMouseClick(const CCT_PT&);

  void disableListeners();
  void enableListeners();

  virtual void decoPost() {}
  virtual void decoPre() {}

public:

  bool isMouseEnabled() { return _mouseEnabled; }
  void setMouseEnabled(bool);
  bool keyPoll(KEYCODE);

  virtual bool initEx(XScene*, int zx = 0);
  virtual void decoUI() =0;
  virtual void onInited() {}

  XScene* getSceneX();

  // tag value
  virtual int getIID() { return 1; }
  virtual ~XLayer() {}
  XLayer();

};




NS_END



