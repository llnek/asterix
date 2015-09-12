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
NS_USING(cocos2d)
NS_USING(ash)
NS_BEGIN(fusilli)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL ComObj : public ash::Component {
private:

public:
  Sprite* sprite;
  bool status;
  Vec2 vel;
  Vec2 lastPos;

  void Hurt(float damage);

  void Inflate(Dictionary* options);
  void Inflate(float x, float y);

  void Deflate();

  float Height();
  float Width();

  void SetPos(float x, float y);

  const Vec2 Pos();

  const Size CSize();

  int Pid();

  void UpdatePosition(float x, float y);

  /**
   * @private
   */
  ComObj(Sprite*, float health, float score);
  ComObj(Sprite*);

  virtual ~ComObj();
};

NS_END(fusilli)
#endif

