#include "cocos2d.h"
#include "Player.h"

USING_NS_CC;

class GameScene :public Layer
{
public:
	Size screenSize;
	
	PhysicsWorld* m_world;

	Player* player;

	TMXTiledMap* tileMapNode;

	void setPhyWorld(PhysicsWorld* world){ m_world = world; };

	void makeBox2dObjAt(Point p, Size size, bool d, float r, float friction, float density, float restitution);
	
	void drawCollisionTiles();

	void addScrollingBackgroundWithTileMap();

	void virtual update(float dt);

	static cocos2d::Scene* createScene();

	virtual bool init();

	virtual void onEnter();

	bool onTouchBegan(Touch *touch, Event *event);

	bool onContactBegin(EventCustom* event, const PhysicsContact& contact);

	CREATE_FUNC(GameScene);
};