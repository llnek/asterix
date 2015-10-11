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

#include "audio/include/SimpleAudioEngine.h"
#include "2d/CCSpriteFrameCache.h"
#include "2d/CCActionInterval.h"
#include "2d/CCMenuItem.h"
#include "2d/CCMenu.h"
#include "2d/CCLabel.h"
#include "2d/CCTransition.h"
#include "base/CCDirector.h"
#include "XConfig.h"
#include "CCSX.h"

NS_ALIAS(den, CocosDenshion)
NS_BEGIN(fusii)
NS_BEGIN(ccsx)

//////////////////////////////////////////////////////////////////////////////
// b & q should be same size
c::Menu* MkBackQuit(not_null<c::MenuItem*> b,
    not_null<c::MenuItem*> q, bool vert) {

  auto menu= c::Menu::create();
//  auto padding = 10;

  menu->addChild(b);
  menu->addChild(q);

  if (!vert) {
    menu->alignItemsHorizontally();//WithPadding(padding);
  } else {
    menu->alignItemsVertically();//WithPadding(padding);
  }

  return menu;
}

//////////////////////////////////////////////////////////////////////////////
//
void SfxPlay(const stdstr& sound) {
  auto fp= XCFG()->GetEffect(sound);
  try {
    den::SimpleAudioEngine::getInstance()->playEffect(fp.c_str());
  } catch (...) {
    CCLOG("failed to play sound: %s", sound.c_str());
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void CreateAudioIcons(c::MenuItem*& on, c::MenuItem*& off) {
  auto n="sound_off.png";
  off= CreateMenuBtn(n,n,n);
  n="sound_on.png";
  on= CreateMenuBtn(n,n,n);
}

//////////////////////////////////////////////////////////////////////////////
//
c::MenuItem* CreateMenuBtn(const stdstr& n) {
  return CreateMenuBtn(n,n,n);
}

//////////////////////////////////////////////////////////////////////////////
//
c::MenuItem* CreateMenuBtn(const stdstr& n,
    const stdstr& s,
    const stdstr& d) {

  return c::MenuItemSprite::create(CreateSprite(n),
                                CreateSprite(s),
                                CreateSprite(d));
}

//////////////////////////////////////////////////////////////////////////////
// Test if this point is inside this rectangle
bool PointInBox(const Box4& box, float x, float y) {
  return x >= box.left && x <= box.right &&
    y >= box.bottom && y <= box.top;
}

//////////////////////////////////////////////////////////////////////////
//
const c::Color3B White() {
  return c::Color3B::WHITE;
}
//////////////////////////////////////////////////////////////////////////
//
const c::Color3B Black() {
  return c::Color3B::BLACK;
}

//////////////////////////////////////////////////////////////////////////////
//
c::Label* CreateBmfLabel(const stdstr& font, const stdstr& text) {

  auto f= c::Label::createWithBMFont( XCFG()->GetFont(font), text);
  f->setOpacity(0.9*255);
  return f;
}
//////////////////////////////////////////////////////////////////////////////
//
c::Label* CreateBmfLabel(float x, float y,
    const stdstr& font, const stdstr& text) {

  auto f= c::Label::createWithBMFont(XCFG()->GetFont(font), text);
  f->setPosition(x,y);
  f->setOpacity(0.9*255);
  return f;
}

//////////////////////////////////////////////////////////////////////////
// Test collision of 2 entities using cc-rects
bool Collide(not_null<ComObj*> a, not_null<ComObj*> b) {
  if (NNP(a) && NNP(b)) {
    return CollideN(a->sprite, b->sprite);
  } else {
    return false;
  }
}

//////////////////////////////////////////////////////////////////////////
// Test collision of 2 sprites
bool CollideN(not_null<c::Node*> a, not_null<c::Node*> b) {
  if (NNP(a) && NNP(b)) {
    return BBox(a).intersectsRect( BBox(b));
  } else {
    return false;
  }
}

//////////////////////////////////////////////////////////////////////////
//
void SetDevRes(float x, float y, ResolutionPolicy pcy) {
  CC_DTOR()->getOpenGLView()->setDesignResolutionSize(x, y, pcy);
}

//////////////////////////////////////////////////////////////////////////
//
bool IsPortrait() {
  auto s=Screen();
  return s.height > s.width;
}

//////////////////////////////////////////////////////////////////////////
//
bool OutOfBound(not_null<ComObj*> ent, const Box4& B) {
  if (NNP(ent) && NNP(ent->sprite)) {
    return OutOfBound(BBox4(ent->sprite), B);
  } else {
    return false;
  }
}

//////////////////////////////////////////////////////////////////////////
//
bool OutOfBound(const Box4& a, const Box4& B) {
  return a.left > B.right    ||
         a.top < B.bottom  ||
         a.right < B.left      ||
         a.bottom > B.top;
}

//////////////////////////////////////////////////////////////////////////
//
void UndoTimer(not_null<c::DelayTime*> tm) {
  CC_DROP(tm)
}

//////////////////////////////////////////////////////////////////////////
// Create a timer action
//
c::DelayTime* CreateTimer(not_null<c::Node*> par, float tm) {
  auto t= c::DelayTime::create(tm);
  par->runAction(t);
  CC_KEEP(t)
  return t;
}

//////////////////////////////////////////////////////////////////////////
//
bool TimerDone(not_null<c::DelayTime*> t) {
  return NNP(t) ? t->isDone() : false;
}

//////////////////////////////////////////////////////////////////////////
// Create a sprite from its frame name
//
c::Sprite* CreateSprite(const stdstr& name) {
  return c::Sprite::createWithSpriteFrameName(name);
}

//////////////////////////////////////////////////////////////////////////
//
const Box4 BBox4(not_null<c::Node*> s) {
  auto z= s->getBoundingBox();
  return Box4(z.origin.y + z.size.height,
      z.origin.x + z.size.width,
      z.origin.y,
      z.origin.x);
  /*
    GetTop(s),
    GetRight(s),
    GetBottom(s),
    GetLeft(s) */
}

//////////////////////////////////////////////////////////////////////////
//
void RunScene(not_null<c::Scene*> ns, float delay) {
  CC_DTOR()->replaceScene(
      c::TransitionCrossFade::create(delay, ns));
}

//////////////////////////////////////////////////////////////////////////
//
void RunScene(not_null<c::Scene*> ns) {
  CC_DTOR()->replaceScene(ns);
}

//////////////////////////////////////////////////////////////////////////
//
bool IsTransitioning() {
  return dynamic_cast<c::TransitionScene*>(
      CC_DTOR()->getRunningScene()) != nullptr;
}

//////////////////////////////////////////////////////////////////////////
// Find size of this sprite
//
const c::Size CalcSize(const stdstr& frame) {
  return CreateSprite(frame)->getContentSize();
}

//////////////////////////////////////////////////////////////////////////
// Calculate halves of width and height of this sprite
//
const c::Size HalfHW(not_null<c::Sprite*> s) {
  auto z= s->getContentSize();
  return c::Size(z.width * 0.5, z.height * 0.5);
}

//////////////////////////////////////////////////////////////////////////
// Create a rectangle from this sprite
//
const c::Rect BBox(not_null<c::Node*> s) {
  return s->getBoundingBox();
  /*
  return c::Rect(GetLeft(s),
                 GetBottom(s),
                 GetWidth(s),
                 GetHeight(s)); */
}

//////////////////////////////////////////////////////////////////////////
// Create a rect from the last frame
//
const Box4 BBox4B4(not_null<ComObj*> ent) {
  return Box4(
    GetLastTop(ent),
    GetLastRight(ent),
    GetLastBottom(ent),
    GetLastLeft(ent));
}

//////////////////////////////////////////////////////////////////////////
//
float GetScaledHeight(not_null<c::Node*> s) {
  return s->getContentSize().height * s->getScaleY();
}

//////////////////////////////////////////////////////////////////////////
//
float GetHeight(not_null<c::Node*> s) {
  return s->getContentSize().height;
}

//////////////////////////////////////////////////////////////////////////
//
float GetScaledWidth(not_null<c::Node*> s) {
  return s->getContentSize().width * s->getScaleX();
}

//////////////////////////////////////////////////////////////////////////
//
float GetWidth(not_null<c::Node*> s) {
  return s->getContentSize().width;
}

//////////////////////////////////////////////////////////////////////////
//
static float get_XXX(not_null<c::Node*> s, float px, float bound) {
  auto w= s->getContentSize().width;
  auto a= s->getAnchorPoint().x;
  return px + (bound - a) * w ;
}

//////////////////////////////////////////////////////////////////////////
//
static float get_YYY(not_null<c::Node*> s, float py, float bound) {
  auto h= s->getContentSize().height;
  auto a= s->getAnchorPoint().y;
  return py + (bound - a) * h ;
}

//////////////////////////////////////////////////////////////////////////
//
float GetLeft(not_null<c::Node*> s) {
  return get_XXX(s, s->getPosition().x, AnchorL().x);
}

//////////////////////////////////////////////////////////////////////////
//
float GetRight(not_null<c::Node*> s) {
  return get_XXX(s, s->getPosition().x, AnchorR().x);
}

//////////////////////////////////////////////////////////////////////////
//
float GetBottom(not_null<c::Node*> s) {
  return get_YYY(s, s->getPosition().y, AnchorB().y);
}

//////////////////////////////////////////////////////////////////////////
//
float GetTop(not_null<c::Node*> s) {
  return get_YYY(s, s->getPosition().y, AnchorT().y);
}

//////////////////////////////////////////////////////////////////////////
//
float GetLastLeft(not_null<ComObj*> ent) {
  return get_XXX(ent->sprite, ent->lastPos.x, AnchorL().x);
}

//////////////////////////////////////////////////////////////////////////
//
float GetLastRight(not_null<ComObj*> ent) {
  return get_XXX(ent->sprite, ent->lastPos.x, AnchorR().x);
}

//////////////////////////////////////////////////////////////////////////
//
float GetLastTop(not_null<ComObj*> ent) {
  return get_YYY(ent->sprite, ent->lastPos.y, AnchorT().y);
}

//////////////////////////////////////////////////////////////////////////
//
float GetLastBottom(not_null<ComObj*> ent) {
  return get_YYY(ent->sprite, ent->lastPos.y, AnchorB().y);
}

//////////////////////////////////////////////////////////////////////////
// Get the x pos of the center of the visible screen
//
float CenterX() { return Center().x; }

//////////////////////////////////////////////////////////////////////////
// Get the y pos of the center of the visible screen.
//
float CenterY() { return Center().y; }

//////////////////////////////////////////////////////////////////////////
// Get the center of the visible screen
//
const c::Vec2 Center() {
  auto rc = VisRect();
  return c::Vec2( rc.origin.x + rc.size.width * 0.5,
      rc.origin.y + rc.size.height * 0.5);
}

//////////////////////////////////////////////////////////////////////////
// Get the screen height
//
float ScreenHeight() { return Screen().height; }

//////////////////////////////////////////////////////////////////////////
// Get the screen width
//
float ScreenWidth() { return Screen().width; }

//////////////////////////////////////////////////////////////////////////
// Get the visible screen rectangle
//
const c::Rect VisRect() {
  return CC_DTOR()->getOpenGLView()->getVisibleRect();
}

//////////////////////////////////////////////////////////////////////////
// Get the visible screen box
//
const Box4 VisBox() {
  auto vr = CC_DTOR()->getOpenGLView()->getVisibleRect();
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
const c::Size Screen() {
  return CC_DTOR()->getOpenGLView()->getFrameSize();
}

//////////////////////////////////////////////////////////////////////////
// Get the actual screen center.
//
const c::Vec2 SCenter() {
  auto sz = Screen();
  return c::Vec2(sz.width * 0.5, sz.height * 0.5);
}

//////////////////////////////////////////////////////////////////////////
// Get the center of this box.
//
const c::Vec2 VBoxMID(const Box4& box) {
  return c::Vec2(box.left + (box.right-box.left) * 0.5,
              box.bottom + (box.top-box.bottom) * 0.5);
}

//////////////////////////////////////////////////////////////////////////
// Test if this box is hitting boundaries
// rect.x & y are center positioned
//
// If hit, the new position and velocities
// are returned
//
bool TraceEnclosure(float dt, const Box4& bbox,
    const c::Rect& rect, const c::Vec2& vel,
    c::Vec2& outPos, c::Vec2& outVel) {
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
c::SpriteFrame* GetSpriteFrame(const stdstr& frameid) {
  return c::SpriteFrameCache::getInstance()->getSpriteFrameByName(frameid);
}

//////////////////////////////////////////////////////////////////////////
//
bool HasKeyPad() {
  return false;
}

//////////////////////////////////////////////////////////////////////////
//
void OnKeyPolls() {
}

//////////////////////////////////////////////////////////////////////////
//
void OnKeys() {
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
bool HasMouse() {
  return false;
}

//////////////////////////////////////////////////////////////////////////
//
void OnMouse() {
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
bool HasTouch() {
  return false; //!!cc.sys.capabilities['touches'];
}

//////////////////////////////////////////////////////////////////////////
//
void OnTouchAll() {
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
void OnTouchOne() {
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
const c::Vec2 AnchorC() { return c::Vec2(0.5, 0.5); }

//////////////////////////////////////////////////////////////////////////
//
const c::Vec2 AnchorT() { return c::Vec2(0.5, 1); }

//////////////////////////////////////////////////////////////////////////
//
const c::Vec2 AnchorTR() { return c::Vec2(1, 1); }

//////////////////////////////////////////////////////////////////////////
//
const c::Vec2 AnchorR() { return c::Vec2(1, 0.5); }

//////////////////////////////////////////////////////////////////////////
//
const c::Vec2 AnchorBR() { return c::Vec2(1, 0); }

//////////////////////////////////////////////////////////////////////////
//
const c::Vec2 AnchorB() { return c::Vec2(0.5, 0); }

//////////////////////////////////////////////////////////////////////////
//
const c::Vec2 AnchorBL() { return c::Vec2(0, 0); }

//////////////////////////////////////////////////////////////////////////
//
const c::Vec2 AnchorL() { return c::Vec2(0, 0.5); }

//////////////////////////////////////////////////////////////////////////
//
const c::Vec2 AnchorTL() { return c::Vec2(0, 1); }

//////////////////////////////////////////////////////////////////////////
// not used for now.
//
void ResolveElastic(not_null<ComObj*> obj1, not_null<ComObj*> obj2) {
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


NS_END(ccsx)
NS_END(fusii)


