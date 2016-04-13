// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Kenneth Leung. All rights reserved.

#pragma once
//////////////////////////////////////////////////////////////////////////////

#include "2d/CCScene.h"
#include "XLayer.h"
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL XScene : public XNode, public c::Scene {

  __decl_nocpyass(XScene)
  __decl_bf(_usePhysics)

public:

  XLayer* addLayer(not_null<XLayer*>, int zx = 0);
  XLayer* getLayer(int tag);

  virtual void decoUI() = 0;
  virtual bool init();
  virtual void onInited() {}

  XScene(bool physics);
  XScene();
  virtual ~XScene();
};



NS_END



