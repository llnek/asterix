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
#include "2d/XScene.h"
#include "core/fusilli.h"
NS_ALIAS(cc,cocos2d)
NS_ALIAS(s,std)
NS_BEGIN(fusilli)

//////////////////////////////////////////////////////////////////////////
//
class CC_DLL MsgBox : public XScene {
friend class MsgBoxLayer;
protected:

  const s::string& GetMsg() { return textMsg; }

  void SetAction(cc::CallFunc* cb);
  void SetMsg(const s::string&);
  void OnYes(cc::Ref*);

  cc::CallFunc* action;
  s::string textMsg;

private:

  DISALLOW_COPYASSIGN(MsgBox)
  MsgBox();

  CREATE_FUNC(MsgBox)

public:

  virtual XScene* Realize() override;

  static MsgBox* CreateWithAction(cc::CallFunc*, const s::string&);
  static MsgBox* CreateWithMsg(const s::string&);
  virtual ~MsgBox();

};



NS_END(fusilli)
#endif

