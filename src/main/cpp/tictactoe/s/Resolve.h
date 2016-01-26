// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Ken Leung. All rights reserved.
#pragma once
//////////////////////////////////////////////////////////////////////////////

#include "ash/System.h"
#include "GEngine.h"
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Resolve : public  a::System {

  bool checkWin(Player*, Grid*, ArrDim& );
  void showWinningIcons(const ArrDim& );
  void doWin(Player*, const ArrDim& );
  void sync();
  void doIt( float);
  void doForfeit();
  void doDraw();
  bool checkDraw( Grid*);
  void doDone( Player*);

public:

  MDECL_SYS_PRIORITY(a::Resolve)
  MDECL_SYS_TPID( "n/Resolve")
  MDECL_SYS_PREAMBLE()
  MDECL_SYS_UPDATE()

  DECL_PTR(a::NodeList, board)
  DECL_PTR(a::NodeList, arena)

  Resolve(a::Engine *e)
  : System(e)
  {}

};



NS_END(tttoe)


