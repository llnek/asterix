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
#include "EFactory.h"
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Stage : public a::System {

  MDECL_SYS_PRIORITY(a::PreUpdate)
  MDECL_SYS_TPID( "n/Stage")
  MDECL_SYS_PREAMBLE()
  MDECL_SYS_UPDATE()

  DECL_PTR(a::NodeList, boardNode)
  DECL_PTR(a::NodeList, arenaNode)

  Stage(a::Engine *e)
  : System(e)
  {}

protected:

  void onSocket(ws::OdinEvent*);
  void onSess(ws::OdinEvent*);
  void onNet(ws::OdinEvent*);

  void initOnline();
  void showGrid();
  void onceOnly();
  void doIt();
};


NS_END(tttoe)
#endif


