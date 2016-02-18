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

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(stoneage)

//////////////////////////////////////////////////////////////////////////////
//
GridAnimations* GridAnimations::create() {
  return new GridAnimations();
}

//////////////////////////////////////////////////////////////////////////////
//
GridAnimations::GridAnimations() {
  animatedGems = 0;
  animatedMatchedGems = 0;
  animatedCollapsedGems = 0;
}

//////////////////////////////////////////////////////////////////////////////
//
void GridAnimations::animateIntro() {
  auto sz=CC_CSV(c::Integer,"GRID_SIZE_X");
  //self.gameLayer.enabled = false
  animatedGems = 0;
  for (auto i=0; i < sz; ++i) {
    dropColumn(i);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GridAnimations::dropColumn(int col) {

  auto sz=CC_CSV(c::Integer,"GRID_SIZE_Y");
  auto delay = 0;

  if (col > 0) { delay = cx::rand() * 1.5; }

  for (auto i=0; i < sz; ++i) {
    auto gem = MGML()->gridGemsColumnMap[col][i];
    gem->show();
    auto finalY = gem->sprite->getPositionY();
    gem->sprite->setPositionY(finalY + 800);
    auto moveTo = c::MoveTo::create(2,
        c::Vec2(gem->sprite->getPositionX(), finalY));
    auto seq1 = c::Sequence::create(
      c::DelayTime::create(delay),
      c::EaseBounceOut::create(moveTo),
      c::CallFunc::create([=](){
        this->animatedGems += 1;
        if (this->animatedGems == GEMS_TOTAL) {
          //self.gameLayer.enabled = true
          MGML()->startTimer();
        }
      }));
    gem->sprite->runAction(seq1);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GridAnimations::animateSelected(Gem *gem) {
  gem->select();
  gem->sprite->stopAllActions();
  gem->sprite->runAction(
      c::EaseBounceOut::create(
        c::RotateBy::create(0.5, 360)));
}

//////////////////////////////////////////////////////////////////////////////
//
void GridAnimations::resetSelectedGem() {
  auto gem = MGML()->selectedGem;
  auto gemPosition = MGML()->selectedGemPosition;

  gem->sprite->runAction(
    c::EaseElasticOut::create(
      c::MoveTo::create(0.25, gemPosition)));
}

//////////////////////////////////////////////////////////////////////////////
//
void GridAnimations::swapGems(
    Gem *gemOrigin, Gem *gemTarget, VOIDFN onComplete) {

  gemOrigin->deselect();

  auto origin = MGML()->selectedGemPosition;
  auto target = c::Vec2(
      gemTarget->sprite->getPositionX(),
      gemTarget->sprite->getPositionY());

  auto moveSelected = c::EaseBackOut::create(
      c::MoveTo::create(0.8, target));
  auto moveTarget = c::EaseBackOut::create(
      c::MoveTo::create(0.8, origin));

  gemOrigin->sprite->runAction(moveSelected);
  gemTarget->sprite->runAction(
      c::Sequence::create(
        moveTarget,
        c::CallFunc::create(onComplete)));
}

//////////////////////////////////////////////////////////////////////////////
//
void GridAnimations::animateMatches(matches, onComplete) {

  auto onCompleteMe= [=]() {
    this->animatedMatchedGems -= 1;
    if (this->animatedMatchedGems == 0) {
      if (onComplete) onComplete();
    }
  };
  this->animatedMatchedGems = #matches;

  //for i, point in ipairs(matches) do
  F__LOOP() {
    gem = MGML()->gridGemsColumnMap[point.x][point.y];
    gem->sprite->stopAllActions();
    auto scale = c::EaseBackOut::create(
        c::ScaleTo::create(0.3, 0));
    auto callback = c::CallFunc::create(onCompleteMe);
    auto action = c::Sequence::create(scale, callback);
    gem->gemContainer->runAction(action);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GridAnimations::animateCollapse( onComplete ) {
  auto gridSpace=CC_CSV(c::Integer, "GRID_SPACE");
  auto tileSize=CC_CSV(c::Integer, "TILE_SIZE");
  auto sx=CC_CSV(c::Integer,"GRID_SIZE_X");
  auto sy=CC_CSV(c::Integer,"GRID_SIZE_Y");
  float newY;
  int drop;
  Gem *gem;
  this->animatedCollapsedGems = 0;
  for (auto i=0;i < sx; ++i) {
    drop = 1;
    for (auto r=0; r < sy; ++r) {
      gem = MGML()->gridGemsColumnMap[c][r];
      //if this gem has been resized, move it to the top
      if (gem->gemContainer->getScaleX() != 1) {
        gem->sprite->setPositionY((sy + drop) * (tileSize + gridSpace));
        this->animatedCollapsedGems += 1;
        gem->setType(MGML()->getNewGem());
        gem->show();
        newY = (sy - drop - 1) * (tileSize + gridSpace);
        this->dropGemTo(gem, newY, 0.2, onComplete);
        drop += 1;
      } else {
        if (drop > 1) {
          this->animatedCollapsedGems += 1;
          newY = gem->sprite->getPositionY() - (drop-1) * (tileSize + gridSpace);
          dropGemTo(gem, newY, 0, onComplete);
        }
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GridAnimations::dropGemTo(
    Gem *gem, float y, float delay, VOIDFN onComplete) {

  gem->sprite->stopAllActions();
  gem->reset();

  auto onCompleteMe= [=]() {
    this->animatedCollapsedGems -= 1;
    if (this->animatedCollapsedGems == 0 ) {
      if (onComplete) onComplete();
    }
  };

  auto move = c::EaseBounceOut::create(
      c::MoveTo::create(0.6,
        c::Vec2(gem->sprite->getPositionX(), y) ) );
  auto action = c::Sequence::create(
      c::DelayTime::create(delay),
      move,
      c::CallFunc::create(onCompleteMe));
  gem->sprite->runAction(action);
}

//////////////////////////////////////////////////////////////////////////////
//
void GridAnimations::collectDiamonds(diamonds) {

  auto removeDiamond= [=](c::Ref* sender) {
    CC_HIDE(sender);
  };
  auto wz=cx::visRect();

  for i = 1, #diamonds do
    auto delay = c::DelayTime::create(i * 0.05);
    auto moveTo = c::EaseBackIn::create(
        c::MoveTo::create(
          0.8, c::Vec2(50, wz.size.height - 50)));
    auto action = c::Sequence::create(
        delay, moveTo, c::CallFunc:create(removeDiamond));
    diamonds[i]->runAction(action);
  }
}


NS_END




