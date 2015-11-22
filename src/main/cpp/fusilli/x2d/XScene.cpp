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

#include "core/XConfig.h"
#include "core/CCSX.h"
#include "XScene.h"
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(fusii)

BEGIN_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL SimLayer : public XLayer {
public:
  virtual XLayer* realize();
  NO__CPYASS(SimLayer)
  IMPL_CTOR(SimLayer)
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL SimScene : public XScene {
public:

  SimScene* setDecoUI(s::function<void (XLayer*)> d) {
    deco=d;
    return this;
  }

  s::function<void (XLayer*)> deco;
  NO__CPYASS(SimScene)

  void decoUI(XLayer* layer) {
    deco(layer);
  }

  virtual XScene* realize() {
    auto y = reifyRefType<SimLayer>();
    addLayer(y);
    y->realize();
    return this;
  }

  IMPL_CTOR(SimScene)
};

//////////////////////////////////////////////////////////////////////////////
//
XLayer* SimLayer::realize() {
  CC_PCAST(SimScene*)->decoUI(this);
  return this;
}

END_NS_UNAMED()

//////////////////////////////////////////////////////////////////////////////
//
XScene* XSceneFactory::reifySimple(s::function<void (XLayer*)> d) {
  return reifyRefType<SimScene>()->setDecoUI(d)->realize();
}

//////////////////////////////////////////////////////////////////////////////
//
XLayer* XScene::addLayer(not_null<XLayer*> y, int z) {
 this->addChild(y, z, y->getIID());
 return y;
}

//////////////////////////////////////////////////////////////////////////////
//
XLayer* XScene::getLayer(int tag) {
  return SCAST(XLayer*, getChildByTag(tag));
}

NS_END(fusii)


