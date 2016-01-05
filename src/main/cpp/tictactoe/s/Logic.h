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

#include "ash/System.h"
#include "EFactory.h"

NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Logic : public a::System {

  MDECL_SYS_PRIORITY( a::Logic)
  MDECL_SYS_TPID("n/Logic")
  MDECL_SYS_PREAMBLE()
  MDECL_SYS_UPDATE()

  DECL_PTR(c::DelayTime, botTimer)
  DECL_PTR(a::NodeList, boardNode)
  DECL_PTR(a::NodeList, arenaNode)

  Logic(a::Engine *e)
  : System(e)
  {}

private:
  void onEnqueue(int pnum, int cell, Grid*);
  void enqueue(int pos, int value, Grid*);
  void doIt( float);

};


NS_END(tttoe)
#endif


