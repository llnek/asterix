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

#pragma once

//////////////////////////////////////////////////////////////////////////////
//
#define CC_APPDB() cocos2d::UserDefault::getInstance()

//////////////////////////////////////////////////////////////////////////////
//
#define CC_GDS(d,f) fusii::dictVal<cocos2d::String>(d,f)->getCString()
#define CC_CSS(f) fusii::cstVal<cocos2d::String>(f)->getCString()

#define CC_GDV(t,d,f) fusii::dictVal<t>(d,f)->getValue()
#define CC_CSV(t,f) fusii::cstVal<t>(f)->getValue()

#define CC_DISPEVENT2(a,b) cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(a,b)
#define CC_DISPEVENT1(a) cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(a)

#define CC_TCAC() cocos2d::TextureCache::getInstance()
#define CC_FILER() cocos2d::FileUtils::getInstance()
#define CC_DTOR() cocos2d::Director::getInstance()
#define CC_PCAST(T) static_cast<T>(getParent())
#define CC_KEEP(x) if (x) {x->retain();}
#define CC_DROP(x) if (x) {x->release();}

#define CC_CSIZE(s) s->getBoundingBox().size
#define CC_BBOX(s) s->getBoundingBox()
#define CC_HIDE(s) s->setVisible(false)
#define CC_SHOW(s) s->setVisible(true)

#define CC_DICT() cocos2d::Dictionary::create()
#define CC_FLOAT(x) cocos2d::Float::create(x)
#define CC_BOOL(x) cocos2d::Bool::create(x)
#define CC_STR(x) cocos2d::String::create(x)
#define CC_INT(x) cocos2d::Integer::create(x)

#define CC_GNLF(t,n,f) ash::nodeFld<t>(n->head,f)
#define CC_GNF(t,n,f) ash::nodeFld<t>(n,f)

#define CC_SIZE  cocos2d::Size(0,0)
#define CC_ZPT   cocos2d::Vec2(0,0)
#define CC_NIL nullptr

//////////////////////////////////////////////////////////////////////////////
//
typedef cocos2d::EventKeyboard::KeyCode KEYCODE;
typedef cocos2d::BlendFunc BDFUNC;

typedef std::function<void ()> VOIDFN;
typedef std::string filepath;
typedef std::string MsgTopic;

//////////////////////////////////////////////////////////////////////////
//
#define STATIC_REIFY_SCENE_CTX(__TYPE__) \
static __TYPE__* reify(fusii::SCTX* x) {  \
    __TYPE__ *p = mc_new( __TYPE__ ); \
    if (NNP(p)) { \
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


#define STATIC_REIFY_SCENE(__TYPE__) \
static __TYPE__* reify() {  \
    __TYPE__ *p = mc_new( __TYPE__ ); \
    if (NNP(p) && p->init()) { \
        p->autorelease(); \
        p->onInited(); \
        return p; \
    } else { \
        delete p; \
        return nullptr; \
    } \
}

#define STATIC_REIFY_LAYER_CTX(__TYPE__) \
  static __TYPE__* reify(fusii::XScene* xs, fusii::SCTX* x, int zx = 0) {  \
    __TYPE__ *p = mc_new( __TYPE__ ); \
    if (NNP(p)) { \
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

#define STATIC_REIFY_LAYER(__TYPE__) \
static __TYPE__* reify(fusii::XScene* xs, int zx = 0) {  \
    __TYPE__ *p = mc_new( __TYPE__ ); \
    if (NNP(p) && p->initEx(xs, zx)) { \
        p->autorelease(); \
        p->onInited(); \
        return p; \
    } else { \
        delete p; \
        return nullptr; \
    } \
}







