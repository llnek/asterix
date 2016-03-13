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
//

#include "2d/CCLayer.h"
#include "XNode.h"
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////////
//
class XScene;
class CC_DLL XLayer : public XNode, public c::Layer {
protected:

  //c::Touch::DispatchMode tMode= c::Touch::DispatchMode::ALL_AT_ONCE;
  c::Touch::DispatchMode _tMode= c::Touch::DispatchMode::ONE_BY_ONE;
  DECL_PTR(c::EventListenerMouse ,_mouseListener)
  DECL_TV(int, _mouseBtn, -911)
  DECL_BF(_mouseEnabled)
  DECL_BF(_mouseDown)
  s_arr<bool, 256> _keys;

  virtual void onTouchesBegan(const s_vec<c::Touch*>&, c::Event*);
  virtual void onTouchesMoved(const s_vec<c::Touch*>&, c::Event*);
  virtual void onTouchesEnded(const s_vec<c::Touch*>&, c::Event*);
  virtual void onKeyReleased(KEYCODE k, c::Event*);
  virtual void onKeyPressed(KEYCODE k, c::Event*);
  virtual void onMouseScroll(c::Event*);
  virtual void onMouseDown(c::Event*);
  virtual void onMouseUp(c::Event*);
  virtual void onMouseMove(c::Event*);

  virtual bool onTouchStart( const s_vec<c::Touch*>& );
  virtual bool onTouchStart( c::Touch*);
  virtual void onTouchMotion( const s_vec<c::Touch*>& );
  virtual void onTouchMotion( c::Touch*);
  virtual void onTouchEnd( const s_vec<c::Touch*>& );
  virtual void onTouchEnd( c::Touch*);
  virtual void onMouseMotion( const c::Vec2&);

  virtual void onMouseStart(const c::Vec2&);
  virtual void onMouseClick(const c::Vec2&);

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
  NOCPYASS(XLayer)
};




NS_END



