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

#include "support/Primitives.h"
#include "platform/CCCommon.h"
#include "2d/ComObj.h"
NS_ALIAS(cc, cocos2d)
NS_ALIAS(s, std)
NS_BEGIN(fusilli)


#define HHZ(z) z.height * 0.5
#define HWZ(z) z.width * 0.5

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL CCSX {
private:
  static void ResolveElastic(ComObj* obj1, ComObj* obj2);

public:
  static bool PointInBox(const Box4& box, float x,  float y);
  //static const Color3B White();
  //static const Color3B Black();

  static bool Collide0(cc::Sprite* , cc::Sprite* );
  static bool Collide(ComObj* a, ComObj* b);

  static void SetDevRes(bool landscape, float x, float y,
      ResolutionPolicy pcy = ResolutionPolicy::NO_BORDER);

  static bool IsPortrait();
  static bool OutOfBound(ComObj* ent, const Box4& B);
  static bool OutOfBound(const Box4& src, const Box4& B);

  static cc::Action* CreateTimer(cc::Node*, float millis);
  static void UndoTimer(cc::Action*);
  static bool TimerDone(cc::Action*);

  static cc::Sprite* CreateSprite(const s::string& frameName);

  static const Box4 BBox4B4(ComObj* ent);
  static const Box4 BBox4(cc::Sprite* );

  static void RunScene(cc::Scene* ns, float delay);
  static void RunScene(cc::Scene* ns);
  static bool IsTransitioning();

  static const cc::Size CSize(const s::string& frame);

  static const cc::Size HalfHW(cc::Sprite* );
  static const cc::Rect BBox(cc::Sprite* );

  static float GetScaledHeight(cc::Node* );

  static float GetHeight(cc::Node*);

  static float GetScaledWidth(cc::Node* );

  static float GetWidth(cc::Node* );

  static float GetLeft(cc::Node* );

  static float GetRight(cc::Node*);

  static float GetTop(cc::Node*);

  static float GetBottom(cc::Node*);

  static float GetLastLeft(ComObj* );

  static float GetLastRight(ComObj*);

  static float GetLastTop(ComObj*);

  static float GetLastBottom(ComObj*);

  static float CenterX();

  static float CenterY();

  static const cc::Vec2 Center();

  static float ScreenHeight();

  static float ScreenWidth();

  static const cc::Rect VisRect();
  static const Box4 VisBox();

  static const cc::Size Screen();
  static const cc::Vec2 SCenter();

  static const cc::Vec2 VBoxMID(const Box4& );

  static bool TraceEnclosure(float dt, const Box4& bbox,
      const cc::Rect& rect, const cc::Vec2& vel,
      cc::Vec2& outPos, cc::Vec2& outVel);

  /**
   * Get the sprite from the frame cache using
   * its id (e.g. #ship).
   */
  static cc::SpriteFrame* GetSpriteFrame(const s::string& frameid);

  static bool HasKeyPad();

  static void OnKeyPolls();

  static void OnKeys();

  static bool HasMouse();

  static void OnMouse();

  static bool HasTouch();

  static void OnTouchAll();

  static void OnTouchOne();

  static const cc::Vec2 AnchorC();
  static const cc::Vec2 AnchorTL();
  static const cc::Vec2 AnchorT();
  static const cc::Vec2 AnchorTR();
  static const cc::Vec2 AnchorR();
  static const cc::Vec2 AnchorL();
  static const cc::Vec2 AnchorBR();
  static const cc::Vec2 AnchorB();
  static const cc::Vec2 AnchorBL();

};




NS_END(fusilli)
#endif



