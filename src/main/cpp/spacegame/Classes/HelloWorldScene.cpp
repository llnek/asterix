#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

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
    
	_batchNode = SpriteBatchNode::create("Sprites.pvr.ccz"); // 1
	this->addChild(_batchNode); // 2
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Sprites.plist"); // 3

	_ship = Sprite::createWithSpriteFrameName("SpaceFlier_sm_1.png"); // 4
	auto winSize = Director::getInstance()->getWinSize(); // 5
	_ship->setPosition(Point(winSize.width *0.1, winSize.height *0.5)); // 6
	_batchNode->addChild(_ship,1); // 7



	// 1) Create the CCParallaxNode
	_backgroundNode = ParallaxNodeExtras::node();
	this->addChild(_backgroundNode,-1);

	// 2) Create the sprites we'll add to the CCParallaxNode
	_spacedust1 = Sprite::create("bg_front_spacedust.png");
	_spacedust2 = Sprite::create("bg_front_spacedust.png");
	_planetsunrise = Sprite::create("bg_planetsunrise.png");
	_galaxy = Sprite::create("bg_galaxy.png");
	_spacialanomaly = Sprite::create("bg_spacialanomaly.png");
	_spacialanomaly2 = Sprite::create("bg_spacialanomaly2.png");

	// 3) Determine relative movement speeds for space dust and background
	Point dustSpeed = Point(0.1, 0.1);
	Point bgSpeed = Point(0.05, 0.05);

	// 4) Add children to CCParallaxNode
	_backgroundNode->addChild(_spacedust1,0,dustSpeed,Point(0, winSize.height / 2));
	_backgroundNode->addChild(_spacedust2,0,dustSpeed,Point(_spacedust1->getContentSize().width, winSize.height / 2));
	_backgroundNode->addChild(_galaxy, -1, bgSpeed,Point(0, winSize.height *0.7));
	_backgroundNode->addChild(_planetsunrise, -1, bgSpeed,Point(600, winSize.height * 0));
	_backgroundNode->addChild(_spacialanomaly, -1, bgSpeed,Point(900, winSize.height *0.3));
	_backgroundNode->addChild(_spacialanomaly2, -1, bgSpeed,Point(1500, winSize.height *0.9));
    
	this->scheduleUpdate();

	HelloWorld::addChild(CCParticleSystemQuad::create("Stars1.plist"));
	HelloWorld::addChild(CCParticleSystemQuad::create("Stars2.plist"));
	HelloWorld::addChild(CCParticleSystemQuad::create("Stars3.plist"));

	this->setAccelerometerEnabled(true);


	#define KNUMASTEROIDS 15
	for (int i = 0; i < KNUMASTEROIDS; ++i) 
	{
		Sprite *asteroid = Sprite::createWithSpriteFrameName("asteroid.png");

		asteroid->setVisible(false);
		_batchNode->addChild(asteroid,5);
		_asteroids.pushBack(asteroid);
	}


	#define KNUMLASERS 5
	for (int i = 0; i < KNUMLASERS; ++i) 
	{
		auto *shipLaser = Sprite::createWithSpriteFrameName("laserbeam_blue.png");

		shipLaser->setVisible(false);
		_batchNode->addChild(shipLaser);
		_shipLasers.pushBack(shipLaser);
	}

	_lives = 3;
	double curTime = getTimeTick();
	_gameOverTime = curTime + 30000;

	SimpleAudioEngine::getInstance()->playBackgroundMusic("SpaceGame.wav", true);
	SimpleAudioEngine::getInstance()->preloadEffect("explosion_large.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("laser_ship.wav");

    return true;
}

void HelloWorld::update(float dt)
{
	Point backgroundScrollVel = Point(-1000, 0);
	_backgroundNode->setPosition(_backgroundNode->getPosition()+backgroundScrollVel*dt);


	Vector<Sprite*> spaceDusts;
	spaceDusts.pushBack(_spacedust1);
	spaceDusts.pushBack(_spacedust2);
	for each(Sprite *spaceDust in spaceDusts)
	{
		if ((_backgroundNode->convertToWorldSpace(spaceDust->getPosition()).x < -spaceDust->getContentSize().width))
		{
			_backgroundNode->incrementOffset(Point(2 * spaceDust->getContentSize().width, 0),spaceDust);
		}
	}

	Vector<Sprite*> backgrounds;
	backgrounds.pushBack(_planetsunrise);
	backgrounds.pushBack(_galaxy);
	backgrounds.pushBack(_spacialanomaly);
	backgrounds.pushBack(_spacialanomaly2);
	for each(Sprite *background in backgrounds)
	{
		if ((_backgroundNode->convertToWorldSpace(background->getPosition()).x < -background->getContentSize().width))
		{
			_backgroundNode->incrementOffset(Point(2000, 0),background);
		}
	}

	auto winSize = Director::getInstance()->getWinSize();
	float maxY = winSize.height - _ship->getContentSize().height / 2;
	float minY = _ship->getContentSize().height / 2;

	float diff = (_shipPointsPerSecY * dt);
	float newY = _ship->getPosition().y + diff;
	newY = MIN(MAX(newY, minY), maxY);
	_ship->setPosition(ccp(_ship->getPosition().x, newY));


	float curTimeMillis = getTimeTick();
	if (curTimeMillis > _nextAsteroidSpawn) 
	{

		float randMillisecs = randomValueBetween(0.20, 1.0) * 1000;
		_nextAsteroidSpawn = randMillisecs + curTimeMillis;

		float randY = randomValueBetween(0.0, winSize.height);
		float randDuration = randomValueBetween(2.0, 10.0);

		Sprite *asteroid = _asteroids.at(_nextAsteroid);
		_nextAsteroid++;

		if (_nextAsteroid >= _asteroids.size())
			_nextAsteroid = 0;

		asteroid->stopAllActions();
		asteroid->setPosition(Point(winSize.width +asteroid->getContentSize().width / 2, randY));
		asteroid->setVisible(true);
		asteroid->runAction(Sequence::create(
			MoveBy::create(randDuration, Point(-winSize.width - asteroid->getContentSize().width, 0)),
			CallFuncN::create(this, callfuncN_selector(HelloWorld::setInvisible)),
			NULL
			));
	}

	Vector<Sprite*>::iterator itAster, itLaser;
	for (itAster = _asteroids.begin(); itAster != _asteroids.end(); itAster++) {
		auto *asteroid = (Sprite *)*itAster;
		if (!asteroid->isVisible())
			continue;
		for (itLaser = _shipLasers.begin(); itLaser != _shipLasers.end(); itLaser++) {
			auto *shipLaser = (Sprite *)*itLaser;
			if (!shipLaser->isVisible())
				continue;
			if (shipLaser->boundingBox().intersectsRect(asteroid->boundingBox())) {
				shipLaser->setVisible(false);
				asteroid->setVisible(false);
				continue;
			}
		}
		if (_ship->boundingBox().intersectsRect(asteroid->boundingBox())) {
			SimpleAudioEngine::getInstance()->playEffect("explosion_large.wav");
			asteroid->setVisible(false);
			_ship->runAction(Blink::create(1.0, 9));
			_lives--;
		}
	}

	if (_lives <= 0)
	{
		this->stopAllActions();
		_ship->setVisible(false);
		this->endScene(KENDREASONLOSE);
	}
	else if (curTimeMillis >= _gameOverTime) 
	{
	 this->endScene(KENDREASONWIN);
	}

}

void HelloWorld::onAcceleration(Acceleration* acc, Event* unused_event)
{
#define KFILTERINGFACTOR 0.1
#define KRESTACCELX -0.6
#define KSHIPMAXPOINTSPERSEC (winSize.height*0.5)
#define KMAXDIFFX 0.2

	double rollingX;

	// Cocos2DX inverts X and Y accelerometer depending on device orientation
	// in landscape mode right x=-y and y=x !!! (Strange and confusing choice)
	acc->x = acc->y;
	rollingX = (acc->x * KFILTERINGFACTOR) + (rollingX * (1.0 - KFILTERINGFACTOR));
	float accelX = acc->x - rollingX;
	auto winSize = Director::getInstance()->getWinSize();
	float accelDiff = accelX - KRESTACCELX;
	float accelFraction = accelDiff / KMAXDIFFX;
	_shipPointsPerSecY = KSHIPMAXPOINTSPERSEC * accelFraction;
}

float HelloWorld::randomValueBetween(float low, float high) {
	return ((float)CCRANDOM_0_1() * (high - low)) + low;
}

float HelloWorld::getTimeTick() {
	timeval* time = new timeval();
	cocos2d::timezone* z = new cocos2d::timezone();
	gettimeofday(time, z);
	unsigned long millisecs = (time->tv_sec * 1000) + (time->tv_usec / 1000);
	return (float)millisecs;
}

void HelloWorld::setInvisible(CCNode * node) {
	node->setVisible(false);
}

void HelloWorld::onEnter()
{
	Layer::onEnter();

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);

	auto dispatcher = Director::getInstance()->getEventDispatcher();

	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

bool HelloWorld::onTouchBegan(Touch *touch, Event *event)
{
	auto winSize = Director::getInstance()->getWinSize();

	auto *shipLaser = _shipLasers.at(_nextShipLaser++);

	SimpleAudioEngine::getInstance()->playEffect("laser_ship.wav");
	if (_nextShipLaser >= _shipLasers.size())
		_nextShipLaser = 0;
	shipLaser->setPosition(_ship->getPosition()+Point(shipLaser->getContentSize().width / 2, 0));
	shipLaser->setVisible(true);
	shipLaser->stopAllActions();
	shipLaser->runAction(Sequence::create(
		MoveBy::create(0.5, Point(winSize.width, 0)),
		CCCallFuncN::create(this, callfuncN_selector(HelloWorld::setInvisible)),
		NULL  // DO NOT FORGET TO TERMINATE WITH NULL
		));

	return true;
}

void HelloWorld::restartTapped(Object* sender)
{
	Director::getInstance()->replaceScene(TransitionZoomFlipX::create(0.5, HelloWorld::createScene()));
	this->scheduleUpdate();
}

void HelloWorld::endScene(EndReason endReason) {
	if (_gameOver)
		return;
	_gameOver = true;

	auto winSize = Director::getInstance()->getWinSize();
	char message[10] = "You Win";
	if (endReason == KENDREASONLOSE)
		strcpy(message, "You Lose");
	LabelBMFont * label;
	label = LabelBMFont::create(message,"fonts/Arial.fnt");
	label->setScale(0.1);
	label->setPosition(Point(winSize.width / 2, winSize.height*0.6));
	this->addChild(label);

	LabelBMFont * restartLabel;
	restartLabel = LabelBMFont::create("Restart", "fonts/Arial.fnt");
	auto *restartItem = MenuItemLabel::create(restartLabel,CC_CALLBACK_1(HelloWorld::restartTapped,this));
	restartItem->setScale(0.1);
	restartItem->setPosition(Point(winSize.width / 2, winSize.height*0.4));

	Menu *menu = Menu::create(restartItem, NULL);
	menu->setPosition(Point::ZERO);
	this->addChild(menu);

	// clear label and menu
	restartItem->runAction(ScaleTo::create(0.5, 1.0));
	label->runAction(ScaleTo::create(0.5, 1.0));
	// Terminate update callback
	this->unscheduleUpdate();
}