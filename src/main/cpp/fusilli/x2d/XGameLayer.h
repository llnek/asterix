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
#include "core/Primitives.h"
#include "ash/System.h"
#include "ash/Engine.h"
#include "XHUDLayer.h"

#include <array>

NS_ALIAS(c, cocos2d)
NS_ALIAS(s, std)
NS_ALIAS(a, ash)
NS_BEGIN(fusii)

class ComObj;
//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL XGameLayer : public XLayer {
protected:

  s::array<bool, 256> keyboard;
  a::Engine* engine;

  c::EventListener* mouseListener;
  c::EventListener* keysListener;
  c::EventListener* touchListener;

  virtual void DisableEventHandlers();
  virtual void EnableEventHandlers();

  virtual void InitMouse();
  virtual void InitKeys();
  virtual void InitTouch();

  virtual void OnGameOver() = 0;

  NO__CPYASS(XGameLayer)
  XGameLayer();

public:

  virtual void SendMsg(const stdstr& topic, void* msg) = 0;

  virtual const Box4 GetEnclosureBox();
  virtual int GetIID() { return 2; }

  virtual void update(float);

  virtual ~XGameLayer();

  bool KeyPoll(c::EventKeyboard::KeyCode key);

  XLayer* GetBackgd(int tag= 1);
  XHUDLayer* GetHUD(int tag=3);

  void InizGame();
  void FinzGame();

  void OnKeyReleased(c::EventKeyboard::KeyCode, c::Event* );
  void OnKeyPressed(c::EventKeyboard::KeyCode, c::Event* );

  void OnMouseMove(c::Event*);
  void OnMouseDown(c::Event*);
  void OnMouseUp(c::Event*);
  void OnMouseScroll(c::Event*);


};





NS_END(fusii)
#endif

