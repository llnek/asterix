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

#if !defined(__NETPLAY_H__)
#define __NETPLAY_H__

//#include "x2d/GameScene.h"
#include "x2d/XScene.h"
#include "core/Odin.h"

NS_ALIAS(ws, fusii::odin)
NS_BEGIN(tttoe)

typedef std::function<void (ws::OdinIO*, j::json)> NPCX_Yes;

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL NPCX : public f::SCTX {
  NPCX(NPCX_Yes yes, VOIDFN no) {
    this->yes=yes;
    this->no=no;
  }
  NPCX_Yes yes;
  VOIDFN no;
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL NetPlay : public f::XScene {

  STATIC_REIFY_SCENE_CTX(NetPlay)

  virtual void decorate();

  virtual ~NetPlay() {}
  NetPlay() {}

  NOCPYASS(NetPlay)
};


NS_END(tttoe)
#endif

