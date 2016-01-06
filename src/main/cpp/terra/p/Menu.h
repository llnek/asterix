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

#if !defined(__MENU_H__)
#define __MENU_H__

#include "x2d/XScene.h"
NS_BEGIN(terra)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL MCX : public f::SCTX {
  DECL_TV(VOIDFN, back, nullptr)
  MCX(VOIDFN cb) { back=cb; }
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL MMenu : public f::XScene {

  STATIC_REIFY_SCENE_CTX(MMenu)
  MDECL_DECORATE()

};


NS_END(terra)
#endif


