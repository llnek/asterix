#pragma once
#include "cocos2d.h"
#include "Creep.h"
#include "projectile.h"

class Tower: public Sprite 
{
public:
	int range;
	Sprite* sprite;
	Creep* target;
	Sprite * selSpriteRange;
	Creep* getClosesTarget();
	CREATE_FUNC(Tower);
};

class MachineGunTower : public Tower 
{
public:
	Projectile* nextProjectile;
	static Tower* tower();
	bool virtual init();
	void towerLogic(float dt);
	void finishFiring();
	void creepMoveFinished(Node* sender);
};