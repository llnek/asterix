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

#ifndef  __ENTITY_H__
#define  __ENTITY_H__

#include "../core/fusilli.h"
#include "cocos2d.h"
USING_NS_CC;
NS_FI_BEGIN

class CC_DLL Entity : public cocos2d::Ref {

private:
  CC_DISALLOW_COPY_AND_ASSIGN(Entity)

public:

  virtual ~Entity();
  Entity();

  void updatePosition(float x, float y);

  Sprite* sprite;
  Vec2 vel;
  Vec2* lastPos;
};

NS_FI_END
#endif

