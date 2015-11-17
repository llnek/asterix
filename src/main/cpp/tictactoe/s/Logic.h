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

#if !defined(__LOGIC_H__)
#define __LOGIC_H__

#include "2d/CCActionInterval.h"
#include "core/BaseSystem.h"
#include "EFactory.h"
#include "n/cobjs.h"

NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Logic : public f::BaseSystem<EFactory> {
protected:

  void OnEnqueue(a::Node*, int pnum, int cell, Grid*);
  void Enqueue(a::Node*, int pos, int value, Grid*);
  void DoIt(a::Node*, float);

  virtual bool OnUpdate(float);

  NO__CPYASS(Logic)
  Logic() = delete;

public:

  Logic(not_null<EFactory*>, not_null<c::Dictionary*>);

  virtual void AddToEngine(not_null<a::Engine*>);

  virtual int Priority() { return a::Logic; }

  virtual ~Logic();

  virtual const a::SystemType TypeId() {
    return "n/Logic";
  }

  c::DelayTime* botTimer;
  a::NodeList* arena;
};


NS_END(tttoe)
#endif


