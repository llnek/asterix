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

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(pumpkins)

class GVars;
//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Popup : public c::LayerColor {
protected:
  virtual c::MenuItem* addLabelButton(const sstr&, const CCT_PT&, BTNCB);
  virtual c::MenuItem* addButton(c::MenuItem*, const CCT_PT&);
  virtual c::Menu* addMenu();

  virtual c::MenuItem* addLabelButton(const sstr &text, const sstr &font, const CCT_PT&, BTNCB);
  virtual c::MenuItem* addSpriteButton(const sstr&, const CCT_PT&, BTNCB);
  void resumeGame();
  void restartGame();
  void nextLevel();
  void quitToMainMenu();

  __decl_ptr(c::Menu, _menu)
  __decl_ptr(GVars, ss)

  Popup(GVars*);

public:

};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL PausePopup : public Popup {

  PausePopup(GVars* ss) : Popup(ss) {}
  bool inix(c::Color4B);

public:

  static owner<PausePopup*> create(not_null<GVars*>);

};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL LevelCompletePopup : public Popup {

  LevelCompletePopup(GVars* ss) : Popup(ss) {}
  bool inix(c::Color4B);
public:

  static owner<LevelCompletePopup*> create(not_null<GVars*>);

};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL GameOverPopup : public Popup {

  GameOverPopup(GVars* ss) : Popup(ss) {}
  bool inix(c::Color4B);

public:

  static owner<GameOverPopup*> create(not_null<GVars*>);
};



NS_END


