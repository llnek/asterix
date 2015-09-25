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

#if !defined(__COMOBJ_H__)
#define __COMOBJ_H__

#include "core/Primitives.h"
#include "2d/CCSprite.h"
#include "ash/Ash.h"

NS_ALIAS(a, ash)
NS_BEGIN(fusilli)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL ComObj : public a::Component {
protected:

  ComObj();

private:

  void Init(c::Sprite*, int health, int score);
  NO__COPYASSIGN(ComObj)

public:

  c::Sprite* sprite;
  int origHealth;
  bool status;
  int health;
  int score;

  c::Vec2 lastPos;
  c::Vec2 vel;

  void Inflate(not_null<c::Dictionary*> options);
  void Inflate(float x, float y);
  void Inflate();
  void Deflate();

  void SetPos(float x, float y);
  void Hurt(int damage=1);

  const c::Size CSize();
  const c::Vec2 Pos();

  float Height();
  float Width();
  int Pid();

  void UpdatePosition(float x, float y);

  ComObj(c::Sprite*, int health, int score);
  ComObj(c::Sprite*);

  virtual ~ComObj() {}
};



NS_END(fusilli)
#endif

