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

#include "2d/XMenuLayer.h"
#include "2d/XScene.h"
NS_BEGIN(invaders)


//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL MenuLayer : public XMenuLayer {
public:

  virtual void OnInit() override;
  void OnBack(Ref*);
  void OnQuit(Ref*);

  virtual ~MenuLayer();
  MenuLayer();

  CREATE_FUNC(MenuLayer)

private:
  DISALLOW_COPYASSIGN(MenuLayer)
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL MainMenu : public XScene {
public:
  static MainMenu* CreateWithBackAction(FiniteTimeAction* );

  virtual void OnInit() override;

  virtual ~MainMenu();
  MainMenu();

  void OnBackAction();
  void OnQuitAction();

private:
  DISALLOW_COPYASSIGN(MainMenu)
};




NS_END(invaders)
#endif

