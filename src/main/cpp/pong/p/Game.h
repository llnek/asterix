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

#include "x2d/GameScene.h"
#include "core/JSON.h"
#include "core/Odin.h"

NS_ALIAS(ws, fusii::odin)
NS_BEGIN(pong)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GCXX : public f::GCX {
  GCXX(f::GMode m, ws::OdinIO *io, j::json pms) : GCX(m,io) {
    _data=pms;
  }
  __decl_md(j::json, _data)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Game : public f::GameScene {

  virtual void sendMsgEx(const MsgTopic&, void*);

  virtual f::GameLayer* getGLayer() {
     return (f::GameLayer*) getLayer(2);
  }

  STATIC_REIFY_SCENE_CTX(Game)
  __decl_deco_ui()

};


NS_END


