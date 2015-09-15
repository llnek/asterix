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
NS_ALIAS(cc, cocos2d)
NS_ALIAS(s, std)
NS_BEGIN(fusilli)


//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL XHUDLayer : public XLayer {

protected:

  void AddIcon(cc::Node* icon,
      int* z = nullptr, int* tag = nullptr);

  XHUDLayer();

  void RemoveIcon(cc::Node*);
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

  void AddReplayIcon(cc::MenuItem*, const cc::Vec2& where);
  void AddMenuIcon(cc::MenuItem*, const cc::Vec2& where);

  cc::MenuItem* replayBtn;
  cc::Label* scoreLabel;
  XLives* lives;
  int score;

private:

  DISALLOW_COPYASSIGN(XHUDLayer)

public:

  virtual const s::string HudAtlas() = 0;
  virtual XLayer* Realize() override;
  virtual ~XHUDLayer();

  int GetScore() { return score; }

};





NS_END(fusilli)
#endif

