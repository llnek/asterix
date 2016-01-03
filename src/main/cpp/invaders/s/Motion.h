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

#include "ash/System.h"
#include "EFactory.h"
NS_BEGIN(invaders)


//////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Motions : public a::System {

  virtual void addToEngine(not_null<a::Engine*>);
  virtual bool update(float);

  MDECL_SYS_PRIORITY( a::Motion)
  MDECL_SYS_TPID( "n/Motions")

  void processAlienMotions(a::Node*,float dt);
  void controlCannon(a::Node*, float dt);
  void fireMissile(a::Node*, float dt);
  void scanInput(a::Node*, float dt);

  DECL_PTR(a::NodeList, cannons)
  DECL_PTR(a::NodeList, ships)
  DECL_PTR(a::NodeList, aliens)
  DECL_BF(right)
  DECL_BF(left)

  GEngine *engine;
};


NS_END(invaders)
#endif


