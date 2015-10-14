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

#if !defined(__HUD_H__)
#define __HUD_H__

#include "platform/CCCommon.h"
#include "x2d/XHUDLayer.h"

NS_ALIAS(f, fusii)
NS_BEGIN(invaders)


//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL HUDLayer : public f::XHUDLayer {
protected:

  void ShowMenu(c::Ref*);
  void OnReplay(c::Ref*);

  void InitLabels();
  void InitIcons();

public:

  virtual int GetIID() { return 3; }
  virtual f::XLayer* Realize();

  NO__CPYASS(HUDLayer)
  IMPL_CTOR(HUDLayer)
};



NS_END(invaders)
#endif


