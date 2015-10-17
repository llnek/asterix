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
NS_ALIAS(f, fusii)
NS_ALIAS(a, Ash)
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Motions : public f::BaseSystem {
protected:

  void OnSocket(a::Node*, float);
  void OnNet(a::Node*, evt);
  void OnSess(a::Node*, evt);
  void OnGUI(a::Node* , evt);
  void OnceOnly();

  virtual bool OnUpdate(float);
  Motions() = delete;
  NO__CPYASS(Motions)

public:

  Motions(not_null<a::Engine*>, not_null<c::Dictionary*>);
  virtual ~Motions();

  virtual void RemoveFromEngine(not_null<a::Engine*>);
  virtual void AddToEngine(not_null<a::Engine*> );
  //Priority: xcfg.ftypes.Motion

};


NS_END(tttoe)
#endif
