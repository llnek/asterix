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
NS_BEGIN(terra)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Aliens : public a::System {

  Enemy* getB(const EnemyType&);
  void addEnemyToGame( int );
  void addEnemy( j::json& );
  void dropBombs(Enemy*);
  void doIt(float);

public:

  MDECL_SYS_PRIORITY( a::Motion)
  MDECL_SYS_TPID( "n/Aliens")
  MDECL_SYS_PREAMBLE()
  MDECL_SYS_UPDATE()

  Aliens(a::Engine *e)
  : System(e)
  {}

  DECL_PTR(a::NodeList, arena)
  DECL_PTR(a::NodeList, ship)
};


NS_END(terra)


