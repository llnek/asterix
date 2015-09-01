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

  /**
   * Set device resolution, policy and orientation.
   * @method
   * @param {Boolean} landscape
   * @param {Number} w
   * @param {Number} h
   * @param {Number} pcy
   */
void CCSX::SetDR(bool landscape, float w, float h, ResolutionPolicy pcy) {
  if (landscape) {
    cc.view.setDesignResolutionSize(w, h, pcy);
  } else {
    cc.view.setDesignResolutionSize(h, w, pcy);
  }
}

bool CCSX::IsPortrait() {
  auto s=this.screen();
  return s.height > s.width;
}

  /**
   * Test if this entity is out of bound.
   * @method
   * @param {Object} ent
   * @param {Object} B
   * @return {Boolean}
   */
bool CCSX::OutOfBound(Entity* ent, const Box4& B) {
  if (ent != nullptr && ent->sprite != nullptr) {
    return OutOfBound(BBox4(ent->sprite), B);
  } else {
    return false;
  }
}

  /**
   * Maybe release this timer.
   * @method
   * @param {Object} par
   * @param {Object} tm
   * @return null
   */
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

  /**
   * Test if this timer is done.
   * @method
   * @param {Object} t
   * @return {Boolean}
   */
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

  /**
   * Create a 4 point rectangle from this sprite.
   * @method
   * @param {cc.Sprite} sprite
   * @return {Object} rect
   */
Box4 CCSX::BBox4(Sprite* s) {
  return Box4(
    GetTop(s),
    GetRight(s),
    GetBottom(s),
    GetLeft(s)
  };
}

  /**
   * @method runScene
   * @param {cc.Scene}
   * @param delay
   */
void CCSX::RunScene(Scene* ns, float delay) {
  Director::getInstance()->replaceScene(TransitionCrossFade::create(delay, ns));
}

  /**
   * @method isTransitioning
   * @return {Boolean}
   */
bool CCSX::IsTransitioning() {
  auto s= Director::getInstance()->getRunningScene();
  auto t= s ? dynamic_cast<TransitionScene*>(s) : nullptr;
  return t != nullptr;
}

  /**
   * Find size of this sprite.
   * @method csize
   * @param {String} frame
   * @return {cc.Size}
   */
const Size& CSize(const string& frame) {
  return CreateSprite(frame)->getContentSize();
}

  /**
   * Calculate halves of width and height of this sprite.
   * @method
   * @param {cc.Sprite} sprite
   * @return {Array} [x, y]
   */
Size CCSX::HalfHW(Sprite* s) {
  auto z= s->getContentSize();
  return Size(z.width * 0.5, z.height * 0.5);
}

  /**
   * Create a rectangle from this sprite.
   * @method
   * @param {cc.Sprite} sprite
   * @return {cc.rect} rect
   */
Rect CCSX::BBox(Sprite* s) {
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
Box4 CCSX::BBox4B4(Entity* ent) {
  return Box4(
    GetLastTop(ent),
    GetLastRight(ent),
    GetLastBottom(ent),
    GetLastLeft(ent));
}

  /**
   * Get the scaled height.
   * @method
   * @param {cc.Sprite} sprite
   * @return {Number}
   */
float CCSX::GetScaledHeight(Sprite* s) {
  return s->getContentSize().height * s->getScaleY();
}

float CCSX::GetHeight(Sprite* s) {
  return s->getContentSize().height;
}

float CCSX::GetScaledWidth(Sprite* s) {
  return s->getContentSize().width * s->getScaleX();
}

  /**
   * Get the width.
   * @method
   * @param {cc.Sprite} sprite
   * @return {Number}
   */
float CCSX::GetWidth(Sprite* s) {
  return s->getContentSize().width;
}

  /**
   * Get the left pos.
   * @method
   * @param {cc.Sprite} sprite
   * @return {Number}
   */
float CCSX::GetLeft(Sprite* s) {
  return s->getPosition().x - GetWidth(s) * 0.5;
}

  /**
   * Get the right pos.
   * @method
   * @param {cc.Sprite} sprite
   * @return {Number}
   */
float CCSX::GetRight(Sprite* s) {
  return s->getPosition().x + GetWidth(s) * 0.5;
}

  /**
   * Get the top pos.
   * @method
   * @param {cc.Sprite} sprite
   * @return {Number}
   */
float CCSX::GetTop(Sprite* s) {
  return s->getPosition().y + GetHeight(s) * 0.5;
}

  /**
   * Get the bottom pos.
   * @method
   * @param {cc.Sprite} sprite
   * @return {Number}
   */
float CCSX::GetBottom(Sprite* s) {
  return s->getPosition().y - GetHeight(s) * 0.5;
}

  /**
   * Maybe get the previous left pos.
   * @method
   * @param {Object} ent
   * @return {Number}
   */
float CCSX::GetLastLeft(Entity* ent) {
  if ( ent->lastPos != nullptr) {
    return ent->lastPos.x - GetWidth(ent->sprite) * 0.5;
  } else {
    return GetLeft(ent);
  }
}

  /**
   * Maybe get the previous right pos.
   * @method
   * @param {Object} ent
   * @return {Number}
   */
float CCSX::GetLastRight(Entity* ent) {
  if ( ent->lastPos != nullptr) {
    return ent->lastPos.x + GetWidth(ent->sprite) * 0.5;
  } else {
    return GetRight(ent);
  }
}

  /**
   * Maybe get the previous top pos.
   * @method
   * @param {Object} ent
   * @return {Number}
   */
float CCSX::GetLastTop(Entity* ent) {
  if (ent->lastPos != nullptr) {
    return ent->lastPos.y + GetHeight(ent->sprite) * 0.5;
  } else {
    return GetTop(ent);
  }
}

  /**
   * Maybe get the previous bottom pos.
   * @method
   * @param {Object} ent
   * @return {Number}
   */
float CCSX::GetLastBottom(ent) {
  if (ent->lastPos != nullptr) {
    return ent->lastPos.y - GetHeight(ent->sprite) * 0.5;
  } else {
    return GetBottom(ent);
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
Vec2 CCSX::Center() {
  auto rc = VisRect();
  return Vec2( rc.x + rc.width * 0.5, rc.y + rc.height * 0.5);
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
Rect CCSX::VisRect() {
  return Director::getInstance().getOpenGLView().getVisibleRect();
}

  /**
   * Get the visible screen box.
   * @method
   * @return {Object} rectangle box.
   */
Box4 CCSX::VisBox() {
  auto vr = Director::getInstance().getOpenGLView().getVisibleRect();
  return Box4(
    vr.y + vr.height,
    vr.x + vr.width,
    vr.y,
    vr.x
  );
}

  /**
   * Get the actual window/frame size.
   * @method
   * @return {cc.Size}
   */
const Size& CCSX::Screen() {
  return Director::getInstance().getOpenGLView().getFrameSize();
}

  /**
   * Get the actual screen center.
   * @method
   * @return {cc.Point}
   */
Vec2 CCSX::SCenter() {
  auto sz = this.screen();
  return Vec2(sz.width * 0.5, sz.height * 0.5);
}

  /**
   * Get the center of this box.
   * @method
   * @param {Object} box
   * @return {cc.Point}
   */
Vec2 CCSX::VBoxMID(const Box4& box) {
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
TraceResult CCSX::TraceEnclosure(float dt, const Box4& bbox, const Rect& rect, const Vec2& vel) {
  auto sz= rect.size.height * 0.5;
  auto sw= rect.size.width * 0.5;
  auto vx= vel.x;
  auto vy= vel.y;
  auto y = rect.origin.y + dt * vel.y;
  auto x = rect.origin.x + dt * vel.x;
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

  return TraceResult(hit, x, y, vx, vy);
}

  /**
   * Get the sprite from the frame cache using
   * its id (e.g. #ship).
   * @method getSprite
   * @param {String} frameid
   * @return {cc.Sprite}
   */
Sprite* CCSX::GetSprite(const string& frameid) {
  return SpriteFrameCache::getInstance().getSpriteFrame(frameid);
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
void CCSX::OnKeyPolls(kb) {
}

  /**
   * @method onKeys
   */
void CCSX::OnKeys(bus) {
  if (!this.hasKeyPad()) {return;}
  cc.eventManager.addListener({
    onKeyPressed(key, e) {
      bus.fire('/key/down', {group: 'key', key: key, event: e});
    },
    onKeyReleased(key, e) {
      bus.fire('/key/up', {group: 'key', key: key, event: e});
    },
    event: cc.EventListener.KEYBOARD
  }, sh.main);
}

  /**
   * @method hasMouse
   * @return {Boolean}
   */
bool CCSX::HasMouse() {
  return false;
}

void CCSX::OnMouse(bus) {
if (!this.hasMouse()) {return;}
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
}

  /**
   * @method hasTouch
   * @return {Boolean}
   */
bool CCSX::HasTouch() {
  return !!cc.sys.capabilities['touches'];
}

void CCSX::OnTouchAll(bus) {
  if (!this.hasTouch()) {return;}
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
}

void CCSX::OnTouchOne(bus) {
  if (!this.hasTouch()) {return;}
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
}

Vec2 CCSX::AncCenter() { return Vec2(0.5, 0.5); }
Vec2 CCSX::AncTop() { return Vec2(0.5, 1); }
Vec2 CCSX::AncTopRight() { return Vec2(1, 1); }
Vec2 CCSX::AncRight() { return Vec2(1, 0.5); }
Vec2 CCSX::AncBottomRight() { return Vec2(1, 0); }
Vec2 CCSX::AncBottom() { return Vec2(0.5, 0); }
Vec2 CCSX::AncBottomLeft() { return Vec2(0, 0); }
Vec2 CCSX::AncLeft() { return Vec2(0, 0.5); }
Vec2 CCSX::AncTopLeft() { return Vec2(0, 1); }

  /**
   * not used for now.
   * @private
   */
void CCSX::ResolveElastic(Entity* obj1, Entity* obj2) {
  let pos2 = obj2.sprite.getPosition(),
  pos1= obj1.sprite.getPosition(),
  sz2= obj2.sprite.getContentSize(),
  sz1= obj1.sprite.getContentSize(),
  hh1= sz1.height * 0.5,
  hw1= sz1.width * 0.5,
  x = pos1.x,
  y= pos1.y,
  bx2 = this.bbox4(obj2.sprite),
  bx1 = this.bbox4(obj1.sprite);

  // coming from right
  if (bx1.left < bx2.right && bx2.right < bx1.right) {
    obj1.vel.x = Math.abs(obj1.vel.x);
    obj2.vel.x = - Math.abs(obj2.vel.x);
    x= this.getRight(obj2.sprite) + hw1;
  }
  else
  // coming from left
  if (bx1.right > bx2.left && bx1.left < bx2.left) {
    obj1.vel.x = - Math.abs(obj1.vel.x);
    obj2.vel.x = Math.abs(obj2.vel.x);
    x= this.getLeft(obj2.sprite) - hw1;
  }
  else
  // coming from top
  if (bx1.bottom < bx2.top && bx1.top > bx2.top) {
    obj1.vel.y = Math.abs(obj1.vel.y);
    obj2.vel.y = - Math.abs(obj2.vel.y);
    y= this.getTop(obj2.sprite) + hh1;
  }
  else
  // coming from bottom
  if (bx1.top > bx2.bottom && bx2.bottom > bx1.bottom) {
    obj1.vel.y = - Math.abs(obj1.vel.y);
    obj2.vel.y = Math.abs(obj2.vel.y);
    y= this.getBottom(obj2.sprite) - hh1;
  }
  else {
    return;
  }
  obj1.updatePosition(x,y);
}

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
Menu* CCSX::TMenu(Array* items, float scale) {
  auto menu= Menu::create();
  Dictionary* obj;
  int t=0;

  for (auto it = items->begin(); it != items->end(); ++it) {
    obj = static_cast<Dictionary*>(*it);
    mi= new cc.MenuItemLabel(new cc.LabelBMFont(obj.text,
                                                obj.fontPath),
                             obj.selector || obj.cb,
                             obj.target);
    mi->setOpacity(255 * 0.9);
    mi->setScale(scale);
    mi->setTag(++t);
    menu->addChild(mi);
  }
  return menu;
}

  /**
   * Make a text label menu containing one single button.
   * @method
   * @param {Object} options
   * @return {cc.Menu}
   */
Menu* CCSX::TMenu1(Dictionary* options) {
  auto arr= Array::createWithObject(options);
  auto menu = TMenu(arr);
  if (options.anchor) { menu.setAnchorPoint(options.anchor); }
  if (options.pos) { menu.setPosition(options.pos); }
  if (options.visible === false) { menu.setVisible(false); }
  menu->alignItemsVertically();
  return menu;
}

  /**
   * Create a vertically aligned menu with graphic buttons.
   * @method
   * @param {Array} items
   * @param {Object} options
   * @return {cc.Menu}
   */
Menu* CCSX::VMenu(Array* items, Dictionary* options) {
  auto m= PMenu(true,
                items,
                hint.scale,
                hint.padding);
  if (!!hint.pos) {
    m.setPosition(hint.pos);
  }
  return m;
}

  /**
   * Create a horizontally aligned menu with graphic buttons.
   * @method
   * @param {Array} items
   * @param {Object} options
   * @return {cc.Menu}
   */
Menu* CCSX::HMenu(Array* items, Dictionary* options) {
  auto m= PMenu(false,
                items,
                hint.scale,
                hint.padding);
  if (!!hint.pos) {
    m.setPosition(hint.pos);
  }
  return m;
}

  /**
   * Create a menu with graphic buttons.
   * @method
   * @param {Boolean} vertical
   * @param {Array} items
   * @param {Number} scale
   * @param {Number} padding
   * @return {cc.Menu}
   */
Menu* CCSX::PMenu(bool vertical,  Array* items, float scale, float padding) {
  auto menu = Menu::create();
  MenuItemSprite* mi;
  Dictionary* obj;
  int t=0;

  for (auto it = items->begin(); it != items->end(); ++it) {
    obj= static_cast<Dictionary*>(*it);
    mi= new cc.MenuItemSprite(new cc.Sprite(obj.nnn),
                              new cc.Sprite(obj.sss || obj.nnn),
                              new cc.Sprite(obj.ddd || obj.nnn),
                              obj.selector || obj.cb,
                              obj.target);
    if (!!obj.color) { mi.setColor(obj.color); }
    if (!!scale) { mi.setScale(scale); }
    mi->setTag(++t);
    menu->addChild(mi);
  }

  padding = padding || 10;
  if (!vertical) {
    menu->alignItemsHorizontallyWithPadding(padding);
  } else {
    menu->alignItemsVerticallyWithPadding(padding);
  }

  return menu;
}

  /**
   * Create a single button menu.
   * @method
   * @param {Object} options
   * @return {cc.Menu}
   */
Menu CCSX::PMenu1(Dictionary* options) {
  auto arr= Array::createWithObject(options);
  auto menu = PMenu(true, arr);
  if (options.anchor) { menu.setAnchorPoint(options.anchor); }
  if (options.pos) { menu.setPosition(options.pos); }
  if (options.visible === false) { menu.setVisible(false); }
  return menu;
}

  /**
   * Create a Label.
   * @method
   * @param {Object} options
   * @return {cc.LabelBMFont}
   */
Label* xxx(Dictionary* options) {
  String* text = DictValue(options, "text",text);
  String* fnt= DictValue(options, "font", fnt);
  auto f= LabelBMFont::create(text->getCString(), fnt->getCString());
  if (options.color) { f.setColor(options.color); }
  if (options.pos) { f.setPosition(options.pos); }
  if (options.anchor) { f.setAnchorPoint(options.anchor); }
  if (options.visible === false) { f.setVisible(false); }
  f->setScale( options.scale || 1);
  f->setOpacity(0.9*255);
  return f;
}

template<typename T>
T* DictValue(Dictionary* d, const string& key, T*& dummy) {
  auto v= d->objectForKey(key);
  if (v != nullptr) {
    return static_cast<T*>(v);
  } else {
    return nullptr;
  }
}




NS_FI_END


