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
#include "Primitives.h"
#include "ComObj.h"
NS_ALIAS(c, cocos2d)
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

  bool Collide0(c::Sprite* , c::Sprite* );
  bool Collide(ComObj* a, ComObj* b);

  void SetDevRes(bool landscape, float x, float y,
      ResolutionPolicy pcy = ResolutionPolicy::NO_BORDER);

  bool IsPortrait();
  bool OutOfBound(ComObj* ent, const Box4& B);
  bool OutOfBound(const Box4& src, const Box4& B);

  c::Action* CreateTimer(c::Node*, float millis);
  void UndoTimer(c::ActionInterval*);
  bool TimerDone(c::ActionInterval*);

  c::Label* CreateBmfLabel(float x, float y,
    const stdstr& fontPath, const stdstr& text);

  c::Sprite* CreateSprite(const stdstr& frameName);

  const Box4 BBox4B4(ComObj* ent);
  const Box4 BBox4(c::Sprite* );

  void RunScene(c::Scene* ns, float delay);
  void RunScene(c::Scene* ns);
  bool IsTransitioning();

  const c::Size CalcSize(const stdstr& frame);

  const c::Size HalfHW(c::Sprite* );
  const c::Rect BBox(c::Sprite* );

  float GetScaledHeight(c::Node* );

  float GetHeight(c::Node*);

  float GetScaledWidth(c::Node* );

  float GetWidth(c::Node* );

  float GetLeft(c::Node* );

  float GetRight(c::Node*);

  float GetTop(c::Node*);

  float GetBottom(c::Node*);

  float GetLastLeft(ComObj* );

  float GetLastRight(ComObj*);

  float GetLastTop(ComObj*);

  float GetLastBottom(ComObj*);

  float CenterX();

  float CenterY();

  const c::Vec2 Center();

  float ScreenHeight();

  float ScreenWidth();

  const c::Rect VisRect();
  const Box4 VisBox();

  const c::Size Screen();
  const c::Vec2 SCenter();

  const c::Vec2 VBoxMID(const Box4& );

  bool TraceEnclosure(float dt, const Box4& bbox,
      const c::Rect& rect, const c::Vec2& vel,
      c::Vec2& outPos, c::Vec2& outVel);

  /**
   * Get the sprite from the frame cache using
   * its id (e.g. #ship).
   */
  c::SpriteFrame* GetSpriteFrame(const stdstr& frameid);

  bool HasKeyPad();

  void OnKeyPolls();

  void OnKeys();

  bool HasMouse();

  void OnMouse();

  bool HasTouch();

  void OnTouchAll();

  void OnTouchOne();

  const c::Vec2 AnchorC();
  const c::Vec2 AnchorTL();
  const c::Vec2 AnchorT();
  const c::Vec2 AnchorTR();
  const c::Vec2 AnchorR();
  const c::Vec2 AnchorL();
  const c::Vec2 AnchorBR();
  const c::Vec2 AnchorB();
  const c::Vec2 AnchorBL();

};

typedef NodeFld<ComObj*> GComObj;


NS_END(fusilli)
#endif



