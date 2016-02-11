﻿#ifndef __TERRAIN_H__
#define __TERRAIN_H__

#define TILE_H_SIZE 6
#define TILE_W_SIZE 8

#include "cocos2d.h"
#include "Block.h"
#include "Player.h"

USING_NS_CC;
using namespace std;


class Terrain : public Sprite {

    Vector<Block *> _blockPool;
	int _blockPoolIndex;

    Vector<Block *> _blocks;
	int _lastBlockHeight;
    int _lastBlockWidth;
	int _minTerrainWidth;

    bool _showGap;
    Size _screenSize;

    int _currentPatternIndex;
    int _currentPatternCnt;
    int _currentWidthIndex;
    int _currentHeightIndex;
    int _currentTypeIndex;

    int _increaseGapInterval;
    float _increaseGapTimer;
    int _gapSize;

    void initTerrain (void);
    void addBlocks(int currentWidth);

    void distributeBlocks();
    void initBlock(Block * block);

    inline int getWidth () {

        int width = 0;
        for (auto block : _blocks) {
            width += block->getWidth();
        }
        return width;
    }


public:

	Terrain(void);
	virtual ~Terrain(void);

	CC_SYNTHESIZE(bool, _startTerrain, StartTerrain);

    static Terrain * create();

	void activateChimneysAt (Player * player);
    void checkCollision (Player * player);

	void move (float xMove);
    void reset (void);

};


#endif // __TERRAIN_H__


