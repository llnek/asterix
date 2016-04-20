// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Kenneth Leung. All rights reserved.

#include "cocos2d.h"
#include "XConfig.h"
#include "JSON.h"
#include "CCSX.h"

NS_BEGIN(fusii)
NS_BEGIN(ccsx)

//////////////////////////////////////////////////////////////////////////////
//
void randomPos(not_null<c::Node*> node) {
  auto wb=visBox();
  node->setPosition(
        randInt(wb.right),
        randInt(wb.top));
}

//////////////////////////////////////////////////////////////////////////////
//
c::Menu* mkHMenu(const s_vec<c::MenuItem*> &v, float pad) {
  return mkMenu(v, false, pad);
}

//////////////////////////////////////////////////////////////////////////////
//
c::Menu* mkVMenu(const s_vec<c::MenuItem*> &v, float pad) {
  return mkMenu(v, true, pad);
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
  menu->addChild(item);
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
    CC_AUDIO()->playBackgroundMusic(fp.c_str(), repeat);
  } catch (...) {
    CCLOG("failed to play music: %s", music.c_str());
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void sfxPlay(const sstr &sound) {
  auto fp= XCFG()->getEffect(sound);
  try {
    CC_AUDIO()->playEffect(fp.c_str());
  } catch (...) {
    CCLOG("failed to play sound: %s", sound.c_str());
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void pauseEffects() {
  CC_AUDIO()->pauseAllEffects();
}

//////////////////////////////////////////////////////////////////////////////
//
void pauseMusic() {
  CC_AUDIO()->pauseBackgroundMusic();
}

//////////////////////////////////////////////////////////////////////////////
//
void pauseAudio() {
  pauseEffects();
  pauseMusic();
}

//////////////////////////////////////////////////////////////////////////////
//
void resumeEffects() {
  CC_AUDIO()->resumeAllEffects();
}

//////////////////////////////////////////////////////////////////////////////
//
void resumeMusic() {
  CC_AUDIO()->resumeBackgroundMusic();
}

//////////////////////////////////////////////////////////////////////////////
//
void resumeAudio() {
  resumeEffects();
  resumeMusic();
}

//////////////////////////////////////////////////////////////////////////////
//
void stopEffects() {
  CC_AUDIO()->stopAllEffects();
}

//////////////////////////////////////////////////////////////////////////////
//
void stopMusic() {
  CC_AUDIO()->stopBackgroundMusic();
}

//////////////////////////////////////////////////////////////////////////////
//
void stopAudio() {
  stopEffects();
  stopMusic();
}

//////////////////////////////////////////////////////////////////////////////
//
const CCT_SZ calcOffsets(const CCT_SZ &sz) {
  return CCT_SZ(sz.width/GOLDEN_RATIO, sz.height/GOLDEN_RATIO);
}

//////////////////////////////////////////////////////////////////////////////
//
c::SpriteBatchNode* reifySpriteBatch(const sstr &name) {
  return c::SpriteBatchNode::createWithTexture( CC_TCAC()->addImage( XCFG()->getImage(name)));
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
float calcDist(not_null<c::Node*> a, not_null<c::Node*> b) {
  return c::ccpDistance(a->getPosition(), b->getPosition());
}

//////////////////////////////////////////////////////////////////////////////
//
const CCT_PT normalize(not_null<c::Node*> a, not_null<c::Node*> b) {
  return c::ccpNormalize(
          c::ccpSub(b->getPosition(), a->getPosition()));
}

//////////////////////////////////////////////////////////////////////////////
//
c::MenuItem* reifyMenuText(const sstr &fnt, float pt, const sstr &txt, const c::Color3B &c) {
  auto z= reifyLabel(fnt, pt, txt);
    z->setColor(c);
  return c::MenuItemLabel::create(z);
}

//////////////////////////////////////////////////////////////////////////////
//
c::MenuItem* reifyMenuText(const sstr &fnt, const sstr &txt, const c::Color3B &c) {
  auto z= reifyBmfLabel(fnt,txt);
    z->setColor(c);
  return c::MenuItemLabel::create(z);
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
bool pointInBox(const Box4 &box, const CCT_PT& pos) {
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
  f->setOpacity(0.9 * 255);
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
// Test collision of 2 sprites
bool collide(not_null<c::Node*> a, not_null<c::Node*> b) {
  return (N_NIL(a) && N_NIL(b)) ? bbox(a).intersectsRect( bbox(b)) : false;
}

//////////////////////////////////////////////////////////////////////////
//
void setDevRes(float x, float y, ResolutionPolicy pcy) {
  CC_DTOR()->getOpenGLView()->setDesignResolutionSize(x, y, pcy);
}

//////////////////////////////////////////////////////////////////////////
//
bool isTapped(not_null<c::Node*> node, const CCT_PT &tap) {
  return tap.distance(node->getPosition()) <= node->getBoundingBox().size.width * 0.8;
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
  auto s= CC_FILER()->getStringFromFile(fpath);
  return j::json::parse(s);
}

//////////////////////////////////////////////////////////////////////////
//
bool outOfBound(not_null<c::Node*> ent, const Box4 &B) {
  return N_NIL(ent) ? outOfBound(bbox4(ent), B) : false;
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
const CCT_SZ scaleSize(const CCT_SZ &z, float scale) {
  return CCT_SZ(z.width * scale, z.height * scale);
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
  DictElement *e= CC_NIL;
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

//////////////////////////////////////////////////////////////////////////////
//
owner<c::Animation*> createAnimation(float dpu, bool restore, int loops) {

  auto a= c::Animation::create();
  a->setRestoreOriginalFrame(restore);
  a->setDelayPerUnit(dpu);
  a->setLoops(loops);
  return a;
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
      CC_DTOR()->getRunningScene()) != CC_NIL;
}

//////////////////////////////////////////////////////////////////////////
// Find size of this sprite
//
const CCT_SZ calcSize(const sstr &frame) {
  //return reifySprite(frame)->getContentSize();
  return reifySprite(frame)->boundingBox().size;
}

//////////////////////////////////////////////////////////////////////////
// Calculate halves of width and height of this sprite
//
const CCT_SZ halfHW(not_null<c::Node*> s) {
  //auto z= s->getContentSize();
  auto z= s->getBoundingBox().size;
  return CCT_SZ(z.width * 0.5, z.height * 0.5);
}

//////////////////////////////////////////////////////////////////////////
// Reify a rectangle from this sprite
//
const CCT_RT bbox(not_null<c::Node*> s) {
  return s->getBoundingBox();
  /*
  return CCT_RT(GetLeft(s),
                 GetBottom(s),
                 GetWidth(s),
                 GetHeight(s)); */
}

//////////////////////////////////////////////////////////////////////////
//
float getHeight(not_null<c::Node*> s) {
  //return s->getContentSize().height * s->getScaleY();
  return s->getBoundingBox().size.height;
}

//////////////////////////////////////////////////////////////////////////
//
float getWidth(not_null<c::Node*> s) {
  //return s->getContentSize().width * s->getScaleX();
  return s->getBoundingBox().size.width;
}

//////////////////////////////////////////////////////////////////////////
//
float getContentHeight(not_null<c::Node*> s) {
  return s->getContentSize().height;
}

//////////////////////////////////////////////////////////////////////////
//
float getContentWidth(not_null<c::Node*> s) {
  return s->getContentSize().width;
}

//////////////////////////////////////////////////////////////////////////
//
static float get_XXX(not_null<c::Node*> s, float px, float bound) {
  //auto w= s->getContentSize().width;
  auto w= s->getBoundingBox().size.width;
  auto a= s->getAnchorPoint().x;
  return px + (bound - a) * w ;
}

//////////////////////////////////////////////////////////////////////////
//
static float get_YYY(not_null<c::Node*> s, float py, float bound) {
  //auto h= s->getContentSize().height;
  auto h= s->getBoundingBox().size.height;
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
const CCT_PT center() {
  auto rc = visRect();
  return CCT_PT(rc.origin.x + HWZ(rc.size), rc.origin.y + HHZ(rc.size));
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
const CCT_RT visRect() {
  return CC_DTOR()->getOpenGLView()->getVisibleRect();
}

//////////////////////////////////////////////////////////////////////////////
//
const CCT_SZ visSize() {
  return CC_DTOR()->getOpenGLView()->getVisibleRect().size;
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
const CCT_SZ screen() {
  return CC_DTOR()->getOpenGLView()->getFrameSize();
}

//////////////////////////////////////////////////////////////////////////
// Get the actual screen center.
//
const CCT_PT scenter() {
  auto sz = screen();
  return CCT_PT(HWZ(sz), HHZ(sz));
}

//////////////////////////////////////////////////////////////////////////
// Get the center of this box.
//
const CCT_PT vboxMID(const Box4 &box) {
  return CCT_PT(box.left + (box.right-box.left) * 0.5,
              box.bottom + (box.top-box.bottom) * 0.5);
}

//////////////////////////////////////////////////////////////////////////
// Test if this box is hitting boundaries
// rect.x & y are center positioned
//
// If hit, the new position and velocities
// are returned
//
bool traceEnclosure(float dt, const Box4 &bbox,
    const Box4 &rect, const CCT_V2 &vel,
    CCT_PT &outPos, CCT_V2 &outVel) {

  auto sz= rect.top-rect.bottom;//height
  auto sw= rect.right-rect.left;//width
  auto y = rect.bottom+(sz*0.5) + dt * vel.y;// new y
  auto x = rect.left+(sw*0.5) + dt * vel.x;// new x
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
c::SpriteFrame* getSpriteFrame(const sstr &frameid, const CCT_RT &r) {
  auto f= CC_SCAC()->getSpriteFrameByName(frameid);
  f->setRect(r);
  return f;
}

//////////////////////////////////////////////////////////////////////////
// Get the sprite from the frame cache using its id (e.g. #ship)
//
c::SpriteFrame* getSpriteFrame(const sstr &frameid) {
  return CC_SCAC()->getSpriteFrameByName(frameid);
}

//////////////////////////////////////////////////////////////////////////
//
const CCT_PT anchorC() { return CCT_PT::ANCHOR_MIDDLE;}

//////////////////////////////////////////////////////////////////////////
//
const CCT_PT anchorT() { return CCT_PT::ANCHOR_MIDDLE_TOP;}

//////////////////////////////////////////////////////////////////////////
//
const CCT_PT anchorTR() { return CCT_PT::ANCHOR_TOP_RIGHT; }

//////////////////////////////////////////////////////////////////////////
//
const CCT_PT anchorR() { return CCT_PT::ANCHOR_MIDDLE_RIGHT;}

//////////////////////////////////////////////////////////////////////////
//
const CCT_PT anchorBR() { return CCT_PT::ANCHOR_BOTTOM_RIGHT; }

//////////////////////////////////////////////////////////////////////////
//
const CCT_PT anchorB() { return CCT_PT::ANCHOR_MIDDLE_BOTTOM; }

//////////////////////////////////////////////////////////////////////////
//
const CCT_PT anchorBL() { return CCT_PT::ANCHOR_BOTTOM_LEFT; }

//////////////////////////////////////////////////////////////////////////
//
const CCT_PT anchorL() { return CCT_PT::ANCHOR_MIDDLE_LEFT; }

//////////////////////////////////////////////////////////////////////////
//
const CCT_PT anchorTL() { return CCT_PT::ANCHOR_TOP_LEFT; }

//////////////////////////////////////////////////////////////////////////
// not used for now.
//
void resolveElastic(
    not_null<c::Node*> obj1,
    CCT_V2 &vel1,
    not_null<c::Node*> obj2, CCT_V2 &vel2) {

  auto pos2 = obj2->getPosition();
  auto pos1= obj1->getPosition();
  auto sz2= CC_CSIZE(obj2);
  auto sz1= CC_CSIZE(obj1);
  auto hh1= HHZ(sz1);
  auto hw1= HWZ(sz1);
  auto x = pos1.x;
  auto y= pos1.y;
  auto bx2 = bbox4(obj2);
  auto bx1 = bbox4(obj1);

  // coming from right
  if (bx1.left < bx2.right && bx2.right < bx1.right) {
    vel2.x = - fabs(vel2.x);
    vel1.x = fabs(vel1.x);
    x= getRight(obj2) + hw1;
  }
  else
  // coming from left
  if (bx1.right > bx2.left && bx1.left < bx2.left) {
    vel1.x = - fabs(vel1.x);
    vel2.x = fabs(vel2.x);
    x= getLeft(obj2) - hw1;
  }
  else
  // coming from top
  if (bx1.bottom < bx2.top && bx1.top > bx2.top) {
    vel2.y = - fabs(vel2.y);
    vel1.y = fabs(vel1.y);
    y= getTop(obj2) + hh1;
  }
  else
  // coming from bottom
  if (bx1.top > bx2.bottom && bx2.bottom > bx1.bottom) {
    vel1.y = - fabs(vel1.y);
    vel2.y = fabs(vel2.y);
    y= getBottom(obj2) - hh1;
  }
  else {
    return;
  }
  //obj1->updatePosition(x,y);
}

//////////////////////////////////////////////////////////////////////////////
//
const CCT_PT clamp(const CCT_PT& loc, const CCT_SZ &sz, const Box4 &world) {
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

  return CCT_PT(x,y);
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
const CCT_PT clamp(const CCT_PT &cur, const Box4 &bx) {
  return ccpClamp(cur, c::ccp(bx.left, bx.bottom), c::ccp(bx.right, bx.top));
}

//////////////////////////////////////////////////////////////////////////////
//
int randInt(int upper) {
    //auto r=cocos2d::rand_0_1();
    //CCLOG("rand01 ===== %f", r);
  return (int) floor( cocos2d::rand_0_1() * upper);
}

//////////////////////////////////////////////////////////////////////////////
//
float randFloat(float upper) {
  return cocos2d::rand_0_1() * upper;
}

//////////////////////////////////////////////////////////////////////////////
//
int randSign() {
    return (::rand() % 2) == 1 ? 1 : -1;
  /*
  int n=  cocos2d::rand_0_1() * 10;
  switch (n) {
    case 0: case 2: case 4: case 6: case 8: return 1;
    default: return -1;
  }
  */
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
long long timeInMillis() {
  return c::utils::getTimeInMilliseconds();
}

//////////////////////////////////////////////////////////////////////////////
//
const CCT_PT calcXY(float angle, float hypot) {
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

  return CCT_PT( x * hypot, y * hypot);
}

//////////////////////////////////////////////////////////////////////////////
//
VOIDFN throttle(VOIDFN func, int wait) {
  long long ww= (long long)wait;
  long long previous = 0;
  return [=]() mutable {
    auto now = timeInMillis();
    if (previous==0)
    { previous = now; }
    auto remaining = ww - (now - previous);
    if (remaining <= 0 || remaining > ww) {
      previous = now;
      func();
    }
  };
}

//////////////////////////////////////////////////////////////////////////////
//
void testCollide(not_null<f::FPool*> p1, not_null<ecs::Node*> node) {
  if (node->status())
    p1->foreach([=](f::Poolable* _p1) {
      auto e1= (ecs::Node*) _p1;
      testCollide(e1,node);
    });
}

//////////////////////////////////////////////////////////////////////////////
//
void testCollide(not_null<f::FPool*> p1, not_null<f::FPool*> p2) {
  p1->foreach([=](f::Poolable* _p1) {
  p2->foreach([=](f::Poolable* _p2) {
    auto e2= (ecs::Node*) _p2;
    auto e1= (ecs::Node*) _p1;
    testCollide(e1,e2);
  });
  });
}

//////////////////////////////////////////////////////////////////////////////
//
void testCollide(not_null<ecs::Node*> e1, not_null<ecs::Node*> e2) {
  auto s2= CC_GEC(f::CPixie,e2.get(),"f/CPixie");
  auto s1= CC_GEC(f::CPixie,e1.get(),"f/CPixie");
  if (e2->status() &&
      e1->status() &&
      collide(s2,s1)) {
    auto h2= CC_GEC(f::CHealth,e2.get(),"f/CHealth");
    auto h1= CC_GEC(f::CHealth,e1.get(),"f/CHealth");
    h2->hurt();
    h1->hurt();
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void hibernate(not_null<ecs::Node*> node) {
  auto s=CC_GEC(f::CPixie,node.get(),"f/CPixie");
  if (s) { s->deflate(); }
  node->yield();
}

//////////////////////////////////////////////////////////////////////////////
//
void resurrect(not_null<ecs::Node*> node, float x, float y) {
  auto h=CC_GEC(f::CHealth,node.get(),"f/CHealth");
  auto s=CC_GEC(f::CPixie,node.get(),"f/CPixie");
  if (s) { s->inflate(x,y); }
  if (h) { h->reset(); }
  node->take();
}

//////////////////////////////////////////////////////////////////////////////
//
void kumakaze(not_null<ecs::Node*> node) {
  auto h=CC_GEC(f::CHealth,node.get(),"f/CHealth");
  if (h) {
    h->curHP = 0;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void resurrect(not_null<ecs::Node*> node) {
  auto h=CC_GEC(f::CHealth,node.get(),"f/CHealth");
  auto s=CC_GEC(f::CPixie,node.get(),"f/CPixie");
  if (s) { s->inflate(); }
  if (h) { h->reset(); }
  node->take();
}

//////////////////////////////////////////////////////////////////////////////
//
void resolveNodes(not_null<f::FPool*> pool) {
  pool->foreach([=](f::Poolable *p) {
    if (p->status()) {
      auto ht=CC_GEC(f::CHealth,p,"f/CHealth");
      if (N_NIL(ht) &&
          !ht->alive())
        hibernate(PCAST(ecs::Node,p));
    }
  });
}


NS_END
NS_END


