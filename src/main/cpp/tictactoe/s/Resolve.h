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

#if !defined(__RESOLVE_H__)
#define __RESOLVE_H__

#include "core/BaseSystem.h"
#include "EFactory.h"
#include "n/cobjs.h"
NS_ALIAS(f, fusii)
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Resolve : f::BaseSystem<EFactory> {
protected:

  virtual bool OnUpdate(float);
  void SyncUp(a::Node*);

  const c::Vec2 XrefCell(int pos, PlayView*);

  void DoIt(a::Node*, float);
  void DoWin(a::Node*, Player&,
      const s::array<int,BD_SZ>& combo);

  void ShowWinningIcons(a::Node*, const s::array<int, BD_SZ>&);
  void DoForfeit(a::Node*);
  void DoDraw(a::Node*);

  void DoDone(a::Node*, Player&);
  bool CheckDraw(a::Node*, Grid*);

  bool CheckWin(a::Node*, Player&, Grid*,
      s::array<int, BD_SZ>& combo);

  NO__CPYASS(Resolve)
  Resolve() = delete;

public:

  Resolve(not_null<EFactory*>, not_null<c::Dictionary*>);
  virtual ~Resolve() {}

  virtual void AddToEngine(not_null<a::Engine*> );

  virtual int Priority() { return a::Resolve; }

  virtual const a::SystemType TypeId() {
    return "n/Resolve";
  }

  a::NodeList* board;
};



NS_END(tttoe)


