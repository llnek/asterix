#include "Tile.h"
USING_NS_CC;

TileElem::TileElem()
{
}

TileElem::~TileElem()
{
    CC_SAFE_RELEASE_NULL(_sprite);
}

TileElem* TileElem::create(int posX, int posY)
{
	TileElem *pRet = new TileElem();
	if (pRet && pRet->initWithPos(posX, posY))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool TileElem::initWithPos(int posX, int posY)
{
	_x = posX;
	_y = posY;
    
    _value = 0;

    _sprite = Sprite::create();
    CC_SAFE_RETAIN(_sprite);
    
	return true;
}

bool TileElem::nearTile(TileElem *otherTile)
{
	return (_x == otherTile->getPosX() &&
		abs(_y - otherTile->getPosY()) == 1) ||
		(_y == otherTile->getPosY() &&
		abs(_x - otherTile->getPosX()) == 1);
}

void TileElem::trade(TileElem *otherTile)
{
	Sprite *tempSprite = Sprite::createWithTexture(_sprite->getTexture());
	tempSprite->setPosition(_sprite->getPosition());
	int tempValue = _value;
	this->setSprite(otherTile->getSprite());
	this->setValue(otherTile->getValue());
	otherTile->setSprite(tempSprite);
	otherTile->setValue(tempValue);
}

Point TileElem::pixPosition()
{
	return Point(kStartX + _x * kTileSize + kTileSize / 2.0f, kStartY + _y * kTileSize + kTileSize / 2.0f);
}

/*void TileElem::setSprite(Sprite* sprite)
{
if (!_sprite)
{
CC_SAFE_RELEASE_NULL(_sprite);
_sprite = sprite;
CC_SAFE_RETAIN(_sprite);
}
}

Sprite* TileElem::getSprite()
{
return _sprite;
}*/