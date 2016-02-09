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

#include "base/CCEventKeyboard.h"
#include "core/ComObj.h"
#include "ash/Engine.h"
#include "XLayer.h"
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL GameLayer : public XLayer {
protected:

  virtual bool onTouchStart(ComObj*, const s_vec<c::Touch*>& );
  virtual bool onTouchStart(ComObj*, c::Touch*);

  virtual void onTouchMotion(ComObj*, const s_vec<c::Touch*>& );
  virtual void onTouchMotion(ComObj*, c::Touch*);

  virtual void onTouchEnd(ComObj*, const s_vec<c::Touch*>& );
  virtual void onTouchEnd(ComObj*, c::Touch*);


  virtual void onMouseMotion(ComObj*, const c::Vec2&);
  virtual void onMouseClick(ComObj*, const c::Vec2&);

  virtual void onTouchesBegan(const s_vec<c::Touch*>&, c::Event*);
  virtual void onTouchesMoved(const s_vec<c::Touch*>&, c::Event*);
  virtual void onTouchesEnded(const s_vec<c::Touch*>&, c::Event*);

  virtual void onKeyReleased(KEYCODE k, c::Event*);
  virtual void onKeyPressed(KEYCODE k, c::Event*);

  virtual void update(float);
  virtual void surcease();

  void disableListeners();
  void enableListeners();

  virtual void postDeco();
  virtual void preDeco();

  c::Touch::DispatchMode tMode= c::Touch::DispatchMode::ALL_AT_ONCE;
  //c::Touch::DispatchMode tmode= c::Touch::DispatchMode::ONE_BY_ONE;
  DECL_PTR(c::EventListenerMouse ,_mouseListener)
  DECL_PTR(a::Engine ,engine)
  DECL_BF(_mouseEnabled)
  s_vec<ComObj*> motionees;
  s_arr<bool, 256> keys;

  virtual void onMouseScroll(c::Event*);
  virtual void onMouseDown(c::Event*);
  virtual void onMouseUp(c::Event*);
  virtual void onMouseMove(c::Event*);

public:

  bool isMouseEnabled() { return _mouseEnabled; }
  void setMouseEnabled(bool);
  bool keyPoll(KEYCODE);

  virtual ~GameLayer();
  GameLayer();
  NOCPYASS(GameLayer)
};




NS_END


