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

#if !defined(__MSGBOX_H__)
#define __MSGBOX_H__

#include "platform/CCCommon.h"
#include "XScene.h"

NS_ALIAS(c,cocos2d)
NS_ALIAS(s,std)
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////
//
class CC_DLL MsgBox : public XScene {
protected:

  void OnYes(c::Ref*);
  NO__CPYASS(MsgBox)

  c::CallFunc* action;
  stdstr textMsg;

public:

  static MsgBox* ReifyWithMsg(not_null<MsgBox*>, const stdstr&);

  static MsgBox* ReifyWithAction(
      not_null<MsgBox*>,
      c::CallFunc*, const stdstr&);

  virtual void DecoUI(XLayer*);
  virtual XScene* Realize();

  DECL_CTOR(MsgBox)
};



NS_END(fusii)
#endif

