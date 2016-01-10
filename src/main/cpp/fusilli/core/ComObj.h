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

#include "2d/CCSprite.h"
#include "Primitives.h"
#include "ash/Ash.h"
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL ComObj : public ash::Component {
protected:

  void init(c::Sprite*, int health, int score);

public:

  virtual const a::COMType typeId() { return "fusii::ComObj"; }

  virtual void inflate(not_null<c::Dictionary*> options);
  virtual void updatePosition(float x, float y);
  virtual void inflate(float x, float y);
  virtual void inflate();
  virtual void deflate();
  virtual void hurt(int damage=1);

  void setPos(float x, float y);
  const c::Size csize();
  const c::Vec2 pos();
  const c::Rect bbox();

  float height();
  int pid();
  float width();

  ComObj(not_null<c::Sprite*>, int health, int score);
  ComObj(not_null<c::Sprite*>);
  virtual ~ComObj() {}
  ComObj();
  NOCPYASS(ComObj)

  //not owner
  DECL_PTR(c::Sprite ,sprite)
  DECL_IZ(origHP)
  DECL_BF(status)
  DECL_IZ(HP)
  DECL_IZ(score)

  DECL_TD(c::Vec2, lastPos)
  DECL_TD(c::Vec2, speed)
  DECL_TD(c::Vec2, acc)
  DECL_TD(c::Vec2, vel)
  DECL_TD(c::Vec2, maxVel)
  DECL_FZ(power)
  //degrees
  DECL_FZ(angle)

};


NS_END(fusii)
#endif

