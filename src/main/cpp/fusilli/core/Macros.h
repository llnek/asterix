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

#pragma once
//////////////////////////////////////////////////////////////////////////////

#define CC_AUDIO() CocosDenshion::SimpleAudioEngine::getInstance()
#define CC_APPDB() cocos2d::UserDefault::getInstance()

#define CC_SCAC() cocos2d::SpriteFrameCache::getInstance()
#define CC_ACAC() cocos2d::AnimationCache::getInstance()
#define CC_TCAC() cocos2d::TextureCache::getInstance()
#define CC_FILER() cocos2d::FileUtils::getInstance()
#define CC_DTOR() cocos2d::Director::getInstance()

//////////////////////////////////////////////////////////////////////////////
//
#define CC_GDS(d,f) fusii::dictVal<cocos2d::String>(d,f)->getCString()
#define CC_GDF(d,f) fusii::dictVal<cocos2d::Float>(d,f)->getValue()
#define CC_GDI(d,f) fusii::dictVal<cocos2d::Integer>(d,f)->getValue()

#define CC_CSS(f) fusii::cstVal<cocos2d::String>(f)->getCString()

#define CC_GDV(t,d,f) fusii::dictVal<t>(d,f)->getValue()
#define CC_CSV(t,f) fusii::cstVal<t>(f)->getValue()

#define CC_EDISP() CC_DTOR()->getEventDispatcher()
#define CC_ADD_ELX(t, x) \
  CC_EDISP()->addEventListenerWithSceneGraphPriority(x,t)

#define CC_ADD_ELC(t, n, h) \
  CC_EDISP()->addEventListenerWithSceneGraphPriority( \
    cocos2d::EventListenerCustom::create(n, h), t)

#define CC_DISP_CE2(a,b) CC_EDISP()->dispatchCustomEvent(a,b)
#define CC_DISP_CE1(a) CC_EDISP()->dispatchCustomEvent(a)

#define CC_PCAST(T) static_cast<T>(getParent())
#define CC_KEEP(x) if (x) { (x)->retain();}
#define CC_DROP(x) if (x) { (x)->release();}

#define CC_RAD_TO_DEG(x) CC_RADIANS_TO_DEGREES(x)
#define CC_CSIZE(s) (s)->getBoundingBox().size
#define CC_CHT(s) CC_CSIZE(s).height
#define CC_CWH(s) CC_CSIZE(s).width
#define CC_CSZ(s) CC_CSIZE(s)

#define CC_BBOX(s) s->getBoundingBox()
#define CC_HIDE(s) do { if (s) s->setVisible(false); } while (0)
#define CC_SHOW(s) do { if (s) s->setVisible(true); } while (0)
#define CC_GCT(s,t) s->getChildByTag(t)
#define CC_POS2(n,x,y) n->setPosition((x),(y))
#define CC_POS1(n,p) n->setPosition(p)

#define CC_DICT() cocos2d::Dictionary::create()
#define CC_FLOAT(x) cocos2d::Float::create(x)
#define CC_BOOL(x) cocos2d::Bool::create(x)
#define CC_STR(x) cocos2d::String::create(x)
#define CC_INT(x) cocos2d::Integer::create(x)

#define CC_ZRT  cocos2d::Rect(0,0,0,0)
#define CC_ZSZ  cocos2d::Size(0,0)
#define CC_ZPT  cocos2d::Vec2(0,0)

#define CCT_PT cocos2d::Vec2
#define CCT_SZ cocos2d::Size
#define CCT_V2 cocos2d::Vec2
#define CCT_RT cocos2d::Rect

#define CC_NIL nullptr

#define CC_ANCHOR(n,x,y) (n)->setAnchorPoint(CCT_PT((x),(y)))
#define CC_GEC(t,e,c) (t*)((ecs::Node*)(e))->get(c)

#define HHZ(z) (z).height * 0.5
#define HWZ(z) (z).width * 0.5
#define CC_ZH(z) (z).height
#define CC_ZW(z) (z).width

//////////////////////////////////////////////////////////////////////////////
//
typedef cocos2d::EventKeyboard::KeyCode KEYCODE;
typedef cocos2d::BlendFunc BDFUNC;
typedef s_vec<cocos2d::Touch*> VecTouches;
typedef s_vec<cocos2d::Vec2> VecPts;
typedef s_vec<cocos2d::Size> VecSzs;

typedef std::function<void (cocos2d::Ref*)> BTNCB;
typedef std::string filepath;
typedef std::string MsgTopic;

//////////////////////////////////////////////////////////////////////////
//
#define __decl_create_scene_ctx(__TYPE__) \
  \
static __TYPE__* reify(fusii::SCTX* x) {  \
    __TYPE__ *p = mc_new( __TYPE__ ); \
    if (N_NIL(p)) { \
      p->setCtx(x); \
      if (p->init()) { \
          p->autorelease(); \
          p->onInited(); \
          return p; \
      } else { \
          delete p; \
          return nullptr; \
      } \
  } \
}

#define __decl_create_scene(__TYPE__) \
  \
static __TYPE__* reify() {  \
    __TYPE__ *p = mc_new( __TYPE__ ); \
    if (N_NIL(p) && p->init()) { \
        p->autorelease(); \
        p->onInited(); \
        return p; \
    } else { \
        delete p; \
        return nullptr; \
    } \
}

#define __decl_create_layer_ctx(__TYPE__) \
  \
  static __TYPE__* reify(fusii::XScene* xs, fusii::SCTX* x, int zx = 0) {  \
    __TYPE__ *p = mc_new( __TYPE__ ); \
    if (N_NIL(p)) { \
      p->setCtx(x); \
      if (p->initEx(xs, zx)) { \
          p->autorelease(); \
          p->onInited(); \
          return p; \
      } else { \
          delete p; \
          return nullptr; \
      } \
  } \
}

#define __decl_create_layer(__TYPE__) \
  \
static __TYPE__* reify(fusii::XScene* xs, int zx = 0) {  \
    __TYPE__ *p = mc_new( __TYPE__ ); \
    if (N_NIL(p) && p->initEx(xs, zx)) { \
        p->autorelease(); \
        p->onInited(); \
        return p; \
    } else { \
        delete p; \
        return nullptr; \
    } \
}







