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

#define TILE_SIZE 90

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(pumpkins)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL TowerMenu : public c::Node {

  __decl_ptr(c::Node, _maintenanceNode)
  __decl_ptr(c::Node, _placementNode)

  c::MenuItem* createButton(const sstr&, const CCT_PT&, int tag, VOIDFN);
  c::Label* createLabel(int content,
      const sstr&, float size, const CCT_PT&);

  __decl_vec(c::MenuItem*, _placementButtons)
  __decl_ptr(c::MenuItem, _upgradeButton)
  __decl_ptr(c::Label, _upgradeLabel)
  __decl_ptr(c::MenuItem, _sellButton)
  __decl_ptr(c::Label, _sellLabel)

  void createMaintenanceMenu();
  void createPlacementMenu();
  virtual bool init();

public:

  static owner<TowerMenu*> create(GVars*);

  void showPlacementMenu(const CCT_PT&);
  void hidePlacementMenu();
  void enablePlacementButtons();

  void showMaintenanceMenu(const CCT_PT&, int tower_index, int tower_type, int tower_level);
  void hideMaintenanceMenu();
  void enableMaintenanceButtons(int tower_type, int tower_level);

};


NS_END


