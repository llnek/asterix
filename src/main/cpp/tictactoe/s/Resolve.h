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

#if  !defined(__RESOLVE_H__)
#define __RESOLVE_H__

#include "core/BaseSystem.h"
#include "EFactory.h"
#include "n/CObjs.h"

NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Resolve : public  f::BaseSystem<EFactory> {
protected:

  bool xrefCell(int pos, PlayView*, c::Vec2 &cell);

  bool checkWin(a::Node*, Player&, Grid*, ArrDim& );
  void showWinningIcons(a::Node*, const ArrDim& );
  void doWin(a::Node*, Player&, const ArrDim& );

  void syncUp(a::Node*);

  void doIt(a::Node*, float);
  void doForfeit(a::Node*);
  void doDraw(a::Node*);

  bool checkDraw(a::Node*, Grid*);
  void doDone(a::Node*, Player&);

public:

  virtual const a::SystemType typeId() { return "n/Resolve"; }

  Resolve(not_null<EFactory*>, not_null<c::Dictionary*>);

  virtual void addToEngine(not_null<a::Engine*> );

  virtual int priority() { return a::Resolve; }

  virtual bool update(float);

  virtual ~Resolve() {}
  NOCPYASS(Resolve)
  NODFT(Resolve)

  a::NodeList *board;
};



NS_END(tttoe)
#endif

