// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Ken Leung. All rights reserved.

#pragma once

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

  virtual void setPos(float x, float y);
  virtual void hurt(int damage=1);
  virtual const c::Size csize();

  void show();
  void hide();

  void mortal() { godMode=false; }
  void god() { godMode=true; }
  void freeze() { status=false; }

  const c::Rect bbox();
  const c::Vec2 pos();

  virtual void place() {
    sprite->setPosition(nextPos); }


  virtual float circum();
  virtual float radius();
  virtual float height();
  virtual float width();

  //virtual float bottom();
  //virtual float top();
  //virtual float right();
  //virtual float left();

  int pid();

  ComObj(not_null<c::Sprite*>, int health, int score);
  ComObj(not_null<c::Sprite*>);
  virtual ~ComObj() {}
  ComObj();
  NOCPYASS(ComObj)

  //not owner
  DECL_PTR(c::Sprite ,sprite)
  DECL_IZ(origHP)
  DECL_BF(status)
  DECL_BF(godMode)
  DECL_IZ(HP)
  DECL_IZ(score)

  DECL_TD(c::Vec2, lastPos)
  DECL_TD(c::Vec2, nextPos)

  //DECL_TD(c::Vec2, maxVel)
  //DECL_TD(c::Vec2, speed)

  DECL_FZ(maxSpeed)
  DECL_FZ(speed)

  DECL_TD(c::Vec2, acc)
  DECL_TD(c::Vec2, vel)

  DECL_FZ(power)
  //degrees
  DECL_FZ(angle)

};


NS_END



