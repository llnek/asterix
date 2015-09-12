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
NS_USING(cocos2d)
NS_BEGIN(fusilli)


//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL XHUDLayer : public XLayer {

public:

  virtual XLayer* Realize() override;

  virtual const string Moniker() { return "HUD"; }
  virtual const string HudAtlas() = 0;

  virtual ~XHUDLayer();
  XHUDLayer();

  float GetScore() { return score; }

protected:

  void AddIcon(Node* icon, int z = INT32_MIN, int tag = INT32_MIN);
  void RemoveIcon(Node*);
  virtual void InitAtlases();
  virtual void InitIcons();
  virtual void InitLabels();
  virtual void InitCtrlBtns();

  virtual void ResetAsNew();
  virtual void Reset();

  bool ReduceLives(int );
  void DisableReplay();
  void EnableReplay();
  void UpdateScore(float num);

  void AddReplayIcon(MenuItem*, const Vec2& where);
  void AddMenuIcon(MenuItem*, const Vec2& where);


  MenuItem* replayBtn;
  Label* scoreLabel;
  XLives* lives;
  float score;

private:

  DISALLOW_COPYASSIGN(XHUDLayer)

};





NS_END(fusilli)
#endif

