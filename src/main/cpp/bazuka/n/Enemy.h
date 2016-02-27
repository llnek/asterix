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
//////////////////////////////////////////////////////////////////////////////

#include "core/XConfig.h"
#include "core/ComObj.h"
#include "core/CCSX.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(bazuka)


#ifndef __wp8Game__Enemy__
#define __wp8Game__Enemy__

#pragma once
#include "cocos2d.h"
using namespace cocos2d;

#include "ProjectileLayer.h"

class Enemy : public CCSprite
{
public:
	Enemy(void);
	~Enemy(void);

	static Enemy* createEnemy(ProjectileLayer* _projLayer);
	bool initEnemy(ProjectileLayer* _projLayer);
	void update();
	void shoot(float dt);


	CCArray* projectiles;
	ProjectileLayer* projLayer;

};


#endif
