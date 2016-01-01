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

#if !defined(__STAGE_H__)
#define __STAGE_H__

#include "ash/System.h"
#include "n/GNodes.h"

NS_BEGIN(pong)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Stage : public a::System {

  virtual const a::SystemType typeId() { return "s/Stage"; }
  virtual int priority() { return a::PreUpdate; }

  virtual void addToEngine(not_null<a::Engine*>);
  virtual bool update(float dt);

  virtual ~Stage() {}
  Stage() {}
  NOCPYASS(Stage)


  DECL_PTR(a::NodeList, paddleNode)
  DECL_PTR(a::NodeList, arenaNode)
  DECL_PTR(a::NodeList, ballNode)
  DECL_PTR(a::NodeList, fauxNode)
  DECL_BF(inited)

protected:

  void initPaddles(a::NodeList*,Slots*);
  const c::Size initPaddleSize();
  const c::Size initBallSize();
  void onceOnly();
};


NS_END(pong)
#endif


