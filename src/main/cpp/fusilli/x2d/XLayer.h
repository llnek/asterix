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

#include "2d/CCSpriteBatchNode.h"
#include "platform/CCCommon.h"
#include "2d/CCLayer.h"
#include "core/Primitives.h"
NS_BEGIN(fusii)

class SpriteBatchNode;
class XScene;
//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL XLayer : public c::Layer {
protected:

  s::map<stdstr, c::SpriteBatchNode*> atlases;

  int lastTag;
  int lastZ;

  c::SpriteBatchNode* regoAtlas(const stdstr& name,
      const MaybeInt& = MaybeInt(),
      const MaybeInt& = MaybeInt() );

  NO__CPYASS(XLayer)
  XLayer();

public:

  virtual c::SpriteBatchNode* getAtlas(const stdstr& name);
  virtual void onQuit(c::Ref*);
  virtual XLayer* realize();

  virtual void addAtlasItem(const stdstr& atlas,
      not_null<c::Node*> n,
      const MaybeInt& = MaybeInt(),
      const MaybeInt& = MaybeInt() );

  virtual void addItem(not_null<c::Node*> n,
      const MaybeInt& = MaybeInt() ,
      const MaybeInt& = MaybeInt() );

  void addAtlasFrame(const stdstr& atlas, const stdstr& n, const c::Vec2& pos);

  void addFrame(const stdstr& n, const c::Vec2& pos);

  void centerImage(const stdstr& n, int z= -1);

  void removeAtlasAll(const stdstr& atlas) ;
  void removeAll();
  void removeItem(not_null<c::Node*> n);

  XScene* getSceneX();
  int incIndexZ();

  // tag value
  virtual int getIID() { return 1; }
  virtual ~XLayer();

};



NS_END(fusii)
#endif

