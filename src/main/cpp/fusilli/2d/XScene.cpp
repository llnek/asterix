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

#include "XScene.h"
NS_FI_BEGIN
USING_NS_CC;

//////////////////////////////////////////////////////////////////////////////
//
XScene::getLayers() {
  return this.layers;
}

bool XScene::init() {
  if (CCScene::init()) {
    createLayers();
    return true;
  } else {
    return false;
  }
}

void XScene::createLayers() {
  //hold off init'ing game layer, leave that as last
  Layer* glptr = nullptr;

  for (auto it = m_layers->begin(); it != m_layers->end(); ++it ) {
    dynamic_cast<XGameLayer*>(*it);
  }
    rc = R.any((proto) => {
      obj= new (proto)(this.options);
      if ( obj instanceof XGameLayer ) {
        glptr = obj;
      }
      else
      if (obj instanceof XLayer) {
        obj.init();
      }

      if (obj instanceof XLayer) {
        obj.setParentScene(this);
      }

      this.layers[ obj.rtti() ] = obj;
      this.addChild(obj);
      return false;
    }, a);

    if (a.length > 0 && rc===false ) {
      if (!!glptr) {
        glptr.init();
      }
    }
}

void XScene::onmsg() {
  //this.ebus.on(topic, cb);
}

XScene::~XScene() {
}

XScene::XScene()
: m_options(nullptr),
  m_layers(nullptr) {
}

XScene::XScene(CCArray* layers, CCDictionary* options) {

  m_options = options;
  options->retain();

  m_layers= layers;
  layers->retain();

};


NS_FI_END
