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

#pragma once
//////////////////////////////////////////////////////////////////////////////

#define __decl_get_iid(x) virtual int getIID() { return x; }
#define __decl_update() virtual void update(float);
#define __decl_deco_ui() virtual void decoUI();

#include "2d/CCSpriteBatchNode.h"
#include "platform/CCCommon.h"
#include "core/Primitives.h"
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL SCTX {
  __decl_mv(int, count,1)
  SCTX() {}
  virtual ~SCTX() {}
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL XNode {
protected:

  virtual c::Menu* addAudioIcons(const s_arr<c::MenuItem*,2>&,
    const CCT_PT &anchor, const CCT_PT &pos);

  virtual c::SpriteBatchNode* regoAtlas(const sstr &name, int zx = 0);
  virtual void bind(c::Node* p) { _self = p; }

  __decl_map(sstr, c::SpriteBatchNode*, _atlases)
  __decl_ptr(c::Node, _self)
  __decl_ptr(SCTX, _context)

  XNode() {}

public:

  virtual void addAtlasFrame(const sstr &atlas, const sstr &n, const CCT_PT &pos, int z, int tag=0);
  virtual void addAtlasFrame(const sstr &atlas, const sstr &n, const CCT_PT &pos);

  virtual void addAtlasItem(const sstr &atlas, not_null<c::Node*> n, int z, int tag=0);
  virtual void addAtlasItem(const sstr &atlas, not_null<c::Node*> n);

  virtual void addFrame(const sstr &n, const CCT_PT &pos, int z, int tag=0);
  virtual void addFrame(const sstr &n, const CCT_PT &pos);

  virtual c::SpriteBatchNode* getAtlas(const sstr &name);

  virtual void addItem(not_null<c::Node*> n, int z, int tag=0);
  virtual void addItem(not_null<c::Node*> n);

  virtual const sstr gets(const sstr&, const s_vec<sstr>&);
  virtual const sstr gets(const sstr&);

  virtual c::Sprite* centerImage(const sstr &n, int z= -1);
  virtual void removeAtlasAll(const sstr &atlas) ;
  virtual void removeItem(c::Node*);
  virtual void removeAll();

  virtual void setCtx(SCTX*, bool clean=true);
  virtual SCTX* getCtx() { return _context; }
  virtual SCTX* ejectCtx();

  virtual ~XNode();
};


NS_END


