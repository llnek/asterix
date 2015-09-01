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

#include "../core/fusilli.h"
#include "cocos2d.h"
USING_NS_STD;
USING_NS_CC;
NS_FI_BEGIN

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Box4 {
  Box4(float t, float r, float b, float l)
    : top(t), right(r), bottom(b), left(l)
  {}
  ~Box4()
  {}
  Box4(const Box4& b) {
    top=b.top;
    right=b.right;
    bottom=b.bottom;
    left=b.left;
  }
  Box4& operator=(const Box4& b) {
    top=b.top;
    right=b.right;
    bottom=b.bottom;
    left=b.left;
  }
  float top;
  float right;
  float bottom;
  float left;
};

class Entity;
class CC_DLL CCSX {

  bool PointInBox(const Rect& box, float x,  float y);
  const Color3B& White();
  const Color3B& Black();

  bool Collide0(Sprite* spriteA, Sprite* spriteB);
  bool Collide(Entity* a, Entity* b);

  void SetDevRes(bool landscape, float w, float h, ResolutionPolicy pcy);

  bool IsPortrait();
  bool OutOfBound(Entity* ent, const Rect& B);

  Action* CreateTimer(Node*, float millis);
  bool UndoTimer(Node*, Action*);
  bool TimerDone(Action*);

  Sprite* CreateSprite(const string& frameName);

  Box4 BBox4B4(Entity* ent);
  Box4 BBox4(Sprite* sprite);

  void RunScene(Scene* ns, float delay);
  bool IsTransitioning();

  Size CSize(const string& frame);

  Vec2 HalfHW(Sprite* sprite);
  Rect BBox(Sprite* sprite);

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

  Vec2& Center();

  float ScreenHeight();

  float ScreenWidth();

  Rect VRect();
  Box4 VBox();

  Size Screen();
  Vec2 SCenter();

  Vec2 VBoxMID(const Box4& );

  TraceResult TraceEnclosure(float dt, const Box4& bbox, const Rect& rect, const Vec2& vel);

  /**
   * Get the sprite from the frame cache using
   * its id (e.g. #ship).
   */
  Sprite* GetSprite(const string& frameid);

  bool HasKeyPad();

  void OnKeyPolls(kb);

  void OnKeys(bus);

  bool HasMouse();

  void OnMouse(bus);

  bool HasTouch();

  void OnTouchAll(bus);

  void OnTouchOne(bus);

  /*
    Center: cc.p(0.5, 0.5),
    Top: cc.p(0.5, 1),
    TopRight: cc.p(1, 1),
    Right: cc.p(1, 0.5),
    BottomRight: cc.p(1, 0),
    Bottom: cc.p(0.5, 0),
    BottomLeft: cc.p(0, 0),
    Left: cc.p(0, 0.5),
    TopLeft: cc.p(0, 1)
*/

  void ResolveElastic(Entity* obj1, Entity* obj2);

  /**
   * Create a text menu containing this set of items.
   *
   * Each item has the form {:text
   * :fontPath
   * :cb
   * :target}
   * @method
   * @param {Array} items
   * @param {Number} scale
   * @return {cc.Menu}
   */
  Menu* TMenu(const Vector& items, float scale = 1);

  /**
   * Make a text label menu containing one single button.
   * @method
   * @param {Object} options
   * @return {cc.Menu}
   */
  Menu* TMenu1(Dictionary* options);

  /**
   * Create a vertically aligned menu with graphic buttons.
   * @method
   * @param {Array} items
   * @param {Object} options
   * @return {cc.Menu}
   */
  Menu* VMenu(const Vector& items, Dictionary* options);

  /**
   * Create a horizontally aligned menu with graphic buttons.
   * @method
   * @param {Array} items
   * @param {Object} options
   * @return {cc.Menu}
   */
  Menu* HMenu(const Vector& items, Dictionary* options);

  /**
   * Create a menu with graphic buttons.
   * @method
   * @param {Boolean} vertical
   * @param {Array} items
   * @param {Number} scale
   * @param {Number} padding
   * @return {cc.Menu}
   */
  Menu* PMenu(bool vertical, const Vector& items, float scale, float padding);

  /**
   * Create a single button menu.
   * @method
   * @param {Object} options
   * @return {cc.Menu}
   */
  Menu* PMenu1(Dictionary* options);

  /**
   * Create a Label.
   * @method
   * @param {Object} options
   * @return {cc.LabelBMFont}
   */
  Label* BmfLabel(Dictionary* options);

};




NS_FI_END
#endif

