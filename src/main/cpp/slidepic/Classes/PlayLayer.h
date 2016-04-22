#pragma once
#include "cocos2d.h"
#include "Box.h"
#include "Constants.h"
USING_NS_CC;

class PlayLayer :
	public Layer
{
public:
	PlayLayer();
	~PlayLayer();

	bool init();
	void check(Object* pSender, void* data);
	void changeWithTileA(TileElem* a, TileElem* b);
	void onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event);

	CREATE_FUNC(PlayLayer);

public:
	Box* box;
	TileElem* selectedTile;
	int value;	
};

