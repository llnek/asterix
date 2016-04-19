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
#include "core/COMP.h"
#include "core/CCSX.h"
#include "Enemy.h"
#include "Tower.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(pumpkins)

//////////////////////////////////////////////////////////////////////////////
//
owner<Enemy*> Enemy::create(not_null<GVars*> ss, int type) {
  auto z = mc_new1(Enemy, ss);
  z->inix(type);
  z->autorelease();
  return z;
}

//////////////////////////////////////////////////////////////////////////////
//
bool Enemy::inix( int type) {

  if(!c::Sprite::init()) {
  return false; }

  _type = type;
  setProps();

  auto anim= CC_ACAC()->getAnimation(_animation);
  auto &fs= anim->getFrames();
  _size= fs.at(0)->getSpriteFrame()->getOriginalSize();
  // hide the enemy till it starts walking
  setVisible(false);
  createHealthBar();

  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Enemy::setProps() {
  // properties are from the EnemyData struct
  setAnimationName(this->ss->enemyData[_type].animation);
  setHealth(this->ss->enemyData[_type].health);
  setArmor(this->ss->enemyData[_type].armor);
  setMagicResistance(this->ss->enemyData[_type].magicResistance);
  setSpeed(this->ss->enemyData[_type].speed);
  setDamage(this->ss->enemyData[_type].damage);
  setReward(this->ss->enemyData[_type].reward);
  _healthLeft = _health;
}

//////////////////////////////////////////////////////////////////////////////
//
float Enemy::radius() {
  return HWZ(_size);
}

//////////////////////////////////////////////////////////////////////////////
//
void Enemy::createHealthBar() {
  auto position = CCT_PT(radius(), radius() * 1.75);
  auto redbar = cx::reifySprite("red_bar.png");
  XCFG()->fit(redbar);
  CC_POS1(redbar, position);
  addChild(redbar);

  auto gb= cx::reifySprite("green_bar.png");
  XCFG()->fit(gb);
  _healthBar = c::ProgressTimer::create(gb);
    _healthBar->setType(c::kCCProgressTimerTypeBar);
  _healthBar->setPercentage( _healthLeft / _health * 100);
  _healthBar->setMidpoint(CCT_PT(0, 1));
  _healthBar->setBarChangeRate(CCT_PT(1, 0));
  CC_POS1(_healthBar, position);
  addChild(_healthBar);
}

//////////////////////////////////////////////////////////////////////////////
//
void Enemy::startWalking() {
  // show the enemy when it starts walking
  setVisible(true);
  // position the enemy at the first walking point
  setPosition(_walkPoints[_currWalkPoint]);
  // calculate duration in terms of time taken to walk a single tile
  auto duration = _speed * ENEMY_MOVE_DURATION *
    c::ccpDistance(_walkPoints[_currWalkPoint + 1],
                   _walkPoints[_currWalkPoint]) / TILE_SIZE;

  // walk to the subsequent walk point
  auto walk = c::MoveTo::create(duration, _walkPoints[_currWalkPoint + 1]);
  auto finwalk= c::CallFunc::create(this, callfunc_selector(Enemy::finishWalking));
  auto walkseq= c::Sequence::create(walk, finwalk,CC_NIL);

  // create a speed action to control the walking speed
  auto walkact= c::Speed::create(walkseq, 1);
  walkact->setTag(ENEMY_MOVE_ACTION_TAG);
  this->runAction(walkact);

  if (getActionByTag(ENEMY_ANIMATE_ACTION_TAG) == CC_NIL) {
    auto anim= c::Animate::create(CC_ACAC()->getAnimation(_animation));
    anim->setTag(ENEMY_ANIMATE_ACTION_TAG);
    this->runAction(anim);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Enemy::finishWalking() {

  if (_hasDied) {
  return; }

  // move to the subsequent walk point
  ++_currWalkPoint;

  if (_currWalkPoint < _numWalkPoints - 1) {
    startWalking();
  } else {
  // enemy has reached the pumpkin
    doDamage();
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Enemy::doDamage() {
  this->stopAllActions();
  enemyAtTheGates(this);
  setVisible(false);
}

//////////////////////////////////////////////////////////////////////////////
//
void Enemy::takeDamage(not_null<c::Node*> object) {

  if (_hasDied) {
  return; }

  // calculate total damage taken by this enemy from a given tower
  auto tower = (Tower*)object.get();
  auto physical_damage = tower->getPhysicalDamage() - _armor;
  auto magical_damage = tower->getMagicalDamage() - _magicResistance;
  auto total_damage = (physical_damage > 0 ? physical_damage : 0) + (magical_damage > 0 ? magical_damage : 0);
  _healthLeft -= total_damage;

  // slow the enemy if not already being slowed &
  // if the tower has speed damage
  if (!_isSlowed && tower->getSpeedDamage() < 1) {
    takeSpeedDamage(tower->getSpeedDamage(), tower->getSpeedDamageDuration());
  }

  if(_healthLeft <= 0) {
    die();
  }

  _healthBar->setPercentage( _healthLeft / _health * 100);
}

//////////////////////////////////////////////////////////////////////////////
//
void Enemy::die() {
  this->stopAllActions();
  _hasDied = true;
  enemyDown(this);
  this->runAction(
      c::Sequence::create(
        c::EaseBackIn::create(c::ScaleTo::create(0.2, 0)),
        c::Hide::create(),
        CC_NIL));
}

//////////////////////////////////////////////////////////////////////////////
//
void Enemy::takeSpeedDamage(float damage, float duration) {
  // reduce the walking speed
  _isSlowed = true;
  auto walkact= (c::Speed*)getActionByTag(ENEMY_MOVE_ACTION_TAG);
  if (walkact) {
    walkact->setSpeed(damage);
    // walking speed must return back to normal after certain duration
    scheduleOnce(schedule_selector(Enemy::resetSpeed), duration);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Enemy::resetSpeed(float dt) {
  // walking speed must return back to normal after certain duration
  _isSlowed = false;
  auto walkact= (c::Speed*)getActionByTag(ENEMY_MOVE_ACTION_TAG);
  if (walkact) {
    walkact->setSpeed(1);
  }
}



NS_END


