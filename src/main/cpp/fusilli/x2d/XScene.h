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

#if !defined(__XSCENE_H__)
#define __XSCENE_H__

#include "2d/CCScene.h"
#include "core/Odin.h"
#include "XLayer.h"

NS_ALIAS(c, cocos2d)
NS_ALIAS(s, std)
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL XScene : public c::Scene {
protected:

  NO__CPYASS(XScene)
  XScene() {}

public:

  XLayer*  AddLayer(not_null<XLayer*>, int z=0);
  XLayer*  GetLayer(int tag);

  virtual bool IsRunning() { return false; }
  virtual XScene* Realize() = 0;
  virtual ~XScene() {}
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL XSceneFactory  {
public:
  static XScene* ReifySimple(s::function<void (XLayer*)> );
};


NS_END(fusii)
#endif

