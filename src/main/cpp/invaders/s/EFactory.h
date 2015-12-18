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

#if !defined(__EFACTORY_H__)
#define __EFACTORY_H__

#include "core/Factory.h"
NS_BEGIN(invaders)

//////////////////////////////////////////////////////////////////////////
//
struct CC_DLL EFactory : public f::Factory {
public:

  EFactory(not_null<a::Engine*>, not_null<c::Dictionary*> options);

  void reifyExplosions(int count = 24);
  void reifyMissiles(int count= 36);
  void reifyBombs(int count = 24);

  a::Entity* reifyAliens();
  a::Entity* reifyShip();

  const c::Size getRankInfo(int r, c::Dictionary*);
  const c::Size calcImgSize(const sstr &img);

  void fillSquad(f::XPool* );
  void bornShip();

  virtual ~EFactory() {}
  NODFT(EFactory)
  NOCPYASS(EFactory)

  f::ComObj* player=nullptr;
};


NS_END(invaders)
#endif


