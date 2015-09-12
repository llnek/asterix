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
NS_USING(cocos2d)
NS_USING(std)
NS_BEGIN(fusilli)


//////////////////////////////////////////////////////////////////////////////
// Test if this point is inside this rectangle
bool CCSX::PointInBox(const Box4& box, float x, float y) {
  return x >= box.left && x <= box.right &&
    y >= box.bottom && y <= box.top;
}

//////////////////////////////////////////////////////////////////////////
//
/*
const Color3B CCSX::White() {
  return Color3B::WHITE;
}
//////////////////////////////////////////////////////////////////////////
//
const Color3B CCSX::Black() {
  return Color3B::BLACK;
}
*/

//////////////////////////////////////////////////////////////////////////
// Test collision of 2 entities using cc-rects
bool CCSX::Collide(ComObj* a, ComObj* b) {
  if (NNP(a) && NNP(b)) {
    return Collide0(a->sprite, b->sprite);
  } else {
    return false;
  }
}

//////////////////////////////////////////////////////////////////////////
// Test collision of 2 sprites
bool CCSX::Collide0(Sprite* a, Sprite* b) {
  if (NNP(a) && NNP(b)) {
    return BBox(a).intersectsRect( BBox(b));
  } else {
    return false;
  }
}

//////////////////////////////////////////////////////////////////////////
//
void CCSX::SetDevRes(bool landscape, float x, float y, ResolutionPolicy pcy) {
  auto v= Director::getInstance()->getOpenGLView();
  if (landscape) {
    v->setDesignResolutionSize(x, y, pcy);
  } else {
    v->setDesignResolutionSize(y, x, pcy);
  }
}

//////////////////////////////////////////////////////////////////////////
//
bool CCSX::IsPortrait() {
  auto s=Screen();
  return s.height > s.width;
}

//////////////////////////////////////////////////////////////////////////
//
bool CCSX::OutOfBound(ComObj* ent, const Box4& B) {
  if (NNP(ent) && NNP(ent->sprite)) {
    return OutOfBound(BBox4(ent->sprite), B);
  } else {
    return false;
  }
}

//////////////////////////////////////////////////////////////////////////
//
bool CCSX::OutOfBound(const Box4& a, const Box4& B) {
  return a.left > B.right    ||
         a.top < B.bottom  ||
         a.right < B.left      ||
         a.bottom > B.top;
}

//////////////////////////////////////////////////////////////////////////
//
void CCSX::UndoTimer(Action* tm) {
  if (NNP(tm)) { tm->release(); }
}

//////////////////////////////////////////////////////////////////////////
// Create a timer action
//
Action* CCSX::CreateTimer(Node* par, float tm) {
  return par->runAction(DelayTime::create(tm));
}

//////////////////////////////////////////////////////////////////////////
//
bool CCSX::TimerDone(Action* t) {
  return NNP(t) ? t->isDone() : false;
}

//////////////////////////////////////////////////////////////////////////
// Create a sprite from its frame name
//
Sprite* CCSX::CreateSprite(const string& name) {
  return Sprite::createWithSpriteFrameName(name);
}

//////////////////////////////////////////////////////////////////////////
//
const Box4 CCSX::BBox4(Sprite* s) {
  return Box4(
    GetTop(s),
    GetRight(s),
    GetBottom(s),
    GetLeft(s)
  );
}

//////////////////////////////////////////////////////////////////////////
//
void CCSX::RunScene(Scene* ns, float delay) {
  Director::getInstance()->replaceScene(TransitionCrossFade::create(delay, ns));
}

//////////////////////////////////////////////////////////////////////////
//
void CCSX::RunScene(Scene* ns) {
  Director::getInstance()->replaceScene(ns);
}

//////////////////////////////////////////////////////////////////////////
//
bool CCSX::IsTransitioning() {
  return dynamic_cast<TransitionScene*>(
    Director::getInstance()->getRunningScene()) != nullptr;
}

//////////////////////////////////////////////////////////////////////////
// Find size of this sprite
//
const Size CCSX::CSize(const string& frame) {
  return CreateSprite(frame)->getContentSize();
}

//////////////////////////////////////////////////////////////////////////
// Calculate halves of width and height of this sprite
//
const Size CCSX::HalfHW(Sprite* s) {
  auto z= s->getContentSize();
  return Size(z.width * 0.5, z.height * 0.5);
}

//////////////////////////////////////////////////////////////////////////
// Create a rectangle from this sprite
//
const Rect CCSX::BBox(Sprite* s) {
  return Rect(GetLeft(s),
                 GetBottom(s),
                 GetWidth(s),
                 GetHeight(s));
}

//////////////////////////////////////////////////////////////////////////
// Create a rect from the last frame
//
const Box4 CCSX::BBox4B4(ComObj* ent) {
  return Box4(
    GetLastTop(ent),
    GetLastRight(ent),
    GetLastBottom(ent),
    GetLastLeft(ent));
}

//////////////////////////////////////////////////////////////////////////
//
float CCSX::GetScaledHeight(Sprite* s) {
  return s->getContentSize().height * s->getScaleY();
}

//////////////////////////////////////////////////////////////////////////
//
float CCSX::GetHeight(Sprite* s) {
  return s->getContentSize().height;
}

//////////////////////////////////////////////////////////////////////////
//
float CCSX::GetScaledWidth(Sprite* s) {
  return s->getContentSize().width * s->getScaleX();
}

//////////////////////////////////////////////////////////////////////////
//
float CCSX::GetWidth(Sprite* s) {
  return s->getContentSize().width;
}

//////////////////////////////////////////////////////////////////////////
//
static float get_XXX(Sprite* s, float px, float bound) {
  auto w= s->getContentSize().width;
  auto a= s->getAnchorPoint().x;
  return px + (bound - a) * w ;
}

//////////////////////////////////////////////////////////////////////////
//
static float get_YYY(Sprite* s, float py, float bound) {
  auto h= s->getContentSize().height;
  auto a= s->getAnchorPoint().y;
  return py + (bound - a) * h ;
}

//////////////////////////////////////////////////////////////////////////
//
float CCSX::GetLeft(Sprite* s) {
  return get_XXX(s, s->getPosition().x, AnchorL().x);
}

//////////////////////////////////////////////////////////////////////////
//
float CCSX::GetRight(Sprite* s) {
  return get_XXX(s, s->getPosition().x, AnchorR().x);
}

//////////////////////////////////////////////////////////////////////////
//
float CCSX::GetBottom(Sprite* s) {
  return get_YYY(s, s->getPosition().y, AnchorB().y);
}

//////////////////////////////////////////////////////////////////////////
//
float CCSX::GetTop(Sprite* s) {
  return get_YYY(s, s->getPosition().y, AnchorT().y);
}

//////////////////////////////////////////////////////////////////////////
//
float CCSX::GetLastLeft(ComObj* ent) {
  return get_XXX(ent->sprite, ent->lastPos.x, AnchorL().x);
}

//////////////////////////////////////////////////////////////////////////
//
float CCSX::GetLastRight(ComObj* ent) {
  return get_XXX(ent->sprite, ent->lastPos.x, AnchorR().x);
}

//////////////////////////////////////////////////////////////////////////
//
float CCSX::GetLastTop(ComObj* ent) {
  return get_YYY(ent->sprite, ent->lastPos.y, AnchorT().y);
}

//////////////////////////////////////////////////////////////////////////
//
float CCSX::GetLastBottom(ComObj* ent) {
  return get_YYY(ent->sprite, ent->lastPos.y, AnchorB().y);
}

//////////////////////////////////////////////////////////////////////////
// Get the x pos of the center of the visible screen
//
float CCSX::CenterX() { return Center().x; }

//////////////////////////////////////////////////////////////////////////
// Get the y pos of the center of the visible screen.
//
float CCSX::CenterY() { return Center().y; }

//////////////////////////////////////////////////////////////////////////
// Get the center of the visible screen
//
const Vec2 CCSX::Center() {
  auto rc = VisRect();
  return Vec2( rc.origin.x + rc.size.width * 0.5,
      rc.origin.y + rc.size.height * 0.5);
}

//////////////////////////////////////////////////////////////////////////
// Get the screen height
//
float CCSX::ScreenHeight() { return Screen().height; }

//////////////////////////////////////////////////////////////////////////
// Get the screen width
//
float CCSX::ScreenWidth() { return Screen().width; }

//////////////////////////////////////////////////////////////////////////
// Get the visible screen rectangle
//
const Rect CCSX::VisRect() {
  return Director::getInstance()->getOpenGLView()->getVisibleRect();
}

//////////////////////////////////////////////////////////////////////////
// Get the visible screen box
//
const Box4 CCSX::VisBox() {
  auto vr = Director::getInstance()->getOpenGLView()->getVisibleRect();
  return Box4(
    vr.origin.y + vr.size.height,
    vr.origin.x + vr.size.width,
    vr.origin.y,
    vr.origin.x
  );
}

//////////////////////////////////////////////////////////////////////////
// Get the actual window/frame size.
//
const Size CCSX::Screen() {
  return Director::getInstance()->getOpenGLView()->getFrameSize();
}

//////////////////////////////////////////////////////////////////////////
// Get the actual screen center.
//
const Vec2 CCSX::SCenter() {
  auto sz = Screen();
  return Vec2(sz.width * 0.5, sz.height * 0.5);
}

//////////////////////////////////////////////////////////////////////////
// Get the center of this box.
//
const Vec2 CCSX::VBoxMID(const Box4& box) {
  return Vec2(box.left + (box.right-box.left) * 0.5,
              box.bottom + (box.top-box.bottom) * 0.5);
}

//////////////////////////////////////////////////////////////////////////
// Test if this box is hitting boundaries
// rect.x & y are center positioned
//
// If hit, the new position and velocities
// are returned
//
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

//////////////////////////////////////////////////////////////////////////
// Get the sprite from the frame cache using its id (e.g. #ship)
//
SpriteFrame* CCSX::GetSpriteFrame(const string& frameid) {
  return SpriteFrameCache::getInstance()->getSpriteFrameByName(frameid);
}

//////////////////////////////////////////////////////////////////////////
//
bool CCSX::HasKeyPad() {
  return false;
}

//////////////////////////////////////////////////////////////////////////
//
void CCSX::OnKeyPolls() {
}

//////////////////////////////////////////////////////////////////////////
//
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

//////////////////////////////////////////////////////////////////////////
//
bool CCSX::HasMouse() {
  return false;
}

//////////////////////////////////////////////////////////////////////////
//
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

//////////////////////////////////////////////////////////////////////////
//
bool CCSX::HasTouch() {
  return false; //!!cc.sys.capabilities['touches'];
}

//////////////////////////////////////////////////////////////////////////
//
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

//////////////////////////////////////////////////////////////////////////
//
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

//////////////////////////////////////////////////////////////////////////
//
const Vec2 CCSX::AnchorC() { return Vec2(0.5, 0.5); }

//////////////////////////////////////////////////////////////////////////
//
const Vec2 CCSX::AnchorT() { return Vec2(0.5, 1); }

//////////////////////////////////////////////////////////////////////////
//
const Vec2 CCSX::AnchorTR() { return Vec2(1, 1); }

//////////////////////////////////////////////////////////////////////////
//
const Vec2 CCSX::AnchorR() { return Vec2(1, 0.5); }

//////////////////////////////////////////////////////////////////////////
//
const Vec2 CCSX::AnchorBR() { return Vec2(1, 0); }

//////////////////////////////////////////////////////////////////////////
//
const Vec2 CCSX::AnchorB() { return Vec2(0.5, 0); }

//////////////////////////////////////////////////////////////////////////
//
const Vec2 CCSX::AnchorBL() { return Vec2(0, 0); }

//////////////////////////////////////////////////////////////////////////
//
const Vec2 CCSX::AnchorL() { return Vec2(0, 0.5); }

//////////////////////////////////////////////////////////////////////////
//
const Vec2 CCSX::AnchorTL() { return Vec2(0, 1); }

//////////////////////////////////////////////////////////////////////////
// not used for now.
//
void CCSX::ResolveElastic(ComObj* obj1, ComObj* obj2) {
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
    obj2->vel.x = - fabs(obj2->vel.x);
    obj1->vel.x = fabs(obj1->vel.x);
    x= GetRight(obj2->sprite) + hw1;
  }
  else
  // coming from left
  if (bx1.right > bx2.left && bx1.left < bx2.left) {
    obj1->vel.x = - fabs( obj1->vel.x);
    obj2->vel.x = fabs(obj2->vel.x);
    x= GetLeft(obj2->sprite) - hw1;
  }
  else
  // coming from top
  if (bx1.bottom < bx2.top && bx1.top > bx2.top) {
    obj2->vel.y = - fabs(obj2->vel.y);
    obj1->vel.y = fabs(obj1->vel.y);
    y= GetTop(obj2->sprite) + hh1;
  }
  else
  // coming from bottom
  if (bx1.top > bx2.bottom && bx2.bottom > bx1.bottom) {
    obj1->vel.y = - fabs(obj1->vel.y);
    obj2->vel.y = fabs(obj2->vel.y);
    y= GetBottom(obj2->sprite) - hh1;
  }
  else {
    return;
  }
  obj1->UpdatePosition(x,y);
}


NS_END(fusilli)


