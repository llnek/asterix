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
#include "ash/Component.h"
#include "cocos2d.h"
NS_USING(cocos2d)
NS_USING(std)
NS_BEGIN(fusilli)


//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
T* DictValue(Dictionary* d, const string& key, T*& dummy) {
  auto v= d->objectForKey(key);
  if (NNP(v)) {
    return static_cast<T*>(v);
  } else {
    return nullptr;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL CCSX {

  bool PointInBox(const Box4& box, float x,  float y);
  const Color3B White();
  const Color3B Black();

  bool Collide0(Sprite* spriteA, Sprite* spriteB);
  bool Collide(Entity* a, Entity* b);

  void SetDevRes(bool landscape, float x, float y, ResolutionPolicy pcy);

  bool IsPortrait();
  bool OutOfBound(Entity* ent, const Box4& B);
  bool OutOfBound(const Box4& src, const Box4& B);

  Action* CreateTimer(Node*, float millis);
  void UndoTimer(Action*);
  bool TimerDone(Action*);

  Sprite* CreateSprite(const string& frameName);

  const Box4 BBox4B4(Entity* ent);
  const Box4 BBox4(Sprite* sprite);

  void RunScene(Scene* ns, float delay);
  bool IsTransitioning();

  const Size CSize(const string& frame);

  const Size HalfHW(Sprite* sprite);
  const Rect BBox(Sprite* sprite);

  float GetScaledHeight(Sprite* sprite);

  float GetHeight(Sprite* sprite);

  float GetScaledWidth(Sprite* sprite);

  float GetWidth(Sprite* sprite);

  float GetLeft(Sprite* );

  float GetRight(Sprite*);

  float GetTop(Sprite*);

  float GetBottom(Sprite*);

  float GetLastLeft(Entity* ent);

  float GetLastRight(Entity*);

  float GetLastTop(Entity*);

  float GetLastBottom(Entity*);

  float CenterX();

  float CenterY();

  const Vec2 Center();

  float ScreenHeight();

  float ScreenWidth();

  const Rect VisRect();
  const Box4 VisBox();

  const Size Screen();
  const Vec2 SCenter();

  const Vec2 VBoxMID(const Box4& );

  bool TraceEnclosure(float dt, const Box4& bbox,
      const Rect& rect, const Vec2& vel,
      Vec2& outPos, Vec2& outVel);

  /**
   * Get the sprite from the frame cache using
   * its id (e.g. #ship).
   */
  SpriteFrame* GetSpriteFrame(const string& frameid);

  bool HasKeyPad();

  void OnKeyPolls();

  void OnKeys();

  bool HasMouse();

  void OnMouse();

  bool HasTouch();

  void OnTouchAll();

  void OnTouchOne();

  void ResolveElastic(Entity* obj1, Entity* obj2);

  const Vec2 AncCenter();
  const Vec2 AncTop();
  const Vec2 AncTopRight();
  const Vec2 AncRight();
  const Vec2 AncBottomRight();
  const Vec2 AncBottom();
  const Vec2 AncBottomLeft();
  const Vec2 AncLeft();
  const Vec2 AncTopLeft();

};




NS_END(fusilli)
#endif

