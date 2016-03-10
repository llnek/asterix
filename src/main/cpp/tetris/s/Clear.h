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

#include "ecs/System.h"
#include "GEngine.h"

NS_BEGIN(tetris)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Clear : public ecs::System {

  void clearFilled();
  void clearOneRow( int);
  void resetOneRow( int);
  void shiftDownLines();
  int findFirstDirty();
  int findFirstEmpty( int);
  int findLastEmpty( int);
  bool isEmptyRow( int);
  void copyLine( int, int);
  void process(float);

public:

  virtual int priority() { return ecs::AI + 50; }

  MDECL_SYS_TPID("s/Clear")
  MDECL_SYS_PREAMBLE()
  MDECL_SYS_UPDATE()

  Clear(ecs::Engine *e)
  : System(e)
  {}

  DECL_PTR(ecs::Entity, arena)

};


NS_END


