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

#if !defined(__XGAMELAYER_H__)
#define __XGAMELAYER_H__

#include "base/CCEventKeyboard.h"
#include "core/ComObj.h"
#include "ash/Engine.h"
#include "XLayer.h"
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL XGameLayer : public XLayer {
protected:

  s::array<bool, 256> keyboard;
  //c::Dictionary* options;
  //a::Engine* engine;

  c::EventListener* mouse;
  c::EventListener* keys;
  c::EventListener* touch;

  virtual void DisableEventHandlers();
  virtual void EnableEventHandlers();

  virtual void InitMouse();
  virtual void InitKeys();
  virtual void InitTouch();

  NO__CPYASS(XGameLayer)
  XGameLayer();

public:

  virtual void SendMsg(const stdstr& topic, void* msg) = 0;
  void SendMsg(const stdstr& topic) {
    SendMsg(topic, nullptr);
  }

  virtual const Box4 GetEnclosureBox();
  virtual int GetIID() { return 2; }

  //virtual void update(float);
  //virtual XLayer* Realize();
  virtual ~XGameLayer();

  void OnKeyReleased(c::EventKeyboard::KeyCode, c::Event* );
  void OnKeyPressed(c::EventKeyboard::KeyCode, c::Event* );

  void OnMouseMove(c::Event*);
  void OnMouseDown(c::Event*);
  void OnMouseUp(c::Event*);
  void OnMouseScroll(c::Event*);

  bool KeyPoll(c::EventKeyboard::KeyCode key);
};





NS_END(fusii)
#endif

