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
#include "core/XSystem.h"
#include "EFactory.h"
#include "n/CObjs.h"

NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Logic : public f::XSystem<EFactory> {
protected:

  void onEnqueue(a::Node*, int pnum, int cell, Grid*);
  void enqueue(a::Node*, int pos, int value, Grid*);
  void doIt(a::Node*, float);

public:

  virtual const a::SystemType typeId() { return "n/Logic"; }

  Logic(not_null<EFactory*>, not_null<c::Dictionary*>);

  virtual void addToEngine(not_null<a::Engine*>);

  virtual int priority() { return a::Logic; }

  virtual bool update(float);

  virtual ~Logic() {}

  NOCPYASS(Logic)
  NODFT(Logic)

  c::DelayTime *botTimer=nullptr;
  a::NodeList *board=nullptr;
};


NS_END(tttoe)
#endif


