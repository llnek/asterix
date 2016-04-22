#include "PlayLayer.h"


PlayLayer::PlayLayer()
{
}

PlayLayer::~PlayLayer()
{
    CC_SAFE_RELEASE_NULL(box);
}

bool PlayLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	value = CCRANDOM_0_1()*kKindCount + 1;
    box = Box::create(Size(kBoxWidth, kBoxHeight), value);
    CC_SAFE_RETAIN(box);
	box->setLayer(this);
	box->setLock(true);
	box->check();

	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(PlayLayer::onTouchesBegan, this);
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	selectedTile = NULL;

	//
	

	return true;
}

void PlayLayer::onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event)
{
    CCLOG("touched");
    
	auto touch = touches.at(0);
	auto location = touch->getLocation();

	if (location.y < kStartY ||
		location.y >(kStartY + (kTileSize * kBoxHeight)) ||
		location.x < kStartX ||
		location.x >(kStartX + (kTileSize * kBoxWidth)))
	{
		return;
	}

	int x = (location.x - kStartX) / kTileSize;
	int y = (location.y - kStartY) / kTileSize;

	if (selectedTile && selectedTile->getPosX() == x && selectedTile->getPosY() == y) {
		selectedTile = NULL;
		return;
	}

	TileElem *tile = box->objectAtPos(x, y);
	if (tile->getPosX() >= 0 && tile->getPosY() >= 0) {
		if (selectedTile && selectedTile->nearTile(tile)) {
			box->setLock(true);
			this->changeWithTileA(selectedTile, tile);
			
			selectedTile = NULL;
		}
		else {
			if (selectedTile) {
				if (selectedTile->getPosX() == x && selectedTile->getPosY() == y) {
					selectedTile = NULL;
				}
			}
			selectedTile = tile;
		}
	}
}

void PlayLayer::check(Object* pSender, void* data)
{
    CCLOG("PlayLayer::check has been called");
}

void PlayLayer::changeWithTileA(TileElem* a, TileElem* b)
{
    CCLOG("PlayLayer::changeWithTileA has been called");
    Action *actionA = Sequence::create(
		MoveTo::create(kMoveTileTime, b->pixPosition()),
		CallFuncN::create(CC_CALLBACK_1(PlayLayer::check, this, (void*)a)),
		NULL);

	Action *actionB = Sequence::create(
		MoveTo::create(kMoveTileTime, a->pixPosition()),
		CallFuncN::create(CC_CALLBACK_1(PlayLayer::check, this, (void*)b)),
		NULL);

	a->getSprite()->runAction(actionA);
	b->getSprite()->runAction(actionB);

	a->trade(b);

	bool isSolved = box->checkSolution();

	Label *_solvedLabel = Label::createWithTTF("The Puzzle is Solved", "fonts/Marker Felt.ttf", 12);
	_solvedLabel->setPosition(0, 200);
	this->addChild(_solvedLabel);

	if (isSolved) {
		_solvedLabel->setString("The Puzzle is Solved");
	}
	else {
		_solvedLabel->setString("The Puzzle is NOT Solved");
	}
}
