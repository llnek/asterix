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

#pragma once
//////////////////////////////////////////////////////////////////////////////

#include "core/XConfig.h"
#include "core/COMP.h"
#include "core/CCSX.h"
#include "Unit.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(gmath)

//////////////////////////////////////////////////////////////////////////////
//
owner<Unit*> Unit::friendly() {
  return [[self alloc] initWithFriendlyUnit];
}

//////////////////////////////////////////////////////////////////////////////
//
owner<Unit*> Unit::enemyWith(int num, const c::Vec2 &atGridPosition) {
  return [[self alloc] initWithEnemyWithNumber:num atPos:pos];
}

//////////////////////////////////////////////////////////////////////////////
//
bool Unit::initWithSpriteFrameName(const sstr &fn) {
  auto rc= c::Sprite::initWithSpriteFrameName("imgUnit.png");
  if (!rc) { return false; }
  auto bx= this->getBoundingBox();
  if (1) {//}(UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
    setScale(0.8);
  }
  _lblValue = cx::reifyBmfLabel("dft", "1");
  //lblValue->setScale(1.5);
  _lblValue->setPosition(bx.size.width/2, bx.size.height * 0.55);
  addChild(_lblValue);
  _dragDirection = DirStanding;

  //[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(repositionUnitToGridPos) name:kUnitDragCancel object:nil];

  _gridWidth = bx.size.width + 0.6;
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
owner<Unit*> Unit::initWithFriendlyUnit() {
  auto z= Unit::create();
  z->isFriendly = true;
  z->unitValue = 1;
  z->direction = DirStanding;
  z->setColor(c::Color3B(39/255.0, 179/255.0, 97/255.0)); //green for friendly
  z->gridPos = c::Vec2(5,5);
  return z;;
}

//////////////////////////////////////////////////////////////////////////////
//
owner<Unit*> Unit::initWithEnemyWithNumber(int num, const c::Vec2 &atPos) {
  auto z= Unit::create();
  z->_isFriendly = false;
  z->_unitValue = num;
  z->_lblValue->setString(FTOS(num));
  z->_direction = DirLeft;
  //red for enemy
  z->setColor(c::Color3B(255/255.0, 91/255.0, 75/255.0));
  z->_gridPos = p;

  return z;
}

//////////////////////////////////////////////////////////////////////////////
//
void Unit::updateLabel() {
  _lblValue->setString(FTOS(_unitValue));
}

//////////////////////////////////////////////////////////////////////////////
//
bool Unit::didMoveIncNumber() {
  auto didIncrease = true;
  ++self.unitValue;

  auto gridX = _gridPos.x;
  auto gridY = _gridPos.y;

  //move unit that direction
  if (_direction == DirUp)
    --gridY;
  else if (_direction == DirDown)
    ++gridY;
  else if (_direction == DirLeft)
    --gridX;
  else if (_direction == DirRight)
    ++gridX;

  //keep within the grid bounds
  if (gridX < 1) gridX = 1;
  if (gridX > 9) gridX = 9;

  if (gridY < 1) gridY = 1;
  if (gridY > 9) gridY = 9;

  if (_direction == DirAtWall) {
    _unitValue -= 2;
    didIncrease = false;
  }

  if (_isFriendly) {
    if ((gridX == 1 && _direction == DirLeft) ||
        (gridX == 9 && _direction == DirRight) ||
        (gridY == 1 && _direction == DirUp) ||
        (gridY == 9 && _direction == DirDown))  {
      //if didn't move, it's standing
      self.direction = DirAtWall;
    }
  }

  _gridPos = c::Vec2(gridX, gridY);

  updateLabel();

  return didIncrease;
}

//////////////////////////////////////////////////////////////////////////////
//
void Unit::setDir(int wall) {
  if (wall == 1)
    _direction = DirDown;
  else if (wall == 2)
    _direction = DirLeft;
  else if (wall == 3)
    _direction = DirUp;
  else if (wall == 4)
    _direction = DirRight;
}

//////////////////////////////////////////////////////////////////////////////
//
void Unit::setNewDirForEnemy() {
  auto choice = rand() % 2;

  if (_gridPos.x == 4 && _gridPos.y == 5) {
    _direction = DirRight;
  } else if (_gridPos.x == 5 && _gridPos.y == 4) {
    _direction = DirDown;
  } else if (_gridPos.x == 5 && _gridPos.y == 6) {
    _direction = DirUp;
  } else if (_gridPos.x == 6 && _gridPos.y == 5) {
    _direction = DirLeft;
  //top left corner
  } else if (_gridPos.x == 4 && _gridPos.y == 4) {
    _direction = choice ? DirRight : DirDown;
  }
  //bottom left corner
  else if (_gridPos.x == 4 && _gridPos.y == 6) {
    _direction = choice ? DirRight : DirUp;
  }
  //top right corner
  else if (_gridPos.x == 6 && _gridPos.y == 4) {
    _direction = choice ? DirLeft : DirDown;
  }
  //bottom right corner
  else if (_gridPos.x == 6 && _gridPos.y == 6) {
    _direction = choice ? DirLeft : DirUp;
  }
  //one of the rows
  else if (_gridPos.x == 4 || _gridPos.x == 6) {
    if (_gridPos.y > 5)
      _direction = DirUp;
    else
      _direction = DirDown;
  }
  else if (_gridPos.y == 4 || _gridPos.y == 6) {
    if (_gridPos.x > 5)
      _direction = DirLeft;
    else
      _direction = DirRight;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Unit::slideWithDist(const c::Vec2 &dist, int dir) {

  auto origPos = getPosForGridCoord(_gridPos);
  auto newX = this->getPositionX();
  auto newY = this->getPositionY();

  if (!_isBeingDragged &&
      (((_direction == DirUp || _direction == DirRight) &&
        (dir == DirDown || dir == DirLeft)) ||
       ((_direction == DirDown || _direction == DirLeft) &&
        (dir == DirUp || dir == DirRight)))) {
    dist *= -1;
  }

  if (_dragDirection == DirUp ||
      (!_isBeingDragged && _direction == DirUp)) {
    newY += dist;
    if (newY > origPos.y + _gridWidth) {
      newY = origPos.y + _gridWidth;
    }
    else if (newY < origPos.y) {
      newY = origPos.y;
    }
  }
  else if (_dragDirection == DirDown ||
           (!_isBeingDragged && _direction == DirDown)) {
    newY += dist;
    if (newY < origPos.y - _gridWidth) {
      newY = origPos.y - _gridWidth;
    }
    else if (newY > origPos.y) {
      newY = origPos.y;
    }
  }
  else if (_dragDirection == DirLeft ||
          (!_isBeingDragged && _direction == DirLeft)) {
    newX += dist;
    if (newX < origPos.x - _gridWidth) {
      newX = origPos.x - _gridWidth;
    }
    else if (newX > origPos.x) {
      newX = origPos.x;
    }
  }
  else if (_dragDirection == DirRight ||
          (!_isBeingDragged && _direction == DirRight)) {
    newX += dist;
    if (newX > origPos.x + _gridWidth)
      newX = origPos.x + _gridWidth;
    else if (newX < origPos.x)
      newX = origPos.x;
  }

  setPosition(newX, newY);
}

//////////////////////////////////////////////////////////////////////////////
//
void Unit::repositionUnitToGridPos() {
  auto pos= getPositionForGridCoord(_gridPos);
  setPosition(pos);
}

//////////////////////////////////////////////////////////////////////////////
//
void Unit::touchStart(c::Touch *touch) {
  _tapAt = touch->getLocation();
}

//////////////////////////////////////////////////////////////////////////////
//
void Unit::touchMotion(c::Touch *touch) {

  auto tap = touch->getLocation();

  //if it's not already being dragged and the touch is dragged far enough away...
  if (!_isBeingDragged && c::ccpDistance(tap, _tapAt) > 20) {
    _isBeingDragged = true;
    //add it here:
    _prevTapAt = _tapAt;

    auto diff = c::Vec2(tap.x - _tapAt.x, tap.y - _tapAt.y);
    //determine direction
    if (diff.x > 0) {
      if (diff.x > abs(diff.y)) {
        _dragDirection = DirRight;
      } else if (diff.y > 0) {
        _dragDirection = DirUp;
      } else {
        _dragDirection = DirDown;
      }
    } else {
      if (diff.x < -1* abs(diff.y)) {
        _dragDirection = DirLeft;
      } else if (diff.y > 0) {
        _dragDirection = DirUp;
      } else {
        _dragDirection = DirDown;
      }
    }

    if ((ss->tutorial == 1 && _dragDirection != DirRight) ||
        (ss->tutorial == 2 && (_dragDirection != DirLeft || _unitValue == 1)) ||
        (ss->tutorial == 3 && _dragDirection != DirRight)) {
      _isBeingDragged = false;
      _dragDirection = DirStanding;
    }
  }

  if (_isBeingDragged) {

    float dist = 0;

    if (_dragDirection == DirUp || _dragDirection == DirDown) {
      dist = tap.y - _prevTapAt.y;
    }
    else if (_dragDirection == DirLeft || _dragDirection == DirRight) {
      dist = tap.x - _prevTapAt.x;
    }

    //make sure we don't drag beyond the edges of the board... so set dist to 0 when this happens
    if (_direction == DirAtWall) {
      if ((_gridPos.y == 1 && _dragDirection == DirUp) ||
        (_gridPos.y == 9 && _dragDirection == DirDown) ||
        (_gridPos.x == 1 && _dragDirection == DirLeft) ||
        (_gridPos.x == 9 && _dragDirection == DirRight)) {
        dist = 0;
      }
    }

    dist /= 2; //optional
    slideAllUnitsWithDistance(dist,_dragDirection);
    _prevTapAt= tap;;
  }

}

//////////////////////////////////////////////////////////////////////////////
//
void Unit::touchEnd(c::Touch *touch) {
  if (!_isBeingDragged) { return; }

  //CGPoint touchPos = [touch locationInNode:self.parent];
  //stop the dragging
  _isBeingDragged = false;

  auto oldSelfPos = getPositionForGridCoord(_gridPos);
  auto pos= this->getPosition();
  auto dist = c::ccpDistance(oldSelfPos, pos);

  if (dist > _gridWidth/2) {
    auto gridX = _gridPos.x;
    auto gridY = _gridPos.y;

    //move unit that direction
    if (_dragDirection == DirUp) {
      --gridY;
    }
    else if (_dragDirection == DirDown) {
      ++gridY;
    }
    else if (_dragDirection == DirLeft) {
      --gridX;
    }
    else if (_dragDirection == DirRight) {
      ++gridX;
    }

    //keep within the grid bounds
    if (gridX < 1) { gridX = 1; }
    if (gridX > 9) { gridX = 9; }

    if (gridY < 1) { gridY = 1; }
    if (gridY > 9) { gridY = 9; }

    //if it's not in the same place... aka, a valid move taken
    if (!(gridX == _gridPos.x && gridY == _gridPos.y)) {
      _direction = _dragDirection;
      //[self updateLabel];
      //pass the unit through to the MainScene
      postNotificationName(kTurnCompletedNotification,CC_NIL, this);
    }
  //else... dist NOT > gridWidth/2
  } else {
    //[self repositionUnitToGridPos];
    postNotificationName(kUnitDragCancel,CC_NIL);
  }

  _dragDirection = DirStanding;

}


NS_END


