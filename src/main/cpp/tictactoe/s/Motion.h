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

#if !defined(__MOTION_H__)
#define __MOTION_H__

#include "core/BaseSystem.h"
#include "EFactory.h"
#include "n/cobjs.h"
#include <queue>

NS_ALIAS(ws, fusii::odin)
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Motions : public f::BaseSystem<EFactory> {
protected:

  void onGUIXXX(a::Node*, const c::Vec2& );
  void onGUI(a::Node*, c::EventMouse* );
  void onGUI(a::Node*, c::EventTouch* );

  void onSocket(a::Node*, ws::OdinEvent*);
  void onSess(a::Node*, ws::OdinEvent*);
  void onNet(a::Node*, ws::OdinEvent*);

  void onceOnly();
  void flushQ();

  virtual bool onUpdate(float);

  NO__CPYASS(Motions)
  Motions() = delete;

  s::queue<c::Event*> evQ;
  bool inited;

public:

  virtual const a::SystemType typeId() { return "n/Motions"; }

  Motions(not_null<EFactory*>, not_null<c::Dictionary*>);
  virtual ~Motions();

  virtual void addToEngine(not_null<a::Engine*> );
  virtual int priority() { return a::Motion; }

  a::NodeList* netplay;
  a::NodeList* gui;

};


NS_END(tttoe)
#endif
