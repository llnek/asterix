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

#if !defined(__COLLIDE_H__)
#define __COLLIDE_H__

#include "ash/System.h"
#include "EFactory.h"

NS_BEGIN(pong)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Collide : public a::System {

  MDECL_SYS_PRIORITY(a::Collide)
  MDECL_SYS_TPID( "s/Collide")
  MDECL_SYS_PREAMBLE()
  MDECL_SYS_UPDATE()

  Collide(a::Engine *e)
  : System(e)
  {}

  DECL_PTR(a::NodeList, paddleNode)
  DECL_PTR(a::NodeList, fauxNode)
  DECL_PTR(a::NodeList, ballNode)
  DECL_PTR(a::NodeList, arenaNode)

private:

  void checkNodes(a::NodeList*);
  void check(a::Node*, Ball*);

};

NS_END(pong)
#endif


