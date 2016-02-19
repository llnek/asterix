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

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/ComObj.h"
#include "core/CCSX.h"
#include "Gem.h"
#include "lib.h"
#include "GridAnimations.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(stoneage)

static int GEMS_TOTAL= GRID_SIZE_X * GRID_SIZE_Y;
static int TILE_GRID= TILE_SIZE+GRID_SPACE;

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
        SENDMSG("/game/start/timer");
    }
  };

  float delay = col>0 ? cx::rand() * 1.5f : 0;
  auto m= ss->gridGemsColumnMap[col];
  Gem *gem;
  for (auto i=0; i < m->size(); ++i) {
    gem = m->get(i);
    gem->show();
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
    c::EaseBounceOut::create(c::RotateBy::create(0.5, 360)));
}

//////////////////////////////////////////////////////////////////////////////
//
void GridAnimations::resetSelectedGem () {
  auto gemPosition = ss->selectedGemPosition;
  auto gem = ss->selectedGem;
  gem->node->runAction(
      c::EaseElasticOut::create(
        c::MoveTo::create(0.25, gemPosition)));
}

//////////////////////////////////////////////////////////////////////////////
//
void GridAnimations::swapGems(Gem *gemOrigin, Gem *gemTarget, VOIDFN onComplete) {

  auto origin = ss->selectedGemPosition;
  auto target = gemTarget->pos();

  auto moveSelected = c::EaseBackOut::create(
      c::MoveTo::create(0.8f, target));
  auto moveTarget = c::EaseBackOut::create(
      c::MoveTo::create(0.8f, origin) );

  gemOrigin->deselect();
  gemOrigin->node->runAction(moveSelected);
  gemTarget->node->runAction(
      c::Sequence::create(
        moveTarget,
        c::CallFunc::create(onComplete),
        CC_NIL));
}

//////////////////////////////////////////////////////////////////////////////
//
void GridAnimations::animateMatches(
    const s_vec<f::Cell2I> &matches, VOIDFN onComplete) {

  this->animatedMatchedGems = matches.size();

  auto onCompleteMe=[=]() {
    this->animatedMatchedGems -= 1;
    if (this->animatedMatchedGems == 0) {
      onComplete();
    }
  };

  F__LOOP(it,matches) {
    auto &pt= *it;
      auto gem = ss->gridGemsColumnMap[pt.x]->get(pt.y);
    gem->node->stopAllActions();
    gem->node->runAction(
        c::Sequence::create(
          c::EaseBackOut::create(c::ScaleTo::create(0.3, 0)),
          c::CallFunc::create(onCompleteMe),
          CC_NIL));
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GridAnimations::animateCollapse(VOIDFN onComplete) {

  this->animatedCollapsedGems = 0;
  for (auto c = 0; c < GRID_SIZE_X; ++c) {
    auto cm = ss->gridGemsColumnMap[c];
    int drop = 1;
    for (auto r = 0; r < cm->size(); ++r) {
        auto gem = cm->get(r);
      //if this gem has been resized, move it to the top
      if (gem->node->getScaleX() != 1) {
        gem->node->setPositionY( (GRID_SIZE_Y+drop) * TILE_GRID);
        this->animatedCollapsedGems += 1;
        gem->setType(getNewGem());
        CC_SHOW(gem->node);
        auto newY = (GRID_SIZE_Y - drop - 1) * TILE_GRID;
        this->dropGemTo(gem, newY,  0.2f, onComplete);
        drop += 1;
      } else {
        if (drop > 1) {
          this->animatedCollapsedGems += 1;
          auto newY = gem->node->getPositionY() - (drop-1) * TILE_GRID;
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

  auto onCompleteMe=[=]() {
    this->animatedCollapsedGems -= 1;
    if (this->animatedCollapsedGems == 0) {
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
        c::CallFuncN::create(removeDiamond),
        CC_NIL);
    d->node->runAction(action);
  }
}


NS_END



