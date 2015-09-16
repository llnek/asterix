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

#include "ash/Ash.h"
NS_ALIAS(f, fusilli)
NS_ALIAS(s, std)
NS_ALIAS(a, ash)
NS_BEGIN(invaders)


//////////////////////////////////////////////////////////////////////////
//
enum class Rank {

};

class a::Engine;
class f::XPool;

//////////////////////////////////////////////////////////////////////////
//
class CC_DLL Factory {
protected:

  cc::Dictionary* state;
  a::Engine engine;

private:

  DISALLOW_COPYASSIGN(Factory)
  Factory();

public:

  Factory(a::Engine* e, cc::Dictionary* options);
  void CreateMissiles(int count= 36);
  void CreateExplosions(int count = 24);
  void CreateBombs(int count = 24);

  const cc::Size CalcImgSize(const s::string& img);
  GetRankInfo(const Rank r);

  void FillSquad(f::XPool* );
  void CreateAliens();
  void BornShip();
  void CreateShip();

  virtual ~Factory();
};


NS_END(invaders)


