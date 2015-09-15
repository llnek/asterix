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


#include "platform/CCCommon.h"
#include "2d/CCSprite.h"
#include "ash/Component.h"
NS_ALIAS(cc, cocos2d)
NS_ALIAS(a, ash)
NS_ALIAS(s,std)
NS_BEGIN(fusilli)


//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL ComObj : public a::Component {

private:

  DISALLOW_COPYASSIGN(ComObj)

public:

  cc::Sprite* sprite;
  bool status;
  int origHealth;
  int health;
  int score;

  cc::Vec2 lastPos;
  cc::Vec2 vel;

  void Inflate(cc::Dictionary* options);
  void Inflate(float x, float y);
  void Deflate();

  void SetPos(float x, float y);
  void Hurt(int damage);

  const cc::Size CSize();
  const cc::Vec2 Pos();

  float Height();
  float Width();
  int Pid();

  void UpdatePosition(float x, float y);

  ComObj(cc::Sprite*, int health, int score);
  ComObj(cc::Sprite*);

  virtual ~ComObj();
};



NS_END(fusilli)
#endif

