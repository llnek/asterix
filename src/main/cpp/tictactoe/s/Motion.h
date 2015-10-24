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

NS_ALIAS(ws, fusii::wsock)
NS_ALIAS(f, fusii)
NS_ALIAS(a, ash)
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Motions : public f::BaseSystem<EFactory> {
protected:

  void OnSess(a::Node*, const ws::Event& );
  void OnGUI(a::Node* , const c::Vec2& );
  void OnNet(a::Node*, const ws::Event& );
  void OnSocket(a::Node*, const ws::Event&);
  void OnceOnly();
  void FlushQ();

  virtual bool OnUpdate(float);

  NO__CPYASS(Motions)
  Motions() = delete;

  s::queue<EventXXX> evQ;
  bool inited;

public:

  Motions(not_null<EFactory*>, not_null<c::Dictionary*>);
  virtual ~Motions();

  virtual void AddToEngine(not_null<a::Engine*> );
  virtual int Priority() { return a::Motion; }

  virtual const a::SystemType TypeId() {
    return "n/Motions";
  }

  a::NodeList* netplay;
  a::NodeList* gui;

};


NS_END(tttoe)
#endif
