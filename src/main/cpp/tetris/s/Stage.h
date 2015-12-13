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

#if !defined(__STAGE_H__)
#define __STAGE_H__

#include "core/BaseSystem.h"
#include "n/GNodes.h"
#include "EFactory.h"

NS_BEGIN(tetris)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Stage : public f::BaseSystem<EFactory> {
protected:

  void xh(const c::Size& , float, float, float);
  void onceOnly(a::Node*);
  void doCtrl(a::Node*);
  void xv(const c::Size& , float);
  void onceOnly_2(a::Node*, const c::Size&,
      const c::Size& , const f::Box4&);

  const s_vec<f::FArrBrick*>
    initBlockMap(const s_vec<f::FArrInt*>& tiles);

  const s_vec<f::FArrInt*>
    fakeTileMap(const c::Size&, const f::Box4&);

public:

  virtual const a::SystemType typeId() { return "s/Stage"; }

  Stage(not_null<EFactory*>, not_null<c::Dictionary*>);

  virtual void addToEngine(not_null<a::Engine*>);

  virtual bool update(float);

  virtual int priority() { return a::PreUpdate; }

  NOCPYASS(Stage)
  NODFT(Stage)

  virtual ~Stage() {}

};


NS_END(tetris)
#endif


