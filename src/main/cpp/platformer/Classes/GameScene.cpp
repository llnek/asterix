#include "GameScene.h"

Scene* GameScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::createWithPhysics();

	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	/*Vect gravity(0.0f, -600.0f);
	scene->getPhysicsWorld()->setGravity(gravity);*/

	// 'layer' is an autorelease object
	auto layer = GameScene::create();

	layer->setPhyWorld(scene->getPhysicsWorld());

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool GameScene::init()
{
	if(!Layer::init())
	{
		return false;
	}
	
	// enable touches

	screenSize = Director::getInstance()->getWinSize();

	this->addScrollingBackgroundWithTileMap();
	this->drawCollisionTiles();

	player = (Player*)Sprite::create("Icon-Small.png");
	player->retain();
	player->setPosition(100.0f, 180.0f);
	player->createdObject();

	this->addChild(player);

	// Start main game loop
	this->scheduleUpdate();

	return true;
}

void GameScene::makeBox2dObjAt(Point p, Size size, bool d, float r, float friction, float density, float restitution)
{
	auto sprite = Sprite::create();
	auto body = PhysicsBody::createBox(size, PHYSICSBODY_MATERIAL_DEFAULT);
	body->getShape(0)->setDensity(density);
	body->getShape(0)->setFriction(friction);
	body->getShape(0)->setRestitution(restitution);
	body->setDynamic(d);
	sprite->setPosition(p);
	sprite->setPhysicsBody(body);
	this->addChild(sprite);
}

void GameScene::drawCollisionTiles()
{
	TMXObjectGroup *objects = tileMapNode->objectGroupNamed("Collision");
	float x, y, w, h;
	ValueVector objectsPoint = objects->getObjects();
	Value objPointMap;
	for each(objPointMap in objectsPoint)
	{
		ValueMap objPoint = objPointMap.asValueMap();
		x = objPoint.at("x").asFloat();
		y = objPoint.at("y").asFloat();
		w = objPoint.at("width").asFloat();
		h = objPoint.at("height").asFloat();

		Point _point = Point(x + w / 2.0f, y + h / 2.0f);
		Size _size = Size(w, h);

		this->makeBox2dObjAt(_point, _size, false, 0, 0.0f, 0.0f, 0);
	}
}

void GameScene::addScrollingBackgroundWithTileMap()
{
	tileMapNode = TMXTiledMap::create("scroller.tmx");
	tileMapNode->setAnchorPoint(Point(0, 0));
	this->addChild(tileMapNode);
}

void GameScene::update(float dt)
{
	Point pos = player->getPosition();
	this->setPosition(-1 * pos.x + 100, this->getPositionY());
}

void GameScene::onEnter()
{
	Layer::onEnter();

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_2(GameScene::onContactBegin, this);


	auto dispatcher = Director::getInstance()->getEventDispatcher();

	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	dispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}

bool GameScene::onContactBegin(EventCustom* event, const PhysicsContact& contact)
{
	return true;
}

bool GameScene::onTouchBegan(Touch *touch, Event *event)
{
	Point touchLocation = this->convertToWorldSpace(this->convertTouchToNodeSpace(touch));

	if (touchLocation.x >= screenSize.width / 2) 
	{
		player->moveRight();
	}
	else 
	{
		player->jump();
	}
	return TRUE;
}