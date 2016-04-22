#include "Box.h"

Box::Box()
{
}

Box::~Box()
{
}

Box* Box::create(Size aSize, int aImgValue)
{
	Box *pRet = new Box();
	if (pRet && pRet->initWithSize(aSize, aImgValue))
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

bool Box::initWithSize(Size aSize, int aImgValue)
{
	imgValue = aImgValue;
	_size = aSize;
	OutBorderTile = TileElem::create(-1, -1);

	for (int y = 0; y < _size.height; y++) {

		Vector<TileElem *> rowContentVec;
		for (int x = 0; x < _size.width; x++) {
			TileElem *tile = TileElem::create(x, y);
			rowContentVec.pushBack(tile);
			readyToRemoveTilesVec.pushBack(tile);
		}
		contentVec.push_back(rowContentVec);
	}
	return true;
}

TileElem* Box::objectAtPos(int posX, int posY)
{
	if (posX < 0 || posX >= kBoxWidth || posY < 0 || posY >= kBoxHeight) {
		return OutBorderTile;
	}

	return contentVec.at(posY).at(posX);
}

bool Box::check()
{
	int countTile = readyToRemoveTilesVec.size();
	if (0 == countTile){
		return false;
	}

	for (int i = 0; i < countTile; i++) {
		TileElem *tile = readyToRemoveTilesVec.at(i);
		tile->setValue(0);
		if (tile->getSprite()) {
			_layer->removeChild(tile->getSprite());
		}
	}
	readyToRemoveTilesVec.clear();

	char name[20] = { 0 };
	sprintf(name, "%d.png", imgValue);
	Texture2D * texture = Director::getInstance()->getTextureCache()->addImage(name);

	Vector<SpriteFrame *> imgFrames;

	for (int i = 0; i < 7; i++) {
		for (int j = 6; j >= 0; j--) {
			SpriteFrame *imgFrame = SpriteFrame::createWithTexture(texture, Rect(i * 40, j * 40, 40, 40)); 
			imgFrames.pushBack(imgFrame);
		}
	}
	for (int x = 0; x < _size.width; x++) {
		int extension = 0;
		for (int y = 0; y < _size.height; y++) {
			TileElem *tile = TileElem::create(x, y);
			if (tile->getValue() == 0){
				extension++;
			} else if(extension == 0) {
			}
		}
        
		for (int i = 0; i < extension; i++) {
			TileElem *destTile = this->objectAtPos(x, kBoxHeight - extension + i);
			SpriteFrame * img = imgFrames.at(0);
			Sprite *sprite = Sprite::createWithSpriteFrame(img);
			imgFrames.eraseObject(img);
			sprite->setPosition(kStartX + x * kTileSize + kTileSize / 2, kStartY + (kBoxHeight + i) * kTileSize + kTileSize / 2 - kTileSize * extension);
			_layer->addChild(sprite);
			//destTile->setValue(imgValue);
			//destTile->setSprite(sprite);
            
			destTile->setSprite(sprite);
            destTile->setValue((7 * destTile->getPosX()) + destTile->getPosY());
            destTile->setOriginalValue(destTile->getValue());
		}
	}

	return true;
}

bool Box::checkSolution() {
    
    bool isSolved = true;
    
    for (int x=0; x < _size.width; x++) {
        for (int y=0; y < _size.height; y++) {
            TileElem *tile = this->objectAtPos(x, y);
            
            if (tile->getOriginalValue() != tile->getValue()) {
                isSolved = false;
            }
        }
    }
    
    if (isSolved) {
        CCLOG("The Sliding Image is Solved");
        return true;
    } else {
        CCLOG("The Sliding Image is NOT Solved");
        return false;
    }
    
    return true;
}