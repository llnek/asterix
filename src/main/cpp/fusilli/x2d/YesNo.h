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

#if !defined(__YESNO_H__)
#define __YESNO_H__


#include "platform/CCCommon.h"
#include "2d/CCActionInstant.h"
#include "XScene.h"

NS_ALIAS(c, cocos2d)
NS_BEGIN(fusii)


//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL YesNo : public XScene {
friend class YesNoLayer;
protected:

  void SetActions(not_null<c::CallFunc*>,
      not_null<c::CallFunc*>);

  const stdstr& GetMsg() { return msgText; }
  void SetMsg(const stdstr&);

  void OnYes(c::Ref*);
  void OnNo(c::Ref*);

  YesNo();

  c::CallFunc* yes;
  c::CallFunc* no;
  stdstr msgText;

  CREATE_FUNC(YesNo)

private:

  NO__CPYASS(YesNo)

public:

  static owner<YesNo*> CreateWithActions(const stdstr& msg,
      not_null<c::CallFunc*> yes,
      not_null<c::CallFunc*> no);

  static owner<YesNo*> Create(const stdstr& msg);

  virtual XScene* Realize();
  virtual ~YesNo();
};


NS_END(fusii)
#endif

