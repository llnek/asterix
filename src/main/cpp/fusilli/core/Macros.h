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

#if !defined(__MACROS_H__)
#define __MACROS_H__

#define CC_GDS(d,f) fusii::DictVal<cocos2d::String>(d,f)->getCString()
#define CC_CSS(f) fusii::CstVal<cocos2d::String>(f)->getCString()

#define CC_LOOP(x,c) for (auto x=c.begin(); x != c.end(); ++x)
#define CC_PAIR(T1,T2,v1,v2) std::pair<T1,T2>(v1,v2)

#define CC_GDV(t,d,f) fusii::DictVal<t>(d,f)->getValue()
#define CC_CSV(t,f) fusii::CstVal<t>(f)->getValue()

#define CC_DTOR() cocos2d::Director::getInstance()
#define CC_PCAST(T) static_cast<T>(getParent())
#define CC_KEEP(x) if (x) {x->retain();}
#define CC_DROP(x) if (x) {x->release();}

#define CC_FLOAT(x) cocos2d::Float::create(x)
#define CC_BOOL(x) cocos2d::Bool::create(x)
#define CC_STR(x) cocos2d::String::create(x)
#define CC_INT(x) cocos2d::Integer::create(x)
#define CC_DICT() cocos2d::Dictionary::create()

#define CC_GNF(t,n,f) ash::NodeFld<t>(n,f)

typedef std::function<void ()> VOIDFN;
typedef std::string filepath;

#define NULL_INT 977814444


#endif

