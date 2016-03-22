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
#define MDECL_GET_IID(x) virtual int getIID() { return x; }
#define MDECL_UPDATE() virtual void update(float);
#define MDECL_DECORATE() virtual void decoUI();

#include "2d/CCSpriteBatchNode.h"
#include "platform/CCCommon.h"
#include "core/Primitives.h"
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL SCTX {
  DECL_TV(int, count,1)
  virtual ~SCTX() {}
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL XNode {
protected:

  virtual c::Menu* addAudioIcons(const s_arr<c::MenuItem*,2>&,
    const c::Vec2 &anchor, const c::Vec2 &pos);

  virtual c::SpriteBatchNode* regoAtlas( const sstr &name, int zx = 0);
  virtual void bind(c::Node* p) { _self = p; }

  s_map<sstr, c::SpriteBatchNode*> _atlases;
  DECL_PTR(c::Node, _self)
  DECL_PTR(SCTX, _context)

  XNode() {}

public:

  virtual void addAtlasFrame(const sstr &atlas, const sstr &n, const c::Vec2 &pos, int z, int tag=0);
  virtual void addAtlasFrame(const sstr &atlas, const sstr &n, const c::Vec2 &pos);

  virtual void addAtlasItem(const sstr &atlas, not_null<c::Node*> n, int z, int tag=0);
  virtual void addAtlasItem(const sstr &atlas, not_null<c::Node*> n);

  virtual void addFrame(const sstr &n, const c::Vec2 &pos, int z, int tag=0);
  virtual void addFrame(const sstr &n, const c::Vec2 &pos);

  virtual c::SpriteBatchNode* getAtlas(const sstr &name);

  virtual void addItem(not_null<c::Node*> n, int z, int tag=0);
  virtual void addItem(not_null<c::Node*> n);

  virtual const sstr gets(const sstr&, const s_vec<sstr>&);
  virtual const sstr gets(const sstr&);

  virtual void centerImage(const sstr &n, int z= -1);
  virtual void removeAtlasAll(const sstr &atlas) ;
  virtual void removeItem(c::Node*);
  virtual void removeAll();

  virtual void setCtx(SCTX*, bool clean=true);
  virtual SCTX* getCtx() { return _context; }
  virtual SCTX* ejectCtx();

  virtual ~XNode();
};


NS_END


