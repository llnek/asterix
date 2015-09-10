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
NS_USING(ash)
NS_BEGIN(invaders)


enum class Rank {

};

//////////////////////////////////////////////////////////////////////////
//
class CC_DLL Factory {
public:
  virtual ~Factory();
  Factory(Engine* e, options);
  void CreateMissiles(int count= 36);
  void CreateExplosions(int count = 24);
  void CreateBombs(int count = 24);
  const Size CalcImgSize(const string& img);
  GetRankInfo(const Rank r);
  void FillSquad(XPool* pool);
  void CreateAliens();
  void BornShip();
  void CreateShip();

private:

  DISALLOW_COPYASSIGNDFT(Factory)

};


NS_END(invaders)
