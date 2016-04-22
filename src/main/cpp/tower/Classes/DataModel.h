#pragma once
#include "cocos2d.h"
#include "CCVector.h"
#include "WayPoint.h"
#include "Creep.h"
#include "Tower.h"
#include "projectile.h"
#include "GameHUD .h"
#include "TutorialSence.h"

USING_NS_CC;

class DataModel 
{
public:
	TutorialSence* _gameLayer;
	GameHUD*       _gameHUDLayer;
	Vector<WayPoint*> waypoints;
	Vector<Creep*> targets;
	Vector<Wave*> waves;
	Vector<Tower*> towers;
	Vector<Projectile*> projectiles;
	static DataModel* getModel();
private:
	DataModel(){};
	static DataModel * m_pInstance;
};