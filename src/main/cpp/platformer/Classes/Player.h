#include "cocos2d.h"
#include "GameObject.h"

USING_NS_CC;

class Player :public GameObject 
{
public:
	PhysicsBody* body;

	void createdObject();
	void jump();
	void moveRight();

	virtual bool init();

	CREATE_FUNC(Player);
};

