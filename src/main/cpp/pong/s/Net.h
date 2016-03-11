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
#include "core/Odin.h"
#include "GEngine.h"

NS_ALIAS(ws, fusii::odin)
NS_BEGIN(pong)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Net : public ecs::System {

  void syncOnePaddle(ecs::Node*, j::json);
  void syncPaddles(ws::OdinEvent*);
  void onceOnly();
  void onEvent(ws::OdinEvent*);
  void onnetw(ws::OdinEvent*);
  void onsess(ws::OdinEvent*);
  void syncScores(j::json);
  void process(ws::OdinEvent*);
  void reposPaddles();
  void reposNodes();

public:

  MDECL_SYS_PRIORITY( ecs::NetPlay)
  MDECL_SYS_TPID( "s/Net")
  MDECL_SYS_PREAMBLE()
  MDECL_SYS_UPDATE()

  Net(ecs::Engine *e)
  : System(e)
  {}

  DECL_PTR(ecs::Node, _arena)
  DECL_PTR(ecs::Node, _ball)
  s_vec<ecs::Node*> _paddles;

};


NS_END


