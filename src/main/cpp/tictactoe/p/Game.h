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

#incldue "x2d/XGameLayer.h"
#include "x2d/MainGame.h"
NS_ALIAS(f, fusii)
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Game : public f::MainGame {
protected:

  virtual f::XGameLayer* GetGLayer();
  NO__CPYASS(Game)

public:

  virtual f::XScene* Realize();

  DECL_CTOR(Game)

  virtual bool IsRunning();
  virtual void Resume();
  virtual void Pause();
  virtual void Run();
};


NS_END(tttoe)
#endif


