#pragma once
#include "cocos2d.h"
#include "Constants.h"


class TileElem :
	public cocos2d::Object
{
public:
	TileElem();
	~TileElem();

	bool initWithPos(int posX, int posY);
	static TileElem* create(int posX, int posY);
	bool nearTile(TileElem *otherTile);
	void trade(TileElem *otherTile);
	cocos2d::Point pixPosition();

	CC_SYNTHESIZE_READONLY(int, _x, PosX);
	CC_SYNTHESIZE_READONLY(int, _y, PosY);
	CC_SYNTHESIZE_RETAIN(cocos2d::Sprite*, _sprite, Sprite);
	CC_SYNTHESIZE(int, _value, Value);
    CC_SYNTHESIZE(int, _originalValue, OriginalValue);
};

