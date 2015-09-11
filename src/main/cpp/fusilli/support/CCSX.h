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
#include "2d/ComObj.h"
#include "cocos2d.h"
NS_USING(cocos2d)
NS_USING(std)
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

  static bool Collide0(Sprite* spriteA, Sprite* spriteB);
  static bool Collide(ComObj* a, ComObj* b);

  static void SetDevRes(bool landscape, float x, float y,
      ResolutionPolicy pcy = ResolutionPolicy::NO_BORDER);

  static bool IsPortrait();
  static bool OutOfBound(ComObj* ent, const Box4& B);
  static bool OutOfBound(const Box4& src, const Box4& B);

  static Action* CreateTimer(Node*, float millis);
  static void UndoTimer(Action*);
  static bool TimerDone(Action*);

  static Sprite* CreateSprite(const string& frameName);

  static const Box4 BBox4B4(ComObj* ent);
  static const Box4 BBox4(Sprite* sprite);

  static void RunScene(Scene* ns, float delay);
  static bool IsTransitioning();

  static const Size CSize(const string& frame);

  static const Size HalfHW(Sprite* sprite);
  static const Rect BBox(Sprite* sprite);

  static float GetScaledHeight(Sprite* sprite);

  static float GetHeight(Sprite* sprite);

  static float GetScaledWidth(Sprite* sprite);

  static float GetWidth(Sprite* sprite);

  static float GetLeft(Sprite* );

  static float GetRight(Sprite*);

  static float GetTop(Sprite*);

  static float GetBottom(Sprite*);

  static float GetLastLeft(ComObj* ent);

  static float GetLastRight(ComObj*);

  static float GetLastTop(ComObj*);

  static float GetLastBottom(ComObj*);

  static float CenterX();

  static float CenterY();

  static const Vec2 Center();

  static float ScreenHeight();

  static float ScreenWidth();

  static const Rect VisRect();
  static const Box4 VisBox();

  static const Size Screen();
  static const Vec2 SCenter();

  static const Vec2 VBoxMID(const Box4& );

  static bool TraceEnclosure(float dt, const Box4& bbox,
      const Rect& rect, const Vec2& vel,
      Vec2& outPos, Vec2& outVel);

  /**
   * Get the sprite from the frame cache using
   * its id (e.g. #ship).
   */
  static SpriteFrame* GetSpriteFrame(const string& frameid);

  static bool HasKeyPad();

  static void OnKeyPolls();

  static void OnKeys();

  static bool HasMouse();

  static void OnMouse();

  static bool HasTouch();

  static void OnTouchAll();

  static void OnTouchOne();

  static const Vec2 AnchorC();
  static const Vec2 AnchorTL();
  static const Vec2 AnchorT();
  static const Vec2 AnchorTR();
  static const Vec2 AnchorR();
  static const Vec2 AnchorL();
  static const Vec2 AnchorBR();
  static const Vec2 AnchorB();
  static const Vec2 AnchorBL();

};




NS_END(fusilli)
#endif

