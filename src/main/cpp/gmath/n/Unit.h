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
class GVars;
//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Unit : public f::CPixie {

  //9x9 grid, 1,1 is top left, 9,9 is bottom right
  DECL_PTR(c::Label,_lblValue)
  DECL_PTR(GVars,ss)
  DECL_TD(c::Vec2,_prevTapAt)
  DECL_TD(c::Color3B,_color)
  DECL_BF(_isBeingDragged)
  DECL_TD(c::Vec2,_tapAt)
  DECL_IZ(_dragDirection)
  virtual bool initWithSpriteFrameName(const sstr &);
  void reposToGrid(c::Ref*);

public:

  DECL_BF(_isFriendly)
  DECL_IZ(_direction)
  DECL_IZ(_unitValue)
  DECL_IZ(_gridWidth)
  DECL_TD(f::Cell2I,_gridPos)
  static owner<Unit*> enemyWith(GVars*, int, const f::Cell2I&);
  static owner<Unit*> friendly(GVars*);

  bool touchStart(const c::Vec2&);
  void touchMotion(const c::Vec2&);
  void touchEnd(const c::Vec2&);

  void updateLabel();
  bool didMoveIncNumber();
  void setDir(int);
  void setNewDirForEnemy();
  void slideWithDist(float,int dir);

};


NS_END



