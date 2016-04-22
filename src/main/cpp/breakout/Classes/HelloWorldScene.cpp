#include "HelloWorldScene.h"
#include "GameOverScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();

	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

	Vect gravity(0.0f, 0.0f);
	scene->getPhysicsWorld()->setGravity(gravity);
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

	layer->setPhyWorld(scene->getPhysicsWorld());

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	
    
	edgeSp = Sprite::create();
	auto boundBody = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
	boundBody->getShape(0)->setRestitution(1.0f);
	boundBody->getShape(0)->setFriction(0.0f);
	boundBody->getShape(0)->setDensity(1.0f);
	edgeSp->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	edgeSp->setPhysicsBody(boundBody);
	this->addChild(edgeSp);
	edgeSp->setTag(0);

	ball = Sprite::create("Ball.png", Rect(0, 0, 52, 52));
	ball->setPosition(100, 100);
	auto ballBody = PhysicsBody::createCircle(ball->getContentSize().width / 2.);
	ballBody->getShape(0)->setRestitution(1.0f);
	ballBody->getShape(0)->setFriction(0.0f);
	ballBody->getShape(0)->setDensity(1.0f);
	ballBody->setGravityEnable(false);
	Vect force = Vect(1000000.0f, 1000000.0f);
	ballBody->applyImpulse(force);
	ball->setPhysicsBody(ballBody);
	ball->setTag(1);
	this->addChild(ball);

	paddle = Sprite::create("Paddle.png");
	auto paddleBody = PhysicsBody::createBox(paddle->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
	paddleBody->getShape(0)->setRestitution(1.0f);
	paddleBody->getShape(0)->setFriction(0.0f);
	paddleBody->getShape(0)->setDensity(10.0f);
	paddleBody->setGravityEnable(false);
	paddleBody->setDynamic(false);
	paddle->setPosition(visibleSize.width / 2, 50);
	paddle->setPhysicsBody(paddleBody);
	ball->setTag(2);
	this->addChild(paddle);

	for (int i = 0; i < 4; i++) {

		static int padding = 100;

		// 创建方块并添加到层中
		auto block = Sprite::create("blocks.png");
		auto blockBody = PhysicsBody::createBox(block->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
		blockBody->getShape(0)->setDensity(10.0f);
		blockBody->getShape(0)->setFriction(0.0f);
		blockBody->getShape(0)->setRestitution(1.f);
		blockBody->setDynamic(false);
		int xOffset = padding + block->getContentSize().width / 2 +
			((block->getContentSize().width + padding)*i);
		block->setPosition(xOffset, 450);
		block->setPhysicsBody(blockBody);
		block->setTag(3);
		this->addChild(block);
	}

	this->schedule(schedule_selector(HelloWorld::tick),0);

    return true;
}

void HelloWorld::tick(float dt)
{	
	bool isWin = true;
	Vector<PhysicsBody*> bodies = m_world->getAllBodies();
	for each(PhysicsBody* body in bodies)
	{
		if (body->getNode()->getTag() == 3)
		{
			isWin = false;
		}
	}
	if (isWin == true)
	{
		auto gameOverScene = GameOverScene::create();
		gameOverScene->getLayer()->getLabel()->setString("You Win!");
		Director::getInstance()->replaceScene(gameOverScene);
	}
}

void HelloWorld::onEnter()
{
	Layer::onEnter();

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_2(HelloWorld::onContactBegin, this);
	

	auto dispatcher = Director::getInstance()->getEventDispatcher();

	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	dispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}

bool HelloWorld::onContactBegin(EventCustom* event, const PhysicsContact& contact)
{
	auto spriteA = (Sprite*)contact.getShapeA()->getBody()->getNode();
	auto spriteB = (Sprite*)contact.getShapeB()->getBody()->getNode();
	int tagA = spriteA->getTag();
	int tagB = spriteB->getTag();

	if (tagA == 3)
	{
		spriteA->removeFromParentAndCleanup(true);
	}

	if (tagB == 3)
	{
		spriteB->removeFromParentAndCleanup(true);
	}

	if (ball->getPositionY() <= 50)
	{
		auto gameOverScene = GameOverScene::create();
		gameOverScene->getLayer()->getLabel()->setString("You Lose!");
		Director::getInstance()->replaceScene(gameOverScene);
	}
	return true;
}

void HelloWorld::onTouchMoved(Touch* touch, Event* event)
{
	Point touchLocation = this->convertToWorldSpace(this->convertTouchToNodeSpace(touch));
	paddle->setPositionX(touchLocation.x);
	
}