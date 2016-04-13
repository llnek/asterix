// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Kenneth Leung. All rights reserved.

#include "core/XConfig.h"
#include "core/COMP.h"
#include "core/CCSX.h"
#include "Fairytale.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(flappy)

//////////////////////////////////////////////////////////////////////////////
//
void Fairytale::init() {
  // this makes a nice midnight sky
  auto bg = c::LayerGradient::create(
      c::Color4B(15, 15, 25, 255),
      c::Color4B(84, 83, 104, 255));
  parentNode->addItem(bg, E_LAYER_BG);

  createCastle();
  createSilhouette();
  createStars();
}

//////////////////////////////////////////////////////////////////////////////
//
void Fairytale::createCastle() {

  auto sz = cx::getSpriteFrame("dhbase")->getOriginalSize();
  castleSpriteSize= XCFG()->fit(sz);

  // initial position
  auto nextPosition = HWZ(castleSpriteSize);
  auto wz= cx::visSize();
  auto len= CC_ZW(wz) * 1.5;

    // fill up one & a half screen
  while (nextPosition < len) {
    // create castle wall sprite and add it to the parent
    auto sp = cx::reifySprite("dhbase");
    XCFG()->fit(sp);
    CC_POS2(sp, nextPosition, CASTLE_SPRITE_Y);
    parentNode->addAtlasItem("dhtex",sp, E_LAYER_CASTLE);
    // store this sprite...we need to update it
    castleSprites.push_back(sp);
    // the next wall depends on this variable
    nextPosition += CC_ZW(castleSpriteSize);
  }

  // we need this to position the next wall sprite
  lastCastleIndex = castleSprites.size() -1;
}

//////////////////////////////////////////////////////////////////////////////
//
void Fairytale::createSilhouette() {
  auto sz = cx::getSpriteFrame("dhbush")->getOriginalSize();
  silhouetteSpriteSize = XCFG()->fit(sz);

    // initial position
  auto nextPosition = 0;
  auto wz= cx::visSize();
  auto len= CC_ZW(wz) * 1.5;

    // fill up one & a half screen
  while (nextPosition < len) {
    // create silhouette sprite and add it to the parent
    auto sp = cx::reifySprite("dhbush");
    XCFG()->fit(sp);
    CC_POS2(sp, nextPosition, SILHOUETTE_SPRITE_Y);
    parentNode->addAtlasItem("dhtex", sp, E_LAYER_SILHOUETTE);
    // store this sprite...we need to update it
    silhouetteSprites.push_back(sp);
    // the next silhouette depends on this variable
    nextPosition += CC_ZW(silhouetteSpriteSize);
  }

    // we need this to position the next silhouette sprite
  lastSilhouetteIndex = silhouetteSprites.size() -1;
}

//////////////////////////////////////////////////////////////////////////////
//
void Fairytale::createStars() {
    // random number of stars...this night sky always changes
  auto numStars = MAX_STARS + (int)floor(cx::randInt(MAX_STARS));
  auto wb=cx::visBox();

  for (auto i = 0; i < numStars; ++i) {
    auto png= cx::rand() > 0.5 ? "dhstar1" : "dhstar2";
    auto star = cx::reifySprite(png);
    XCFG()->fit(star);
    // either big star or small
    // random position
    CC_POS2(star, cx::randInt(wb.right),
                  cx::randInt(wb.top));
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
    auto &sp = *it;
    CC_POS2(sp, sp->getPositionX() - MAX_SCROLLING_SPEED,
                sp->getPositionY());
    // check if the sprite has gone completely out of the left edge of the screen
    if (sp->getPositionX() < -HWZ(castleSpriteSize)) {
      // reposition it after the last wall sprite
      auto x = castleSprites[lastCastleIndex]->getPositionX() + CC_ZW(castleSpriteSize) - MAX_SCROLLING_SPEED;
      CC_POS2(sp, x, sp->getPositionY());
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
    auto &sp = *it;
    CC_POS2(sp, sp->getPositionX() - MAX_SCROLLING_SPEED*0.3,
                sp->getPositionY());
    // check if the sprite has gone completely out of the left edge of the screen
    if (sp->getPositionX() < -HWZ(silhouetteSpriteSize)) {
        // reposition it after the last silhouette sprite
      auto x = silhouetteSprites[lastSilhouetteIndex]->getPositionX() + CC_ZW(silhouetteSpriteSize) - MAX_SCROLLING_SPEED*0.3;
      CC_POS2(sp, x, sp->getPositionY());
      // this sprite now becomes the new last silhouette
      lastSilhouetteIndex = n;
    }

    ++n;
  }
}



NS_END




