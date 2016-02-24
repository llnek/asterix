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

  DECL_PTR(c::Label, _tutorialLabel)
  DECL_PTR(c::Label, scoreLabel)
  DECL_PTR(c::Label, _messages)

  DECL_PTR(c::Sprite, _btnStart)
  DECL_PTR(c::Sprite, _btnReset)
  DECL_PTR(c::Sprite, _btnPause)
  DECL_PTR(c::Sprite, _btnAgain)
  DECL_PTR(c::Sprite, _btnMenu)
  DECL_IZ(score)
  DECL_PTR(GVars,ss)

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
  MDECL_DECORATE()
  MDECL_GET_IID(3)


};


NS_END



