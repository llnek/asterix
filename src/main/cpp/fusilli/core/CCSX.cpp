// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Ken Leung. All rights reserved.

#include "audio/include/SimpleAudioEngine.h"
#include "cocos2d.h"
#include "XConfig.h"
#include "JSON.h"
#include "CCSX.h"

NS_ALIAS(den, CocosDenshion)
NS_BEGIN(fusii)
NS_BEGIN(ccsx)

//////////////////////////////////////////////////////////////////////////////
//
c::Menu* mkHMenu(const s_vec<c::MenuItem*> &items, float pad) {
  return mkMenu(items, false, pad);
}

//////////////////////////////////////////////////////////////////////////////
//
c::Menu* mkVMenu(const s_vec<c::MenuItem*> &items, float pad) {
  return mkMenu(items, true, pad);
}

//////////////////////////////////////////////////////////////////////////////
// items should be same size
c::Menu* mkMenu(const s_vec<c::MenuItem*> &items, bool vert, float pad) {

  auto menu= c::Menu::create();

  F__LOOP(it, items) {
    menu->addChild( *it);
  }

  if (!vert) {
    menu->alignItemsHorizontallyWithPadding(pad);
  } else {
    menu->alignItemsVerticallyWithPadding(pad);
  }

  //menu->setAnchorPoint(anchorBL());
  //menu->setPosition(0,0);
  return menu;
}

//////////////////////////////////////////////////////////////////////////////
//
c::Menu* mkMenu(c::MenuItem *item) {
  auto menu= c::Menu::create();
  menu->addChild( item);
  menu->setAnchorPoint(anchorBL());
  menu->setPosition(0,0);
  return menu;
}

//////////////////////////////////////////////////////////////////////////////
//
bool isDesktop() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
  return true;
#else
  return false;
#endif
}

//////////////////////////////////////////////////////////////////////////////
//
const c::Color3B colorRGB(const sstr &color) {
  int r=0, g=0, b=0;
  ::sscanf(color.c_str(), "#%2x%2x%2x", &r, &g, &b);
  return c::Color3B( (GLubyte)r, (GLubyte)g, (GLubyte)b);
}

//////////////////////////////////////////////////////////////////////////////
//
void sfxMusic(const sstr &music, bool repeat) {
  auto fp= XCFG()->getMusic(music);
  try {
    den::SimpleAudioEngine::getInstance()->playBackgroundMusic(fp.c_str(), repeat);
  } catch (...) {
    CCLOG("failed to play music: %s", music.c_str());
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void sfxPlay(const sstr &sound) {
  auto fp= XCFG()->getEffect(sound);
  try {
    den::SimpleAudioEngine::getInstance()->playEffect(fp.c_str());
  } catch (...) {
    CCLOG("failed to play sound: %s", sound.c_str());
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void pauseEffects() {
  den::SimpleAudioEngine::getInstance()->stopAllEffects();
}

//////////////////////////////////////////////////////////////////////////////
//
void pauseMusic() {
  den::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}

//////////////////////////////////////////////////////////////////////////////
//
void pauseAudio() {
  den::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
  den::SimpleAudioEngine::getInstance()->stopAllEffects();
}

//////////////////////////////////////////////////////////////////////////////
//
void resumeEffects() {
  den::SimpleAudioEngine::getInstance()->resumeAllEffects();
}

//////////////////////////////////////////////////////////////////////////////
//
void resumeMusic() {
  den::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

//////////////////////////////////////////////////////////////////////////////
//
void resumeAudio() {
  den::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
  den::SimpleAudioEngine::getInstance()->resumeAllEffects();
}

//////////////////////////////////////////////////////////////////////////////
//
const s_arr<c::MenuItem*,2> createAudioIcons() {
  auto n3="sound_off.png";
  auto n2="sound_on.png";
  return s_arr<c::MenuItem*,2> { createMenuBtn(n3,n3),
    createMenuBtn(n2,n2) };
}

//////////////////////////////////////////////////////////////////////////////
//
const s_arr<c::MenuItem*,2> reifyAudioIcons() {
  auto n3="sound_off.png";
  auto n2="sound_on.png";
  return s_arr<c::MenuItem*,2> { reifyMenuBtn(n3,n3),
    reifyMenuBtn(n2,n2) };
}

//////////////////////////////////////////////////////////////////////////////
//
c::MenuItem* reifyMenuBtn(const sstr &n) {
  return reifyMenuBtn(n,n);
}

//////////////////////////////////////////////////////////////////////////////
//
c::MenuItem* reifyMenuBtn(const sstr &n, const sstr &s) {
  return c::MenuItemSprite::create(reifySprite(n), reifySprite(s));
}

//////////////////////////////////////////////////////////////////////////////
//
c::MenuItem* createMenuBtn(const sstr &n) {
  return createMenuBtn(n,n);
}

//////////////////////////////////////////////////////////////////////////////
//
c::MenuItem* createMenuBtn(const sstr &n, const sstr &s) {
  return c::MenuItemImage::create(
                                  XCFG()->getImage(n), XCFG()->getImage(s));
}

//////////////////////////////////////////////////////////////////////////////
// Test if this point is inside this rectangle
bool pointInBox(const Box4 &box, float x, float y) {
  return x >= box.left && x <= box.right &&
    y >= box.bottom && y <= box.top;
}

//////////////////////////////////////////////////////////////////////////////
//
bool pointInBox(const Box4 &box, const c::Vec2& pos) {
  return pointInBox(box, pos.x, pos.y);
}

//////////////////////////////////////////////////////////////////////////
//
const c::Color3B white() { return c::Color3B::WHITE; }

//////////////////////////////////////////////////////////////////////////
//
const c::Color3B black() { return c::Color3B::BLACK; }

//////////////////////////////////////////////////////////////////////////////
//
c::Label* reifyBmfLabel(const sstr &font, const sstr &text) {
  auto f= c::Label::createWithBMFont( XCFG()->getFont(font), text);
  f->setOpacity(0.9f * 255);
  return f;
}

//////////////////////////////////////////////////////////////////////////////
//
c::Label* reifyBmfLabel(float x, float y, const sstr &font, const sstr &text) {
  auto f= reifyBmfLabel(font, text);
  f->setPosition(x,y);
  return f;
}

//////////////////////////////////////////////////////////////////////////////
//
c::Label* reifyLabel(const sstr &font, float sz, const sstr &text) {
  return c::Label::createWithTTF(text, XCFG()->getFont(font), sz);
}

//////////////////////////////////////////////////////////////////////////////
//
c::Label* reifyLabel(float x, float y,
  const sstr &font,
  float sz,
  const sstr &text) {
  auto t = reifyLabel(font, sz, text);
  t->setPosition(x,y);
  return t;
}

//////////////////////////////////////////////////////////////////////////
// Test collision of 2 entities using cc-rects
bool collide(not_null<ComObj*> a, not_null<ComObj*> b) {
  return (NNP(a) && NNP(b)) ? collideN(a->node, b->node) : false;
}

//////////////////////////////////////////////////////////////////////////
// Test collision of 2 sprites
bool collideN(not_null<c::Node*> a, not_null<c::Node*> b) {
  return (NNP(a) && NNP(b)) ? bbox(a).intersectsRect( bbox(b)) : false;
}

//////////////////////////////////////////////////////////////////////////
//
void setDevRes(float x, float y, ResolutionPolicy pcy) {
  CC_DTOR()->getOpenGLView()->setDesignResolutionSize(x, y, pcy);
}

//////////////////////////////////////////////////////////////////////////
//
bool isClicked(not_null<c::Node*> node, const c::Vec2 &tap) {
  return tap.distance(node->getPosition()) <= node->getBoundingBox().size.width * 0.8f;
}

//////////////////////////////////////////////////////////////////////////////
//
float deltaX(not_null<c::Node*> a, not_null<c::Node*> b) {
  return a->getPositionX() - b->getPositionX();
}

//////////////////////////////////////////////////////////////////////////////
//
float deltaY(not_null<c::Node*> a, not_null<c::Node*> b) {
  return a->getPositionY() - b->getPositionY();
}

//////////////////////////////////////////////////////////////////////////
//
bool isPortrait() {
  auto s=screen();
  return s.height > s.width;
}

//////////////////////////////////////////////////////////////////////////
//
c::Array* readXmlAsList(const sstr &fpath) {
  return c::Array::createWithContentsOfFile(fpath.c_str());
}

//////////////////////////////////////////////////////////////////////////
//
c::Dictionary* readXmlAsDict(const sstr &fpath) {
  return c::Dictionary::createWithContentsOfFile(fpath.c_str());
}

//////////////////////////////////////////////////////////////////////////
//
j::json readJson(const sstr &fpath) {
    auto s= c::FileUtils::getInstance()->getStringFromFile(fpath);
  return j::json::parse(s);
}

//////////////////////////////////////////////////////////////////////////
//
bool outOfBound(not_null<ComObj*> ent, const Box4 &B) {
  return (NNP(ent) && NNP(ent->node)) ? outOfBound(bbox4(ent->node), B) : false;
}

//////////////////////////////////////////////////////////////////////////
//
bool outOfBound(const Box4 &a, const Box4 &B) {
  return a.left > B.right    ||
         a.top < B.bottom  ||
         a.right < B.left      ||
         a.bottom > B.top;
}

//////////////////////////////////////////////////////////////////////////
//
const c::Size scaleSize(const c::Size &z, float scale) {
  return c::Size(z.width * scale, z.height * scale);
}

//////////////////////////////////////////////////////////////////////////
//
void undoTimer(c::DelayTime* tm) {
  if (tm) CC_DROP(tm);
}

//////////////////////////////////////////////////////////////////////////
//
void mergeDict(c::Dictionary *src, c::Dictionary *d2) {
  NS_USING(cocos2d)
  DictElement *e= nullptr;
  CCDICT_FOREACH(d2, e) {
    auto obj = e->getObject();
    auto key= e->getStrKey();
    src->setObject(obj, key);
  }
}

//////////////////////////////////////////////////////////////////////////
// Reify a timer action
//
c::DelayTime* reifyTimer(not_null<c::Node*> par, float millis) {
  auto t= c::DelayTime::create(millis/1000.0f);// in secs
  CC_KEEP(t)
  par->runAction(t);
  return t;
}

//////////////////////////////////////////////////////////////////////////
//
bool timerDone(c::DelayTime* t) {
  return t ? t->isDone() : false;
}

//////////////////////////////////////////////////////////////////////////
// Reify a sprite from its frame name
c::Sprite* reifySprite(const sstr &name) {
  return c::Sprite::createWithSpriteFrameName(name);
}

//////////////////////////////////////////////////////////////////////////
// Reify a sprite from its frame name
c::Sprite* createSprite(const sstr &name) {
  return c::Sprite::create( XCFG()->getImage(name));
}

//////////////////////////////////////////////////////////////////////////////
//
c::Sprite* loadSprite(const sstr &fname, const sstr &path) {
  auto sp= c::Sprite::create();
  sp->initWithFile(path + "/" + fname);
  return sp;
}

//////////////////////////////////////////////////////////////////////////
//
const Box4 bbox4(not_null<c::Node*> s) {
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
void prelude() {
  runEx(XCFG()->prelude());
}

//////////////////////////////////////////////////////////////////////////
//
void runEx(not_null<c::Scene*> ns) {
  run(ns, CC_CSV(c::Float, "SCENE_DELAY"));
}

//////////////////////////////////////////////////////////////////////////
//
void run(not_null<c::Scene*> ns, float delay) {
  CC_DTOR()->replaceScene(
      c::TransitionCrossFade::create(delay, ns));
}

//////////////////////////////////////////////////////////////////////////
//
void run(not_null<c::Scene*> ns) {
  CC_DTOR()->replaceScene(ns);
}

//////////////////////////////////////////////////////////////////////////
//
void pushEx(not_null<c::Scene*> ns) {
  push(ns, CC_CSV(c::Float, "SCENE_DELAY"));
}

//////////////////////////////////////////////////////////////////////////
//
void push(not_null<c::Scene*> ns, float delay) {
  CC_DTOR()->pushScene(
      c::TransitionCrossFade::create(delay, ns));
}

//////////////////////////////////////////////////////////////////////////
//
void push(not_null<c::Scene*> ns) {
  CC_DTOR()->pushScene(ns);
}

//////////////////////////////////////////////////////////////////////////
//
bool isTransitioning() {
  return dynamic_cast<c::TransitionScene*>(
      CC_DTOR()->getRunningScene()) != nullptr;
}

//////////////////////////////////////////////////////////////////////////
// Find size of this sprite
//
const c::Size calcSize(const sstr &frame) {
  return reifySprite(frame)->getContentSize();
}

//////////////////////////////////////////////////////////////////////////
// Calculate halves of width and height of this sprite
//
const c::Size halfHW(not_null<ComObj*> n) {
  return halfHW(n->node);
}

//////////////////////////////////////////////////////////////////////////
// Calculate halves of width and height of this sprite
//
const c::Size halfHW(not_null<c::Node*> s) {
  auto z= s->getContentSize();
  return c::Size(z.width * 0.5f, z.height * 0.5f);
}

//////////////////////////////////////////////////////////////////////////
// Reify a rectangle from this sprite
//
const c::Rect bbox(not_null<c::Node*> s) {
  return s->getBoundingBox();
  /*
  return c::Rect(GetLeft(s),
                 GetBottom(s),
                 GetWidth(s),
                 GetHeight(s)); */
}

//////////////////////////////////////////////////////////////////////////
// Reify a rect from the last frame
//
const Box4 bbox4B4(not_null<ComObj*> ent) {
  return Box4(
    getLastTop(ent),
    getLastRight(ent),
    getLastBottom(ent),
    getLastLeft(ent));
}

//////////////////////////////////////////////////////////////////////////
//
float getScaledHeight(not_null<c::Node*> s) {
  return s->getContentSize().height * s->getScaleY();
}

//////////////////////////////////////////////////////////////////////////
//
float getHeight(not_null<c::Node*> s) {
  return s->getContentSize().height;
}

//////////////////////////////////////////////////////////////////////////
//
float getScaledWidth(not_null<c::Node*> s) {
  return s->getContentSize().width * s->getScaleX();
}

//////////////////////////////////////////////////////////////////////////
//
float getWidth(not_null<c::Node*> s) {
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
float getLeft(not_null<c::Node*> s) {
  return get_XXX(s, s->getPosition().x, anchorL().x);
}

//////////////////////////////////////////////////////////////////////////
//
float getRight(not_null<c::Node*> s) {
  return get_XXX(s, s->getPosition().x, anchorR().x);
}

//////////////////////////////////////////////////////////////////////////
//
float getBottom(not_null<c::Node*> s) {
  return get_YYY(s, s->getPosition().y, anchorB().y);
}

//////////////////////////////////////////////////////////////////////////
//
float getTop(not_null<c::Node*> s) {
  return get_YYY(s, s->getPosition().y, anchorT().y);
}

//////////////////////////////////////////////////////////////////////////
//
float getLastLeft(not_null<ComObj*> ent) {
  return get_XXX(ent->node, ent->lastPos.x, anchorL().x);
}

//////////////////////////////////////////////////////////////////////////
//
float getLastRight(not_null<ComObj*> ent) {
  return get_XXX(ent->node, ent->lastPos.x, anchorR().x);
}

//////////////////////////////////////////////////////////////////////////
//
float getLastTop(not_null<ComObj*> ent) {
  return get_YYY(ent->node, ent->lastPos.y, anchorT().y);
}

//////////////////////////////////////////////////////////////////////////
//
float getLastBottom(not_null<ComObj*> ent) {
  return get_YYY(ent->node, ent->lastPos.y, anchorB().y);
}

//////////////////////////////////////////////////////////////////////////
// Get the x pos of the center of the visible screen
//
float centerX() { return center().x; }

//////////////////////////////////////////////////////////////////////////
// Get the y pos of the center of the visible screen.
//
float centerY() { return center().y; }

//////////////////////////////////////////////////////////////////////////
// Get the center of the visible screen
//
const c::Vec2 center() {
  auto rc = visRect();
  return c::Vec2(rc.origin.x + HWZ(rc.size), rc.origin.y + HHZ(rc.size));
}

//////////////////////////////////////////////////////////////////////////
// Get the screen height
//
float screenHeight() { return screen().height; }

//////////////////////////////////////////////////////////////////////////
// Get the screen width
//
float screenWidth() { return screen().width; }

//////////////////////////////////////////////////////////////////////////
// Get the visible screen rectangle
//
const c::Rect visRect() {
  return CC_DTOR()->getOpenGLView()->getVisibleRect();
}

//////////////////////////////////////////////////////////////////////////
// Get the visible screen box
//
const Box4 visBox() {
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
const c::Size screen() {
  return CC_DTOR()->getOpenGLView()->getFrameSize();
}

//////////////////////////////////////////////////////////////////////////
// Get the actual screen center.
//
const c::Vec2 scenter() {
  auto sz = screen();
  return c::Vec2(HWZ(sz), HHZ(sz));
}

//////////////////////////////////////////////////////////////////////////
// Get the center of this box.
//
const c::Vec2 vboxMID(const Box4 &box) {
  return c::Vec2(box.left + (box.right-box.left) * 0.5f,
              box.bottom + (box.top-box.bottom) * 0.5f);
}

//////////////////////////////////////////////////////////////////////////
// Test if this box is hitting boundaries
// rect.x & y are center positioned
//
// If hit, the new position and velocities
// are returned
//
bool traceEnclosure(float dt, const Box4 &bbox,
    const Box4 &rect, const c::Vec2 &vel,
    c::Vec2 &outPos, c::Vec2 &outVel) {

  auto sz= rect.top-rect.bottom;//height
  auto sw= rect.right-rect.left;//width
  auto y = rect.bottom+(sz*0.5f) + dt * vel.y;// new y
  auto x = rect.left+(sw*0.5f) + dt * vel.x;// new x
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

//////////////////////////////////////////////////////////////////////////////
//
bool isIntersect(const Box4 &a1, const Box4 &a2) {
  return ! (a1.left > a2.right ||
            a2.left > a1.right ||
            a1.top < a2.bottom ||
            a2.top < a1.bottom);
}

//////////////////////////////////////////////////////////////////////////
// Get the sprite from the frame cache using its id (e.g. #ship)
//
c::SpriteFrame* getSpriteFrame(const sstr &frameid) {
  return c::SpriteFrameCache::getInstance()->getSpriteFrameByName(frameid);
}

//////////////////////////////////////////////////////////////////////////
//
const c::Vec2 anchorC() { return c::Vec2(0.5, 0.5); }

//////////////////////////////////////////////////////////////////////////
//
const c::Vec2 anchorT() { return c::Vec2(0.5, 1); }

//////////////////////////////////////////////////////////////////////////
//
const c::Vec2 anchorTR() { return c::Vec2(1, 1); }

//////////////////////////////////////////////////////////////////////////
//
const c::Vec2 anchorR() { return c::Vec2(1, 0.5); }

//////////////////////////////////////////////////////////////////////////
//
const c::Vec2 anchorBR() { return c::Vec2(1, 0); }

//////////////////////////////////////////////////////////////////////////
//
const c::Vec2 anchorB() { return c::Vec2(0.5, 0); }

//////////////////////////////////////////////////////////////////////////
//
const c::Vec2 anchorBL() { return c::Vec2(0, 0); }

//////////////////////////////////////////////////////////////////////////
//
const c::Vec2 anchorL() { return c::Vec2(0, 0.5); }

//////////////////////////////////////////////////////////////////////////
//
const c::Vec2 anchorTL() { return c::Vec2(0, 1); }

//////////////////////////////////////////////////////////////////////////
// not used for now.
//
void resolveElastic(not_null<ComObj*> obj1, not_null<ComObj*> obj2) {
  auto pos2 = obj2->node->getPosition();
  auto pos1= obj1->node->getPosition();
  auto sz2= obj2->node->getContentSize();
  auto sz1= obj1->node->getContentSize();
  auto hh1= HHZ(sz1);
  auto hw1= HWZ(sz1);
  auto x = pos1.x;
  auto y= pos1.y;
  auto bx2 = bbox4(obj2->node);
  auto bx1 = bbox4(obj1->node);

  // coming from right
  if (bx1.left < bx2.right && bx2.right < bx1.right) {
    obj2->vel.x = - fabs(obj2->vel.x);
    obj1->vel.x = fabs(obj1->vel.x);
    x= getRight(obj2->node) + hw1;
  }
  else
  // coming from left
  if (bx1.right > bx2.left && bx1.left < bx2.left) {
    obj1->vel.x = - fabs( obj1->vel.x);
    obj2->vel.x = fabs(obj2->vel.x);
    x= getLeft(obj2->node) - hw1;
  }
  else
  // coming from top
  if (bx1.bottom < bx2.top && bx1.top > bx2.top) {
    obj2->vel.y = - fabs(obj2->vel.y);
    obj1->vel.y = fabs(obj1->vel.y);
    y= getTop(obj2->node) + hh1;
  }
  else
  // coming from bottom
  if (bx1.top > bx2.bottom && bx2.bottom > bx1.bottom) {
    obj1->vel.y = - fabs(obj1->vel.y);
    obj2->vel.y = fabs(obj2->vel.y);
    y= getBottom(obj2->node) - hh1;
  }
  else {
    return;
  }
  obj1->updatePosition(x,y);
}

//////////////////////////////////////////////////////////////////////////////
//
const c::Vec2 clamp(const c::Vec2& loc, const c::Size &sz, const Box4 &world) {
  auto hh = HHZ(sz);
  auto hw = HWZ(sz);
  auto x= loc.x;
  auto y= loc.y;

  if ((loc.x + hw) > world.right) {
    x = world.right - hw;
  }
  if ((loc.x - hw) < world.left) {
    x = world.left + hw;
  }
  if ((loc.y - hh) < world.bottom) {
    y = world.bottom + hh;
  }
  if ((loc.y + hh) > world.top) {
    y = world.top - hh;
  }

  return c::Vec2(x,y);
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
const c::Vec2 clamp(const c::Vec2 &cur, const Box4 &bx) {
  return ccpClamp(cur, c::ccp(bx.left, bx.bottom), c::ccp(bx.right, bx.top));
}

//////////////////////////////////////////////////////////////////////////////
//
int randInt(int upper) {
  return (int) floor(cocos2d::rand_0_1() * upper);
}

//////////////////////////////////////////////////////////////////////////////
//
float randFloat(float upper) {
  return cocos2d::rand_0_1() * upper;
}

//////////////////////////////////////////////////////////////////////////////
//
int randSign() {
  int n=  cocos2d::rand_0_1() * 10;
  switch (n) {
    case 0: case 2: case 4: case 6: case 8: return 1;
    default: return -1;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
float rand() {
  return cocos2d::rand_0_1();
}

//////////////////////////////////////////////////////////////////////////////
//
float degToRad(float deg) {
  return deg * M_PI / 180;
}

//////////////////////////////////////////////////////////////////////////////
//
long long currentTimeInMillis() {
  return c::utils::getTimeInMilliseconds();
}

//////////////////////////////////////////////////////////////////////////////
//
const c::Vec2 calcXY(float angle, float hypot) {
  // quadrants =  4 | 1
  //             --------
  //              3 | 2
  float theta, q, x, y;
  if (angle >= 0 && angle <= 90) {
    theta = degToRad(90 - angle);
    x = cos(theta);
    y = sin(theta);
    q=1;
  }
  else
  if (angle >= 90 && angle <= 180 ) {
    theta = degToRad(angle - 90);
    x = cos(theta);
    y =  - sin(theta);
    q=2;
  }
  else
  if (angle >= 180 && angle <= 270) {
    theta = degToRad(270 - angle);
    x = - cos(theta);
    y = - sin(theta);
    q=3;
  }
  else
  if (angle >= 270 && angle <= 360) {
    theta= degToRad(angle - 270);
    x = - cos(theta);
    y = sin(theta);
    q=4;
  }
  else {
  }

  return c::Vec2( x * hypot, y * hypot);
}

//////////////////////////////////////////////////////////////////////////////
//
VOIDFN throttle(VOIDFN func, int wait) {
  long long ww= (long long)wait;
  long long previous = 0;
  return [=]() mutable {
    auto now = currentTimeInMillis();
    if (previous==0)
    { previous = now; }
    auto remaining = ww - (now - previous);
    if (remaining <= 0 || remaining > ww) {
      previous = now;
      func();
    }
  };
}


NS_END
NS_END


