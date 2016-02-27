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