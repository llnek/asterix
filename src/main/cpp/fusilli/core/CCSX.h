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
NS_BEGIN(fusii)

#define HHZ(z) z.height * 0.5
#define HWZ(z) z.width * 0.5

//////////////////////////////////////////////////////////////////////////////
//
namespace ccsx {

  c::Dictionary* ReadXmlAsDict(const stdstr& fpath);
  c::Array* ReadXmlAsList(const stdstr& fpath);

  void ReifyAudioIcons(c::MenuItem*& off, c::MenuItem*& on);
  void ResolveElastic(not_null<ComObj*>, not_null<ComObj*>);

  FArray<GLubyte> ColorRGB(const stdstr& color);

  void SfxPlay(const stdstr& sound);
  void PauseAudio();

  c::MenuItem* ReifyMenuBtn(const stdstr& n);

  c::MenuItem* ReifyMenuBtn(
      const stdstr& n,
      const stdstr& s, const stdstr& d);

  c::Menu* MkBackQuit(not_null<c::MenuItem*> b,
      not_null<c::MenuItem*> q, bool vert = false);

  bool PointInBox(const Box4& box, float x,  float y);
  const c::Color3B White();
  const c::Color3B Black();

  bool CollideN(not_null<c::Node*>, not_null<c::Node*> );
  bool Collide(not_null<ComObj*>, not_null<ComObj*>);

  void SetDevRes(float width, float height,
      ResolutionPolicy pcy = ResolutionPolicy::NO_BORDER);

  bool IsPortrait();

  bool OutOfBound(not_null<ComObj*>, const Box4& B);
  bool OutOfBound(const Box4& src, const Box4& B);

  c::DelayTime* ReifyTimer(not_null<c::Node*>, float millis);
  void UndoTimer(not_null<c::DelayTime*>);
  bool TimerDone(not_null<c::DelayTime*>);

  c::Label* ReifyBmfLabel(const stdstr& font, const stdstr& text);

  c::Label* ReifyBmfLabel(float x, float y,
    const stdstr& font,
    const stdstr& text);

  c::Sprite* ReifySprite(const stdstr& frameName);

  const Box4 BBox4B4(not_null<ComObj*>);
  const Box4 BBox4(not_null<c::Node*>);

  void RunScene(not_null<c::Scene*>, float delay);
  void RunScene(not_null<c::Scene*>);

  c::Size ScaleSize(c::Size z, float scale);

  bool IsTransitioning();

  const c::Size CalcSize(const stdstr& frame);

  const c::Size HalfHW(not_null<c::Node*>);

  const c::Rect BBox(not_null<c::Node*>);

  float GetScaledHeight(not_null<c::Node*>);
  float GetScaledWidth(not_null<c::Node*>);

  float GetHeight(not_null<c::Node*>);
  float GetWidth(not_null<c::Node*>);

  float GetLeft(not_null<c::Node*>);
  float GetRight(not_null<c::Node*>);
  float GetTop(not_null<c::Node*>);
  float GetBottom(not_null<c::Node*>);

  float GetLastLeft(not_null<ComObj*> );
  float GetLastRight(not_null<ComObj*> );
  float GetLastTop(not_null<ComObj*> );
  float GetLastBottom(not_null<ComObj*> );

  const c::Vec2 Center();
  float CenterX();
  float CenterY();

  float ScreenHeight();
  float ScreenWidth();

  const c::Vec2 VBoxMID(const Box4& );
  const c::Rect VisRect();
  const Box4 VisBox();

  const c::Vec2 SCenter();
  const c::Size Screen();

  bool TraceEnclosure(float dt, const Box4& bbox,
      const c::Rect& rect, const c::Vec2& vel,
      c::Vec2& outPos, c::Vec2& outVel);

  /**
   * Get the sprite from the frame cache using
   * its id (e.g. ship.png)
   */
  c::SpriteFrame* GetSpriteFrame(const stdstr& frameid);

  bool HasKeyPad();
  bool HasMouse();
  bool HasTouch();

  void OnKeyPolls();
  void OnKeys();

  void OnMouse();

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

NS_END(fusii)
#endif



