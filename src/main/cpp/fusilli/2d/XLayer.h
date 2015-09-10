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

#if !defined(__XLAYER_H__)
#define __XLAYER_H__

#include <stdint.h>
#include "core/fusilli.h"
#include "2d/CCLayer.h"
NS_USING(cocos2d)
NS_USING(std)
NS_BEGIN(fusilli)


//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL XLayer : public Layer {

public:

  MenuItem* CreateMenuBtn(const string& n,
      const string& s, const string& d);

  SpriteBatchNode* RegoAtlas(const string& name,
      int z= INT32_MIN, int tag = INT32_MIN);

  virtual const string& Moniker() = 0;

  virtual XLayer* Realize();

  virtual void PKInput();

  virtual void AddAudioIcon(Dictionary* options);

  virtual void CenterAtlasImage(const string& atlas, const string& frame);

  virtual void CenterImage(const string& frame);

  virtual void AddAtlasFrame(const string& atlas,
      const Vec2& pos, const string& frame);

  virtual void AddFrame(const string& frame, const Vec2& pos);

  virtual SpriteBatchNode* GetAtlas(const string& name);

  virtual void RemoveAtlasAll(const string& atlas, bool c=true) ;

  virtual void RemoveAtlasItem(const string& atlas, Node* n, bool c=true);

  virtual void RemoveAll(bool c= true);

  virtual void RemoveItem(Node* n, bool c=true);

  virtual void AddAtlasItem(const string& atlas, Node* n, int zx = INT32_MIN, int tag = INT32_MIN);

  virtual void AddItem(Node* n, int zx = INT32_MIN, int tag = INT32_MIN);

  virtual int IncIndexZ();

  Scene* GetScene();

  //virtual bool init() override;

  virtual ~XLayer();
  XLayer();

  CREATE_FUNC(XLayer)

private:

  map<string,x> atlases;
  int lastTag;
  int lastZix;

  DISALLOW_COPYASSIGN(XLayer)
};





NS_END(fusilli)
#endif

