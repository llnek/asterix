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

#if !defined(__CCSX_H__)
#define __CCSX_H__

#include "platform/CCCommon.h"
#include "base/ccTypes.h"
#include "support/Primitives.h"
#include "2d/ComObj.h"

NS_ALIAS(cc, cocos2d)
NS_ALIAS(s, std)
NS_BEGIN(fusilli)


#define HHZ(z) z.height * 0.5
#define HWZ(z) z.width * 0.5

//////////////////////////////////////////////////////////////////////////////
//
namespace ccsx {

  void ResolveElastic(ComObj* obj1, ComObj* obj2);

  void CreateAudioIcons(c::MenuItem*& off, c::MenuItem*& on);

  c::MenuItem* CreateMenuBtn(const stdstr& n);

  c::MenuItem* CreateMenuBtn(
      const stdstr& n,
      const stdstr& s, const stdstr& d);

  c::Menu* MkBackQuit(c::MenuItem* b,
      c::MenuItem* q, bool vert = false);

  void MkAudio();

  bool PointInBox(const Box4& box, float x,  float y);
  const c::Color3B White();
  const c::Color3B Black();

  bool Collide0(cc::Sprite* , cc::Sprite* );
  bool Collide(ComObj* a, ComObj* b);

  void SetDevRes(bool landscape, float x, float y,
      ResolutionPolicy pcy = ResolutionPolicy::NO_BORDER);

  bool IsPortrait();
  bool OutOfBound(ComObj* ent, const Box4& B);
  bool OutOfBound(const Box4& src, const Box4& B);

  cc::Action* CreateTimer(cc::Node*, float millis);
  void UndoTimer(cc::ActionInterval*);
  bool TimerDone(cc::ActionInterval*);

  c::Label* CreateBmfLabel(float x, float y,
    const stdstr& fontPath, const stdstr& text);

  cc::Sprite* CreateSprite(const stdstr& frameName);

  const Box4 BBox4B4(ComObj* ent);
  const Box4 BBox4(cc::Sprite* );

  void RunScene(cc::Scene* ns, float delay);
  void RunScene(cc::Scene* ns);
  bool IsTransitioning();

  const cc::Size CalcSize(const stdstr& frame);

  const cc::Size HalfHW(cc::Sprite* );
  const cc::Rect BBox(cc::Sprite* );

  float GetScaledHeight(cc::Node* );

  float GetHeight(cc::Node*);

  float GetScaledWidth(cc::Node* );

  float GetWidth(cc::Node* );

  float GetLeft(cc::Node* );

  float GetRight(cc::Node*);

  float GetTop(cc::Node*);

  float GetBottom(cc::Node*);

  float GetLastLeft(ComObj* );

  float GetLastRight(ComObj*);

  float GetLastTop(ComObj*);

  float GetLastBottom(ComObj*);

  float CenterX();

  float CenterY();

  const cc::Vec2 Center();

  float ScreenHeight();

  float ScreenWidth();

  const cc::Rect VisRect();
  const Box4 VisBox();

  const cc::Size Screen();
  const cc::Vec2 SCenter();

  const cc::Vec2 VBoxMID(const Box4& );

  bool TraceEnclosure(float dt, const Box4& bbox,
      const cc::Rect& rect, const cc::Vec2& vel,
      cc::Vec2& outPos, cc::Vec2& outVel);

  /**
   * Get the sprite from the frame cache using
   * its id (e.g. #ship).
   */
  cc::SpriteFrame* GetSpriteFrame(const stdstr& frameid);

  bool HasKeyPad();

  void OnKeyPolls();

  void OnKeys();

  bool HasMouse();

  void OnMouse();

  bool HasTouch();

  void OnTouchAll();

  void OnTouchOne();

  const cc::Vec2 AnchorC();
  const cc::Vec2 AnchorTL();
  const cc::Vec2 AnchorT();
  const cc::Vec2 AnchorTR();
  const cc::Vec2 AnchorR();
  const cc::Vec2 AnchorL();
  const cc::Vec2 AnchorBR();
  const cc::Vec2 AnchorB();
  const cc::Vec2 AnchorBL();

};

typedef NodeFld<ComObj*> GComObj;


NS_END(fusilli)
#endif



