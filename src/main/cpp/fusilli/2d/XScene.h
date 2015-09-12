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
#include "XLayer.h"
#include <map>
NS_USING(cocos2d)
NS_USING(std)
NS_BEGIN(fusilli)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL XScene : public Scene {
protected:

  map<string,XLayer*> layers;
  XScene();

  virtual void OnQuitAction();

private:
  DISALLOW_COPYASSIGN(XScene)

public:

  virtual XScene* Realize();
  virtual ~XScene();

  XLayer* GetLayer(const string&);
  void AddLayer(XLayer*);

};





NS_END(fusilli)
#endif

