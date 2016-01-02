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

  c::SpriteBatchNode* regoAtlas(const sstr &name, int zx = 0);
  s_map<sstr, c::SpriteBatchNode*> atlases;

  DECL_TV(int, lastTag, 1)
  DECL_TV(int, lastZ, 0)

public:

  virtual c::SpriteBatchNode* getAtlas(const sstr &name);
  virtual void onQuit(c::Ref*);

  virtual bool initEx(XScene*, int zx = 0);
  virtual void decorate() = 0;

  void addAtlasItem( const sstr &atlas, not_null<c::Node*> n, int z, int tag);
  void addAtlasItem( const sstr &atlas, not_null<c::Node*> n);

  void addItem(not_null<c::Node*> n, int z, int tag);
  void addItem(not_null<c::Node*> n);

  void addFrame(const sstr &n, const c::Vec2 &pos, int z, int tag);
  void addFrame(const sstr &n, const c::Vec2 &pos);

  void addAtlasFrame( const sstr &atlas, const sstr &n, const c::Vec2 &pos, int z, int tag);
  void addAtlasFrame( const sstr &atlas, const sstr &n, const c::Vec2 &pos);

  void centerImage(const sstr &n, int z= -1);

  void removeAtlasAll(const sstr &atlas) ;
  void removeAll();
  void removeItem(not_null<c::Node*> n);

  XScene* getSceneX();
  int incIndexZ();
  float getDelay();

  const sstr gets(const sstr&, const s_vec<sstr>&);
  const sstr gets(const sstr&);

  // tag value
  virtual int getIID() { return 1; }

  virtual ~XLayer() {}
  XLayer() {}

  NOCPYASS(XLayer)
};



NS_END(fusii)
#endif

