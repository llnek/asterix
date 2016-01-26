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

#include "2d/CCLayer.h"
#include "XNode.h"
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////////
//
class XScene;
class CC_DLL XLayer : public XNode, public c::Layer {
protected:

  virtual void postDeco() {}
  virtual void preDeco() {}

public:

  virtual bool initEx(XScene*, int zx = 0);
  virtual void decorate() =0;
  virtual void postReify() {}

  XScene* getSceneX();

  // tag value
  virtual int getIID() { return 1; }
  virtual ~XLayer() {}

  XLayer();
  NOCPYASS(XLayer)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL SingleLayer : public XLayer {
  virtual void decorate() {}
  virtual ~SingleLayer() {}
  SingleLayer() {}
  NOCPYASS(SingleLayer)
};




NS_END



