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
#include "lib.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(stoneage)

//////////////////////////////////////////////////////////////////////////////
//
GridAnimations* GridAnimations::create(GVars *ss) {
  return mc_new1(GridAnimations,ss);
}

//////////////////////////////////////////////////////////////////////////////
//
void GridAnimations::animateIntro() {
  this->animatedGems = 0;
  ss->enabled = false;
  for (auto i = 0; i < GRID_SIZE_X; ++i) {
    dropColumn(i);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GridAnimations::dropColumn(int col) {

  auto onAnimatedColumn= [=]() {
    this->animatedGems += 1;
    if (this->animatedGems == GEMS_TOTAL) {
      this->ss->enabled = true;
      startTimer(this->ss);
    }
  };

  float delay = col>0 ? cx::rand() * 1.5f : 0;
  auto m= ss->gridGemsColumnMap[col];
  Gem *gem;
  for (auto i=0; i < gcm.size(); ++i) {
    gem = m[i];
    CC_SHOW(gem->node);
    auto finalY = gem->node->getPositionY();
    gem->node->setPositionY(finalY + 800);
    gem->node->runAction(
      c::Sequence::create(
        c::DelayTime::create(delay),
        c::EaseBounceOut::create(
          c::MoveTo::create(
            2,
            c::Vec2(gem->node->getPositionX(), finalY))),
        c::CallFunc::create(onAnimatedColumn),
        CC_NIL));
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GridAnimations::animateSelected(Gem *gem) {
  gem->select();
  gem->node->stopAllActions();
  gem->node->runAction(
    c::EaseBounceOut::create (c::RotateBy::create(0.5, 360)));
}

//////////////////////////////////////////////////////////////////////////////
//
void GridAnimations::resetSelectedGem () {
  local gemPosition = ss->selectedGemPosition;
  local gem = ss->selectedGem;
  gem->node->runAction(
      c::EaseElasticOut::create(
        c::MoveTo::create(0.25, gemPosition)));
}

//////////////////////////////////////////////////////////////////////////////
//
void GridAnimations::swapGems(Gem *gemOrigin, Gem *gemTarget, VOIDFN onComplete) {
  gemOrigin->deselect();

  auto origin = ss->selectedGemPosition;
  auto target = c::Vec2(
      gemTarget->node->getPositionX(),
      gemTarget->node->getPositionY());

  auto moveSelected = c::EaseBackOut::create(
      c::MoveTo::create(0.8f, target));
  auto moveTarget = c::EaseBackOut::create(
      c::MoveTo::create(0.8f, origin) );
  auto callback = c::CallFunc::create(onComplete);

  gemOrigin->node->runAction(moveSelected);
  gemTarget->node->runAction(
      c::Sequence::create(moveTarget, callback));
}

//////////////////////////////////////////////////////////////////////////////
//
void GridAnimations::animateMatches(
    const s_vec<f::Cell2P> &matches, onComplete) {

  auto onCompleteMe=[=]() {
    this->animatedMatchedGems -= 1;
    if (this->animatedMatchedGems == 0) {
      if (onComplete) onComplete();
    }
  };
  this->animatedMatchedGems = matches.size();

  F__LOOP(it,matches) {
    auto &pt= *it;
    auto gem = ss->gridGemsColumnMap[pt.x][pt.y];
    gem->node->stopAllActions();
    auto scale = c::EaseBackOut::create(c::ScaleTo::create(0.3, 0));
    auto callback = c::CallFunc::create(onCompleteMe);
    auto action = c::Sequence::create(scale, callback);
    gem->node->runAction(action);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GridAnimations::animateCollapse(onComplete) {
  this->animatedCollapsedGems = 0;
  local gem = nil
  local drop  = 1

  for (auto c = 0; c < GRID_SIZE_X; ++c) {
    auto cm = ss->gridGemsColumnMap[c];
    drop = 1;
    for (auto r = 0; r < cm->size(); ++r) {
      auto gem = cm[r];
      //if this gem has been resized, move it to the top
      if (gem->node->getScaleX() != 1) {
        gem->node->setPositionY(
            (GRID_SIZE_Y+drop) * (TILE_SIZE+GRID_SPACE));
        this->animatedCollapsedGems += 1;
        gem->setType(getNewGem());
        CC_SHOW(gem->node);
        auto newY = (GRID_SIZE_Y - drop - 1) * (TILE_SIZE + GRID_SPACE);
        this->dropGemTo(gem, newY,  0.2f, onComplete);
        drop += 1;
      } else {
        if (drop > 1) {
          this->animatedCollapsedGems += 1;
          auto newY = gem->node->getPositionY() - (drop-1) * (TILE_SIZE + GRID_SPACE);
          this->dropGemTo(gem, newY, 0, onComplete);
        }
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GridAnimations::dropGemTo(
    Gem *gem, float y, float delay, VOIDFN onComplete)  {

  gem->node->stopAllActions();
  gem->reset();

  auto nCompleteMe=[=]() {
    this->animatedCollapsedGems -= 1;
    if (this->animatedCollapsedGems == 0 &&
        onComplete != CC_NIL) {
      onComplete();
    }
  };

  auto move = c::EaseBounceOut::create(
      c::MoveTo::create(0.6f, c::Vec2(gem->node->getPositionX(), y)));
  auto action = c::Sequence::create(
      c::DelayTime::create(delay),
      move,
      c::CallFunc::create(onCompleteMe),
      CC_NIL);
  gem->node->runAction(action);
}

//////////////////////////////////////////////////////////////////////////////
//
void GridAnimations::collectDiamonds(const s_vec<Gem*> &diamonds) {

  auto removeDiamond=[=](c::Node *n) {
    CC_HIDE(n);
  };
  auto wb=cx::visBox();
  int i=1;

  F__LOOP(it,diamonds) {
    auto d= *it;
    auto delay = c::DelayTime::create(i * 0.05f);
    auto moveTo = c::EaseBackIn::create(
        c::MoveTo::create(
          0.8f, c::Vec2(50, wb.top - 50)));
    auto action = c::Sequence::create(
        delay,
        moveTo,
        cc.CallFunc:create(removeDiamond),
        CC_NIL);
    d->node->runAction(action);
  }
}


NS_END



