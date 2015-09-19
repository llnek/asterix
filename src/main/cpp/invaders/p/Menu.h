// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2015, Ken Leung. All rights reserved.

#if !defined(__MENU_H__)
#define __MENU_H__

#include "2d/CCActionInstant.h"
#include "2d/XMenuLayer.h"
#include "2d/XScene.h"
NS_ALIAS(f,fusilli)
NS_BEGIN(invaders)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL MainMenu : public f::XScene {
friend class MainMenuLayer;
private:
  NO__COPYASSIGN(MainMenu)
  MainMenu();

  void OnBackAction();
  void OnQuitAction();

  c::CallFunc* backAction;

public:

  static MainMenu* CreateWithBackAction(c::CallFunc* );
  virtual f::XScene* Realize();
  virtual void OnInit();

  virtual ~MainMenu();

};




NS_END(invaders)
#endif

