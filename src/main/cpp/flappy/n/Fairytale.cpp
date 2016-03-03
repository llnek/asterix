// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Ken Leung. All rights reserved.

#include "core/XConfig.h"
#include "core/ComObj.h"
#include "core/CCSX.h"
#include "Fairytale.h"


NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(flappy)

//////////////////////////////////////////////////////////////////////////////
//
void Fairytale::init() {
  // this makes a nice midnight sky
  auto background = c::LayerGradient::create(
      c::Color4B(15, 15, 25, 255), c::Color4B(84, 83, 104, 255));
  parentNode->addItem(background, E_LAYER_BG);

  createCastle();
  createSilhouette();
  createStars();
}

//////////////////////////////////////////////////////////////////////////////
//
void Fairytale::createCastle() {
    // record size of the castle wall sprite
  castleSpriteSize = cx::getSpriteFrame("dhbase")->getOriginalSize();
    // initial position
  auto nextPosition = HWZ(castleSpriteSize);
  auto wz= cx::visRect();
  auto len= wz.size.width * 1.5;
    // fill up one & a half screen
  while (nextPosition < len) {
    // create castle wall sprite and add it to the parent's batch node
    auto castleSprite = cx::reifySprite("dhbase");
    castleSprite->setPosition(nextPosition, CASTLE_SPRITE_Y);
    parentNode->addAtlasItem("dhtex",castleSprite, E_LAYER_CASTLE);
    // store this sprite...we need to update it
    castleSprites.push_back(castleSprite);
    // the next wall depends on this variable
    nextPosition += castleSpriteSize.width;
  }
  // we need this to position the next wall sprite
  lastCastleIndex = castleSprites.size() -1;
}

//////////////////////////////////////////////////////////////////////////////
//
void Fairytale::createSilhouette() {
    // record size of the silhouette sprite
  silhouetteSpriteSize = cx::getSpriteFrame("dhbush")->getOriginalSize();
    // initial position
  auto nextPosition = 0;
  auto wz= cx::visRect();
  auto len= wz.size.width * 1.5;
    // fill up one & a half screen
  while (nextPosition < len) {
    // create silhouette sprite and add it to the parent's batch node
    auto silhouetteSprite = cx::reifySprite("dhbush");
    silhouetteSprite->setPosition(nextPosition, SILHOUETTE_SPRITE_Y);
    parentNode->addAtlasItem("dhtex", silhouetteSprite, E_LAYER_SILHOUETTE);
    // store this sprite...we need to update it
    silhouetteSprites.push_back(silhouetteSprite);
    // the next silhouette depends on this variable
    nextPosition += silhouetteSpriteSize.width;
  }
    // we need this to position the next silhouette sprite
  lastSilhouetteIndex = silhouetteSprites.size() -1;
}

//////////////////////////////////////////////////////////////////////////////
//
void Fairytale::createStars() {
    // random number of stars...this night sky always changes
  auto numStars = MAX_STARS + floor(cx::rand() * MAX_STARS);
    auto wz=cx::visRect();
  for (auto i = 0; i < numStars; ++i) {
    // either big star or small
    auto png= (cx::rand() > 0.5) ? "dhstar1" : "dhstar2";
    auto star = cx::reifySprite(png);
    // random position
    star->setPosition(cx::rand() * wz.size.width, cx::rand() * wz.size.height);
    // twinkle twinkle randomly star
    auto duration = 1 + cx::rand() * 2;
    auto action = c::RepeatForever::create(
        c::Sequence::create(
          c::DelayTime::create(duration*2),
          c::FadeOut::create(duration),
          c::FadeIn::create(duration),
          CC_NIL));
    star->runAction(action);
    parentNode->addAtlasItem("dhtex", star);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Fairytale::update(float dt) {
  updateCastle();
  updateSilhouette();
}

//////////////////////////////////////////////////////////////////////////////
//
void Fairytale::updateCastle() {
  auto n=0;
  F__LOOP(it,castleSprites) {
    // first update the position based on the scroll speed
    auto castleSprite = *it;
    castleSprite->setPosition(
        castleSprite->getPositionX() - MAX_SCROLLING_SPEED, castleSprite->getPositionY());

    // check if the sprite has gone completely out of the left edge of the screen
    if (castleSprite->getPositionX() < (castleSpriteSize.width * -0.5)) {
      // reposition it after the last wall sprite
      auto positionX = castleSprites[lastCastleIndex]->getPositionX() + castleSpriteSize.width - MAX_SCROLLING_SPEED;
      castleSprite->setPosition(positionX, castleSprite->getPositionY());
      // this sprite now becomes the new last wall
      lastCastleIndex = n;
    }
    ++n;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Fairytale::updateSilhouette() {
  auto n=0;
  F__LOOP(it,silhouetteSprites) {
      // first update the position based on the scroll speed
    auto silhouetteSprite = *it;
    silhouetteSprite->setPosition(silhouetteSprite->getPositionX() - MAX_SCROLLING_SPEED*0.3, silhouetteSprite->getPositionY());

    // check if the sprite has gone completely out of the left edge of the screen
    if (silhouetteSprite->getPositionX() < (silhouetteSpriteSize.width * -0.5)) {
        // reposition it after the last silhouette sprite
      auto positionX = silhouetteSprites[lastSilhouetteIndex]->getPositionX() + silhouetteSpriteSize.width - MAX_SCROLLING_SPEED*0.3;
      silhouetteSprite->setPosition(positionX, silhouetteSprite->getPositionY());
      // this sprite now becomes the new last silhouette
      lastSilhouetteIndex = n;
    }

    ++n;
  }
}



NS_END




