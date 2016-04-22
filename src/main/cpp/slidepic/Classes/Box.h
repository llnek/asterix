#pragma once
#include "cocos2d.h"
#include "Constants.h"
#include "Tile.h"
USING_NS_CC;

class Box :
	public Object
{
public:
	Box();
	~Box();

	bool initWithSize(Size aSize, int aImgValue);
	TileElem* objectAtPos(int posX, int posY);
	bool check();
    bool checkSolution();

	static Box* create(Size size, int factor);

	CC_SYNTHESIZE_READONLY(Size, _size, Size);
	CC_SYNTHESIZE(Layer*, _layer, Layer);
	CC_SYNTHESIZE(bool, lock, Lock);
public:
    std::vector<Vector<TileElem *>> contentVec;
	Vector<TileElem *> readyToRemoveTilesVec;
	
	TileElem* OutBorderTile;
	int imgValue;
};

