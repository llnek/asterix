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
//

#include "2d/CCScene.h"
#include "XLayer.h"
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL XScene : public XNode, public c::Scene {

  XLayer* addLayer(not_null<XLayer*>, int zx = 0);
  XLayer* getLayer(int tag);

  virtual void decorate() = 0;
  virtual bool init();
  virtual void postReify() {}

  virtual ~XScene();
  XScene();
  NOCPYASS(XScene)
};


//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL SceneWithOne : public XScene {
protected:

  DECL_PTR(SingleLayer, layer)

public:

  virtual ~SceneWithOne() {}
  SceneWithOne();
  NOCPYASS(SceneWithOne)

};



NS_END



