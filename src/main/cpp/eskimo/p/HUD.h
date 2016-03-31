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

#include "x2d/XLayer.h"
#include "n/lib.h"
NS_BEGIN(eskimo)

//////////////////////////////////////////////////////////////////////////
//
class CC_DLL HUDLayer : public f::XLayer {

  s_vec<c::Sprite*> _buttons;
  void createBtns();

  __decl_ptr(c::Label, _tutorialLabel)
  __decl_ptr(c::Label, _scoreLabel)
  __decl_ptr(c::Label, _messages)

  __decl_ptr(c::Sprite, _btnStart)
  __decl_ptr(c::Sprite, _btnReset)
  __decl_ptr(c::Sprite, _btnPause)
  __decl_ptr(c::Sprite, _btnAgain)
  __decl_ptr(c::Sprite, _btnMenu)
  __decl_iz(_score)
  __decl_ptr(GVars,ss)

public:

  bool touchStart(c::Touch *touch);
  void touchEnd(c::Touch *touch);

  void toggleTutorial(bool);
  void toggleBtn(int , bool );

  void showTut(const sstr&);
  void showMsg(const sstr&);
  void showMenu();

  void setG(not_null<GVars*> g) { ss= g.get(); }

  STATIC_REIFY_LAYER(HUDLayer)
  __decl_deco_ui()
  __decl_get_iid(3)


};


NS_END



