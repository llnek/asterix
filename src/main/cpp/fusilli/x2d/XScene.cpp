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

#include "core/XConfig.h"
#include "core/CCSX.h"
#include "XScene.h"
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(fusii)

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

//////////////////////////////////////////////////////////////////////////////
//
bool XScene::init() {
  bool ok= _usePhysics ? c::Scene::initWithPhysics() : c::Scene::init();
  if (ok) { decoUI(); }
  return ok;
}

///////////////////////////////////////////////////////////////////////////////
//
XScene::~XScene() {
}

/////////////////////////////////////////////////////////////////////////////
//
XScene::XScene() {
  bind(this);
}

/////////////////////////////////////////////////////////////////////////////
//
XScene::XScene(bool physics) {
  _usePhysics=physics;
  bind(this);
}



NS_END


