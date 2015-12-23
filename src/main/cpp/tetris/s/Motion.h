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

#include "core/XSystem.h"
#include "EFactory.h"
#include "n/GNodes.h"
NS_BEGIN(tetris)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Motions : public f::XSystem<EFactory> {

  virtual const a::SystemType typeId() { return "s/Motions"; }

  Motions(not_null<EFactory*>, not_null<c::Dictionary*>);

  virtual void addToEngine(not_null<a::Engine*>);
  virtual bool update(float);

  void doit(a::Node *node, float);
  void onkey(a::Node *node, float);
  void sftRight(a::Node *node, float);
  void sftLeft(a::Node *node, float);
  void sftDown(a::Node *node, float);
  void rotRight(a::Node *node, float);
  void rotLeft(a::Node *node, float);
  void mkThrottler();
  void onceOnly();

  virtual int priority() { return a::Motion; }
  virtual ~Motions();
  NODFT(Motions)

  NOCPYASS(Motions)

  c::DelayTime *timer=nullptr;
  a::NodeList *arena=nullptr;
};



NS_END(tetris)
#endif

