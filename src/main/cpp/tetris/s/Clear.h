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

#if !defined(__CLEAR_H__)
#define __CLEAR_H__

#include "core/XSystem.h"
#include "EFactory.h"
#include "n/GNodes.h"
NS_BEGIN(tetris)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Clear : public f::XSystem<EFactory> {

  virtual const a::SystemType typeId() { return "s/Clear"; }

  Clear(not_null<EFactory*>, not_null<c::Dictionary*>);

  virtual void addToEngine(not_null<a::Engine*>);
  virtual bool update(float);
  virtual int priority() { return a::AI + 50; }

  void clearFilled(a::Node*);
  void clearOneRow(a::Node*, int);
  void resetOneRow(a::Node*, int);
  void shiftDownLines(a::Node*);
  int findFirstDirty(a::Node*);
  int findFirstEmpty(a::Node*, int);
  int findLastEmpty(a::Node*, int);
  bool isEmptyRow(a::Node*, int);
  void copyLine(a::Node*, int, int);

  virtual ~Clear() {}
  NODFT(Clear)
  NOCPYASS(Clear)

  a::NodeList *arena;
};


NS_END(tetris)
#endif


