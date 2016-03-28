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

#define kTurnCompletedNotification "unitDragComplete"
#define kUnitDragging "unitDragging"
#define kUnitDragCancel "unitDragCancelled"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(gmath)

enum UnitDirection {
  DirUp,
  DirDown,
  DirLeft,
  DirRight,
  DirAtWall,
  DirStanding //for when a new one spawns at the center
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Unit : public f::CPixie {

  //9x9 grid, 1,1 is top left, 9,9 is bottom right
  DECL_TD(c::Vec2,_gridPos) 
  DECL_PTR(c::Label,_lblValue)
  DECL_TD(c::Vec2,_prevTapAt)
  DECL_IZ(_unitValue)
  DECL_BF(_isFriendly)
  DECL_IZ(_direction)
  DECL_TD(c::Color3B,_color)
  DECL_BF(_isBeingDragged)
  DECL_TD(c::Vec2,_tapAt)
  DECL_IZ(_dragDirection)
  DECL_IZ(_gridWidth)

public:

  static owner<Unit*> enemyUnitWithNumber(int, const c::Vec2&);
  static owner<Unit*> friendlyUnit();

  void updateLabel();
  bool moveUnitDidIncreaseNumber();
  void setDirectionBasedOnWall(int);
  void setNewDirectionForEnemy();
  void slideUnitWithDistance(float,UnitDirection);

};


NS_END



