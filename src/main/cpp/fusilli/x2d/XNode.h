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

#if !defined(__XNODE_H__)
#define __XNODE_H__

#include "2d/CCSpriteBatchNode.h"
#include "platform/CCCommon.h"
#include "core/Primitives.h"
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL XNode {
protected:

  c::SpriteBatchNode* regoAtlas(not_null<c::Node*>, const sstr &name, int zx = 0);
  s_map<sstr, c::SpriteBatchNode*> atlases;
  DECL_TV(int, lastTag, 0)
  DECL_TV(int, lastZ, 0)
  XNode() {}

public:

  void addAtlasFrame( not_null<c::Node*>, const sstr &atlas, const sstr &n, const c::Vec2 &pos, int z, int tag);
  void addAtlasFrame( not_null<c::Node*>, const sstr &atlas, const sstr &n, const c::Vec2 &pos);

  void addAtlasItem( not_null<c::Node*>, const sstr &atlas, not_null<c::Node*> n, int z, int tag);
  void addAtlasItem( not_null<c::Node*>, const sstr &atlas, not_null<c::Node*> n);

  void addFrame(not_null<c::Node*>, const sstr &n, const c::Vec2 &pos, int z, int tag);
  void addFrame(not_null<c::Node*>, const sstr &n, const c::Vec2 &pos);

  virtual c::SpriteBatchNode* getAtlas(const sstr &name);

  void addItem(not_null<c::Node*> par, not_null<c::Node*> n, int z, int tag);
  void addItem(not_null<c::Node*> par, not_null<c::Node*> n);

  const sstr gets(const sstr&, const s_vec<sstr>&);
  const sstr gets(const sstr&);

  void centerImage(not_null<c::Node*>, const sstr &n, int z= -1);
  void removeAtlasAll(const sstr &atlas) ;
  void removeAll(not_null<c::Node*> par);
  void removeItem(not_null<c::Node*> n);

  int zIndex() { return lastZ; }
  int incIndexZ();

  virtual ~XNode() {}
};

NS_END(fusii)
#endif

