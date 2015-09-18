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

#if !defined(__XHUDLAYER_H__)
#define __XHUDLAYER_H__

#include "XLayer.h"
NS_ALIAS(c, cocos2d)
NS_ALIAS(s, std)
NS_BEGIN(fusilli)


//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL XHUDLayer : public XLayer {

protected:

  void AddIcon(c::Node* icon,
      int* z = nullptr, int* tag = nullptr);

  XHUDLayer();

  void RemoveIcon(c::Node*);
  virtual void InitAtlases();
  virtual void InitIcons();
  virtual void InitLabels();
  virtual void InitCtrlBtns();

  virtual void ResetAsNew();
  virtual void Reset();

  bool ReduceLives(int );
  void DisableReplay();
  void EnableReplay();
  void UpdateScore(int num);

  void AddReplayIcon(c::MenuItem*, const c::Vec2& where);
  void AddMenuIcon(c::MenuItem*, const c::Vec2& where);

  c::MenuItem* replayBtn;
  c::Label* scoreLabel;
  XLives* lives;
  int score;

private:

  NO__COPYASSIGN(XHUDLayer)

public:

  virtual const stdstr HudAtlas() = 0;
  virtual XLayer* Realize() ;
  virtual ~XHUDLayer();

  int GetScore() { return score; }

};





NS_END(fusilli)
#endif

