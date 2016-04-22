#include "cocos2d.h"
#include "Constants.h"

USING_NS_CC;

class GameObject :public Sprite
{
public:
	GameObjectType  type;

	virtual bool init();

	CREATE_FUNC(GameObject);
};

