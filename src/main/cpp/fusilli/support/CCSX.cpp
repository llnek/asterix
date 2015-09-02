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

#include "CCSX.h"
USING_NS_STD;
USING_NS_CC;
NS_FI_BEGIN

//////////////////////////////////////////////////////////////////////////////
//
/**
 * Test if this point is inside this rectangle
 */
bool CCSX::PointInBox(const Box4& box, float x, float y) {
  return x >= box.left && x <= box.right &&
    y >= box.bottom && y <= box.top;
}

const Color3B& CCSX::White() {
  return Color3B::WHITE;
}

const Color3B& CCSX::Black() {
  return Color3B::BLACK;
}

  /**
   * Test collision of 2 entities using cc-rects.  Each entity
   * wraps a sprite object.
   * @method
   * @param {Object} a
   * @param {Object} b
   * @return {Boolean}
   */
bool CCSX::Collide(Entity* a, Entity* b) {
  if (a != nullptr && b != nullptr) {
    return Collide0(a->sprite, b->sprite);
  } else {
    return false;
  }
}

  /**
   * Test collision of 2 sprites.
   * @method
   * @param {Object} spriteA
   * @param {Object} spriteB
   * @return {Boolean}
   */
bool CCSX::Collide0(Sprite* a, Sprite* b) {
  if (a != nullptr && b != nullptr) {
    return BBox(a).intersectsRect( BBox(b));
  } else {
    return false;
  }
}

void CCSX::SetDevRes(bool landscape, float x, float y, ResolutionPolicy pcy) {
  auto v= Director::getInstance()->getOpenGLView();
  if (landscape) {
    v->setDesignResolutionSize(x, y, pcy);
  } else {
    v->setDesignResolutionSize(y, x, pcy);
  }
}

bool CCSX::IsPortrait() {
  auto s=Screen();
  return s.height > s.width;
}

bool CCSX::OutOfBound(Entity* ent, const Box4& B) {
  if (ent != nullptr && ent->sprite != nullptr) {
    return OutOfBound(BBox4(ent->sprite), B);
  } else {
    return false;
  }
}

bool CCSX::OutOfBound(const Box4& ent, const Box4& B) {
    return false;
}

void CCSX::UndoTimer(Action* tm) {
  if (tm != nullptr) { tm->release(); }
}

  /**
   * Create a timer action.
   * @method
   * @param {Object} par
   * @param {Object} tm
   * @return {Object} timer action
   */
Action* CCSX::CreateTimer(Node* par, float tm) {
  return par->runAction(DelayTime::create(tm));
}

bool CCSX::TimerDone(Action* t) {
  return (t != nullptr) ? t->isDone() : false;
}

  /**
   * Create a sprite from its frame name.
   * @method
   * @param {String} name
   * @return {cc.Sprite}
   */
Sprite* CCSX::CreateSprite(const string& name) {
  return Sprite::createWithSpriteFrameName(name);
}

const Box4 CCSX::BBox4(Sprite* s) {
  return Box4(
    GetTop(s),
    GetRight(s),
    GetBottom(s),
    GetLeft(s)
  );
}

  /**
   * @method runScene
   * @param {cc.Scene}
   * @param delay
   */
void CCSX::RunScene(Scene* ns, float delay) {
  Director::getInstance()->replaceScene(TransitionCrossFade::create(delay, ns));
}

bool CCSX::IsTransitioning() {
  return dynamic_cast<TransitionScene*>(
    Director::getInstance()->getRunningScene()) != nullptr;
}

  /**
   * Find size of this sprite.
   * @method csize
   * @param {String} frame
   * @return {cc.Size}
   */
const Size CCSX::CSize(const string& frame) {
  return CreateSprite(frame)->getContentSize();
}

  /**
   * Calculate halves of width and height of this sprite.
   */
const Size CCSX::HalfHW(Sprite* s) {
  auto z= s->getContentSize();
  return Size(z.width * 0.5, z.height * 0.5);
}

  /**
   * Create a rectangle from this sprite.
   * @method
   * @param {cc.Sprite} sprite
   * @return {cc.rect} rect
   */
const Rect CCSX::BBox(Sprite* s) {
  return Rect(GetLeft(s),
                 GetBottom(s),
                 GetWidth(s),
                 GetHeight(s));
}

  /**
   * Create a rect from the last frame.
   * @method
   * @param {Object} ent
   * @return {Object} box
   */
const Box4 CCSX::BBox4B4(Entity* ent) {
  return Box4(
    GetLastTop(ent),
    GetLastRight(ent),
    GetLastBottom(ent),
    GetLastLeft(ent));
}

float CCSX::GetScaledHeight(Sprite* s) {
  return s->getContentSize().height * s->getScaleY();
}

float CCSX::GetHeight(Sprite* s) {
  return s->getContentSize().height;
}

float CCSX::GetScaledWidth(Sprite* s) {
  return s->getContentSize().width * s->getScaleX();
}

float CCSX::GetWidth(Sprite* s) {
  return s->getContentSize().width;
}

static float GetXXX(Sprite* s, float px, float bound) {
  auto w= s->getContentSize().width;
  auto a= s->getAnchorPoint().x;
  return px + (bound - a) * w ;
}

static float GetYYY(Sprite* s, float py, float bound) {
  auto h= s->getContentSize().height;
  auto a= s->getAnchorPoint().y;
  return py + (bound - a) * h ;
}

float CCSX::GetLeft(Sprite* s) {
  return GetXXX(s, s->getPosition().x, AncLeft().x);
}

float CCSX::GetRight(Sprite* s) {
  return GetXXX(s, s->getPosition().x, AncRight().x);
}

float CCSX::GetBottom(Sprite* s) {
  return GetYYY(s, s->getPosition().y, AncBottom().y);
}

float CCSX::GetTop(Sprite* s) {
  return GetYYY(s, s->getPosition().y, AncTop().y);
}

float CCSX::GetLastLeft(Entity* ent) {
  if ( ent->lastPos != nullptr) {
    return GetXXX(ent->sprite, ent->lastPos->x, AncLeft().x);
  } else {
    return GetLeft(ent->sprite);
  }
}

float CCSX::GetLastRight(Entity* ent) {
  if ( ent->lastPos != nullptr) {
    return GetXXX(ent->sprite, ent->lastPos->x, AncRight().x);
  } else {
    return GetRight(ent->sprite);
  }
}

float CCSX::GetLastTop(Entity* ent) {
  if (ent->lastPos != nullptr) {
    return GetYYY(ent->sprite, ent->lastPos->y, AncTop().y);
  } else {
    return GetTop(ent->sprite);
  }
}

float CCSX::GetLastBottom(Entity* ent) {
  if (ent->lastPos != nullptr) {
    return GetYYY(ent->sprite, ent->lastPos->y, AncBottom().y);
  } else {
    return GetBottom(ent->sprite);
  }
}

  /**
   * Get the x pos of the center of the visible screen.
   * @method
   * @return {Number}
   */
float CCSX::CenterX() { return Center().x; }

  /**
   * Get the y pos of the center of the visible screen.
   * @method
   * @return {Number}
   */
float CCSX::CenterY() { return Center().y; }

  /**
   * Get the center of the visible screen.
   * @method
   * @return {cc.Point}
   */
const Vec2 CCSX::Center() {
  auto rc = VisRect();
  return Vec2( rc.origin.x + rc.size.width * 0.5,
      rc.origin.y + rc.size.height * 0.5);
}

  /**
   * Get the screen height.
   * @method
   * @return {Number}
   */
float CCSX::ScreenHeight() { return Screen().height; }

  /**
   * Get the screen width.
   * @method
   * @return {Number}
   */
float CCSX::ScreenWidth() { return Screen().width; }

  /**
   * Get the visible screen rectangle.
   * @method
   * @return {Object} cc.rect
   */
const Rect CCSX::VisRect() {
  return Director::getInstance()->getOpenGLView()->getVisibleRect();
}

  /**
   * Get the visible screen box.
   * @method
   * @return {Object} rectangle box.
   */
const Box4 CCSX::VisBox() {
  auto vr = Director::getInstance()->getOpenGLView()->getVisibleRect();
  return Box4(
    vr.origin.y + vr.size.height,
    vr.origin.x + vr.size.width,
    vr.origin.y,
    vr.origin.x
  );
}

  /**
   * Get the actual window/frame size.
   * @method
   * @return {cc.Size}
   */
const Size CCSX::Screen() {
  return Director::getInstance()->getOpenGLView()->getFrameSize();
}

  /**
   * Get the actual screen center.
   * @method
   * @return {cc.Point}
   */
const Vec2 CCSX::SCenter() {
  auto sz = Screen();
  return Vec2(sz.width * 0.5, sz.height * 0.5);
}

  /**
   * Get the center of this box.
   * @method
   * @param {Object} box
   * @return {cc.Point}
   */
const Vec2 CCSX::VBoxMID(const Box4& box) {
  return Vec2(box.left + (box.right-box.left) * 0.5,
              box.bottom + (box.top-box.bottom) * 0.5);
}

  /**
   * Test if this box is hitting boundaries.
   * rect.x & y are center positioned.
   *
   * If hit, the new position and velocities
   * are returned.
   * @method
   * @param {Number} dt  delta time
   * @param {Object} bbox
   * @param {Oject} rect
   * @param {Object} vel velocity for [x,y]
   * @return {Object}
   */
bool CCSX::TraceEnclosure(float dt, const Box4& bbox,
    const Rect& rect, const Vec2& vel,
    Vec2& outPos, Vec2& outVel) {
  auto y = rect.origin.y + dt * vel.y;
  auto x = rect.origin.x + dt * vel.x;
  auto sz= rect.size.height * 0.5;
  auto sw= rect.size.width * 0.5;
  auto vx= vel.x;
  auto vy= vel.y;
  auto hit=false;

  if (y + sz > bbox.top) {
    //hitting top wall
    y = bbox.top - sz;
    vy = -vy;
    hit=true;
  }
  else
  if (y - sz < bbox.bottom) {
    //hitting bottom wall
    y = bbox.bottom + sz;
    vy = -vy;
    hit=true;
  }

  if (x + sw > bbox.right) {
    //hitting right wall
    x = bbox.right - sw;
    vx = -vx;
    hit=true;
  }
  else
  if (x - sw < bbox.left) {
    //hitting left wall
    x = bbox.left + sw;
    vx = -vx;
    hit=true;
  }

  outPos.x=x;
  outPos.y=y;
  outVel.x=vx;
  outVel.y=vy;

  return hit;
}

  /**
   * Get the sprite from the frame cache using
   * its id (e.g. #ship).
   * @method getSprite
   * @param {String} frameid
   * @return {cc.Sprite}
   */
Sprite* CCSX::GetSprite(const string& frameid) {
  return SpriteFrameCache::getInstance()->getSpriteFrameByName(frameid);
}

  /**
   * @method hasKeyPad
   * @return {Boolean}
   */
bool CCSX::HasKeyPad() {
  return false;
}

  /**
   * @method onKeyPolls
   */
void CCSX::OnKeyPolls() {
}

  /**
   * @method onKeys
   */
void CCSX::OnKeys() {
  if (!HasKeyPad()) {return;}
  /*
  cc.eventManager.addListener({
    onKeyPressed(key, e) {
      bus.fire('/key/down', {group: 'key', key: key, event: e});
    },
    onKeyReleased(key, e) {
      bus.fire('/key/up', {group: 'key', key: key, event: e});
    },
    event: cc.EventListener.KEYBOARD
  }, sh.main);
  */
}

  /**
   * @method hasMouse
   * @return {Boolean}
   */
bool CCSX::HasMouse() {
  return false;
}

void CCSX::OnMouse() {
if (!HasMouse()) {return;}
/*
cc.eventManager.addListener({
  onMouseMove(e) {
    if (e.getButton() === cc.EventMouse.BUTTON_LEFT) {
      bus.fire('/mouse/move', {group:'mouse',
               loc: e.getLocation(),
               delta: e.getDelta(),
               event: e});
    }
  },
  onMouseDown(e) {
    bus.fire('/mouse/down', {group:'mouse',
             loc: e.getLocation(),
             event: e});
  },
  onMouseUp(e) {
    bus.fire('/mouse/up', {group:'mouse',
             loc: e.getLocation(),
             event: e});
  },
  event: cc.EventListener.MOUSE
  }, sh.main);
  */
}

  /**
   * @method hasTouch
   * @return {Boolean}
   */
bool CCSX::HasTouch() {
  return false; //!!cc.sys.capabilities['touches'];
}

void CCSX::OnTouchAll() {
  if (!HasTouch()) {return;}
  /*
  cc.eventManager.addListener({
    event: cc.EventListener.TOUCH_ALL_AT_ONCE,
    prevTouchId: -1,
    onTouchesBegan(ts,e) { return true; },
    onTouchesEnded(ts,e) {
      bus.fire('/touch/all/end', {group:'touch',
                 event: e,
                 loc: ts[0].getLocation()});
    },
    onTouchesMoved(ts,e) {
      const id = ts[0].id;
      if (this.prevTouchId != id) {
        this.prevTouchId = id;
      } else {
        bus.fire('/touch/all/move', {group:'touch',
                 event: e,
                 delta: ts[0].getDelta()});
      }
    }
  }, sh.main);
  */
}

void CCSX::OnTouchOne() {
  if (!HasTouch()) {return;}
  /*
  cc.eventManager.addListener({
    event: cc.EventListener.TOUCH_ONE_BY_ONE,
    swallowTouches: true,
    onTouchBegan(t,e) { return true; },
    onTouchMoved(t,e) {
      bus.fire('/touch/one/move', {group:'touch',
               event: e,
               delta: t.getDelta(),
               loc: t.getLocation()});
    },
    onTouchEnded(t,e) {
      bus.fire('/touch/one/end', {group:'touch',
               event: e,
               loc: t.getLocation()});
    }
  }, sh.main);
  */
}

const Vec2 CCSX::AncCenter() { return Vec2(0.5, 0.5); }
const Vec2 CCSX::AncTop() { return Vec2(0.5, 1); }
const Vec2 CCSX::AncTopRight() { return Vec2(1, 1); }
const Vec2 CCSX::AncRight() { return Vec2(1, 0.5); }
const Vec2 CCSX::AncBottomRight() { return Vec2(1, 0); }
const Vec2 CCSX::AncBottom() { return Vec2(0.5, 0); }
const Vec2 CCSX::AncBottomLeft() { return Vec2(0, 0); }
const Vec2 CCSX::AncLeft() { return Vec2(0, 0.5); }
const Vec2 CCSX::AncTopLeft() { return Vec2(0, 1); }

  /**
   * not used for now.
   * @private
   */
void CCSX::ResolveElastic(Entity* obj1, Entity* obj2) {
  auto pos2 = obj2->sprite->getPosition();
  auto pos1= obj1->sprite->getPosition();
  auto sz2= obj2->sprite->getContentSize();
  auto sz1= obj1->sprite->getContentSize();
  auto hh1= sz1.height * 0.5;
  auto hw1= sz1.width * 0.5;
  auto x = pos1.x;
  auto y= pos1.y;
  auto bx2 = BBox4(obj2->sprite);
  auto bx1 = BBox4(obj1->sprite);

  // coming from right
  if (bx1.left < bx2.right && bx2.right < bx1.right) {
    obj2->vel.x = - abs(obj2->vel.x);
    obj1->vel.x = abs(obj1->vel.x);
    x= GetRight(obj2->sprite) + hw1;
  }
  else
  // coming from left
  if (bx1.right > bx2.left && bx1.left < bx2.left) {
    obj1->vel.x = - abs(obj1->vel.x);
    obj2->vel.x = abs(obj2->vel.x);
    x= GetLeft(obj2->sprite) - hw1;
  }
  else
  // coming from top
  if (bx1.bottom < bx2.top && bx1.top > bx2.top) {
    obj2->vel.y = - abs(obj2->vel.y);
    obj1->vel.y = abs(obj1->vel.y);
    y= GetTop(obj2->sprite) + hh1;
  }
  else
  // coming from bottom
  if (bx1.top > bx2.bottom && bx2.bottom > bx1.bottom) {
    obj1->vel.y = - abs(obj1->vel.y);
    obj2->vel.y = abs(obj2->vel.y);
    y= GetBottom(obj2->sprite) - hh1;
  }
  else {
    return;
  }
  obj1->updatePosition(x,y);
}


NS_FI_END


