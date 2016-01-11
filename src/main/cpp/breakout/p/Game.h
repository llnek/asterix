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

#if !defined(__GAME_H__)
#define __GAME_H__

#include "x2d/GameScene.h"

NS_BEGIN(breakout)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Game : public f::GameScene {
  virtual void sendMsgEx(const MsgTopic&, void*);
  STATIC_REIFY_SCENE_CTX(Game)
  MDECL_DECORATE()
  MDECL_GLAYER(2)
  virtual const f::Box4 getEnclosureBox();

};


NS_END(breakout)
#endif


