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
#include "Tower.h"
#include "Enemy.h"
#include "Lightning.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(pumpkins)

//////////////////////////////////////////////////////////////////////////////
//
owner<Tower*> Tower::create(int type, const CCT_PT &position) {
  auto z = mc_new(Tower);
  z->inix(type, position);
  z->autorelease();
  return z;
}

//////////////////////////////////////////////////////////////////////////////
//
bool Tower::inix(int type, const CCT_PT &position) {

  if ( !c::Sprite::init()) {
  return false; }

  _type = type;

  setPosition(position);
  setScale(0);
  setProperties();

  // animate the tower's spawning
  if (_base) {
    _base->runAction(
        c::EaseBackOut::create(c::ScaleTo::create(0.2, 1)));
  }

  this->runAction(
      c::EaseBackOut::create(c::ScaleTo::create(0.2, 1)));

  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Tower::setProperties() {
  // tower properties are set from the TowerDataSet & TowerData structs
  setBulletName(ss->tower_data_sets[_type_]->bullet);
  setLightning(ss->tower_data_sets[_type]->lightning);
  setRotating(ss->tower_data_sets[_type]-rotating);
  setSpriteName(ss->tower_data_sets[_type]->tower_data[_curLevel]->sprite);
  setRange(ss->tower_data_sets[_type]->tower_data[_curLevel]->range);
  setPhysicalDamage(ss->tower_data_sets[_type]->tower_data[_curLevel]->physicalDamage);
  setMagicalDamage(ss->tower_data_sets[_type]->tower_data[_curLevel]->magicalDamage);
  setSpeedDamage(ss->tower_data_sets[_type]->tower_data[_curLevel]->speedDamage);
  setSpeedDamageDuration(ss->tower_data_sets[_type]->tower_data[_curLevel]->speedDamageDuration);
  setFireRate(ss->tower_data_sets[_type]->tower_data[_curLevel_]->fireRate);
  setCost(ss->tower_data_sets[_type]->tower_data[_curLevel]->cost);

}

//////////////////////////////////////////////////////////////////////////////
//
void Tower::update() {
  checkForEnemies();
  updateRotation();
}

//////////////////////////////////////////////////////////////////////////////
//
void Tower::updateRotation() {
  // rotation to be updated only for rotating towers and if there is an enemy to target
  if (!_rotating || _target == CC_NIL) {
  return; }

  // update rotation so tower is always facing enemy
  auto pos= getPosition();
  setRotation(180 +
    CC_RADIANS_TO_DEGREES(- c::ccpToAngle(c::ccpSub(pos, _target->getPosition()))));
}

//////////////////////////////////////////////////////////////////////////////
//
void Tower::upgrade() {
  // are there any upgrades left?
  if (_curLevel >= NUM_TOWER_UPGRADES - 1) {
  return; }

  // increment upgrade level and reset tower properties
  ++_curLevel;
  setProperties();
  // debit cash
  updateCash(- _cost);

  // reset the range
  _rangeNode->removeFromParentAndCleanup(true);
  _rangeNode = CC_NIL;
  showRange();
}

//////////////////////////////////////////////////////////////////////////////
//
void Tower::sell() {
  // animate the tower's removal
  if (_base) {
    _base->runAction(
        c::Sequence::create(
          c::EaseBackIn::create(
            c::ScaleTo::create(0.2, 0)),
          c::RemoveSelf::create(true),
          CC_NIL));
  }

  this->runAction(
      c::Sequence::create(
        c::EaseBackIn::create(
          c::ScaleTo::create(0.2, 0)),
        c::RemoveSelf::create(true),
        CC_NIL));

}

//////////////////////////////////////////////////////////////////////////////
//
void Tower::checkForEnemies() {
  // only check the current wave for enemies
  auto curr_wave = getCurrentWave();

  if (curr_wave == CC_NIL) {
  return; }

  // search for a target only when there isn't one already
  auto pos= getPosition();
  if (_target == CC_NIL) {
    // loop through each enemy in the current wave
    for (auto i = 0; i < curr_wave->numEnemies; ++i) {
      auto curr_enemy = curr_wave->enemies[i];
      // save this enemy as a target it if it still alive and if it is within range
      if (!curr_enemy->getHasDied() &&
          c::ccpDistance(pos, curr_enemy->getPosition()) <= (_range + curr_enemy->radius())) {
        setTarget(curr_enemy);
        break;
      }
    }
  }

  // check if a target should still be considered
  if (_target) {
    // a target is still valid if it is alive and if it is within range
    if (_target->getHasDied() ||
        c::ccpDistance(pos, _target->getPosition()) > (_range + _target->radius())) {
      setTarget(CC_NIL);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Tower::setTarget(Enemy *enemy) {
  _target = enemy;

  if (_target) {
    // shoot as soon as you get a target
    shoot(0);
    schedule(schedule_selector(Tower::Shoot), _fireRate);
  } else {
    // stop shooting when you lose a target
    unschedule(schedule_selector(Tower::Shoot));
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Tower::shoot(float dt) {
  // don't do anything for a NULL target
  if (_target == CC_NIL) {
  return; }

  // shoot lightning or a bullet?
  if (_lightning) {
    shootLightning();
  } else {
    shootBullet();
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Tower::shootBullet() {
  auto bullet_move_duration = c::ccpDistance(getPosition(), _target->getPosition()) / TILE_SIZE * BULLET_MOVE_DURATION;

  // damage the enemy
  auto damage_enemy = c::Sequence::create(
      c::DelayTime::create(bullet_move_duration),
      c::CallFuncO::create(_target, callfuncO_selector(Enemy::takeDamage), this),
      CC_NIL);
  _target->runAction(damage_enemy);

  // create the bullet
  auto bullet = cx::reifySprite(bulletName);
  auto k= XCFG()->fit(bullet);
  bullet->setScale(0);
  CC_POS1(bullet, getPosition());
  MGML()->addAtlasItem("game-pics", bullet, E_LAYER_TOWER - 1);

  // animate the bullet
  bullet->runAction(c::ScaleTo::create(0.05, k));

  // move the bullet then remove it
  bullet->runAction(
      c::Sequence::create(
        c::MoveTo::create(bullet_move_duration, _target->getPosition()),
        c::RemoveSelf::create(true),
        CC_NIL));
}

//////////////////////////////////////////////////////////////////////////////
//
void Tower::shootLightning() {
  // damage the enemy
  auto damage_enemy = c::Sequence::create(
      c::DelayTime::create(HTV(LIGHTNING_DURATION) ),
      c::CallFuncO::create(_target, callfuncO_selector(Enemy::takeDamage), this),
      CC_NIL);
  _target->runAction(damage_enemy);

  // create the lightning without animation
  auto lightning = Lightning::create(getPosition(), _target->getPosition(), ccc4f(0.1098f, 0.87059f, 0.92157f, 1.0f), false);
  MGML()->addAtlasItem("game-pics", lightning, E_LAYER_TOWER - 1);

  // animate the lightning
  auto shake = c::Sequence::create(
      c::MoveTo::create(0.01, CCT_PT(3, 0)),
      c::MoveTo::create(0.01, CCT_PT(-3, 0)),
      c::MoveTo::create(0.01, CCT_PT(0, 3)),
      c::MoveTo::create(0.01, CCT_PT(0, -3)),
      CC_NIL);
  lightning->runAction(c::Repeat::create(shake, 5));

  // remove the lightning
  lightning->runAction(
    c::Sequence::create(
      c::DelayTime::create(LIGHTNING_DURATION),
      c::RemoveSelf::create(true),
      CC_NIL));
}

//////////////////////////////////////////////////////////////////////////////
//
void Tower::showRange() {

  if (_rangeNode == CC_NIL) {
    createRangeNode();
  }

  // show the range node then hide it a few seconds later
  auto k= _rangeNode->getScale();
  _rangeNode->setScale(0);
  _rangeNode->runAction(
      c::Sequence::create(
        c::Show::create(),
        c::EaseBackOut::create(
          c::ScaleTo::create(0.2, k)),
        c::DelayTime::create(3),
        c::EaseBackIn::create(
          c::ScaleTo::create(0.2, 0)),
        c::Hide::create(),
        CC_NIL));
}

//////////////////////////////////////////////////////////////////////////////
//
void Tower::createRangeNode() {
  auto num_vertices = 30;
  CCT_PT vertices[30];
  auto theta = 0.0f;

  // generate vertices for the circle
  for (auto i = 0; i < num_vertices; ++i) {
    vertices[i] = CCT_PT(_range * cosf(theta), _range * sinf(theta));
    theta += 2 * M_PI / num_vertices;
  }

  auto sz= CC_CSIZE(this);
  // draw a semi transparent green circle with a border
  _rangeNode = c::DrawNode::create();
  _rangeNode->drawPolygon(vertices, num_vertices, ccc4f(0.0f, 1.0f, 0.0f, 0.15f), 2, ccc4f(0.0f, 1.0f, 0.0f, 0.25f));
  CC_HIDE(_rangeNode);
  CC_POS2(_rangeNode, HWZ(sz), HHZ(sz));
  addChild(_rangeNode);

}

//////////////////////////////////////////////////////////////////////////////
//
void Tower::setIsRotating(bool b) {
  _rotating = b;
  // a base must be added separately for a tower that rotates
  if(_rotating && _base == CC_NIL) {
    _base= cx::reifySprite("TD_tbase.png");
    auto k= XCFG()->fit(_base);
    _base->setPosition(getPosition());
    _base->setScale(0);
    MGML()->addAtlasItem("game-pics", _base, E_LAYER_TOWER-1);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Tower::setSpriteName(const sstr &name) {
  // init this tower's sprite
  //initWithFile(sprite_name);
  initWithSpriteFrameName(name);
}


NS_END



