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
NS_ALIAS(cc, cocos2d)
NS_BEGIN(fusilli)


//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL YesNo : public XScene {
friend class YesNoLayer;
protected:

  void SetActions(cc::CallFunc*, cc::CallFunc*);
  void SetMsg(const s::string&);
  const s::string& GetMsg() { return msg; }
  void OnYes(cc::Ref*);
  void OnNo(cc::Ref*);

  YesNo();

  cc::CallFunc* yes;
  cc::CallFunc* no;
  s::string msg;

private:

  DISALLOW_COPYASSIGN(YesNo)

public:

  static YesNo* CreateWithActions(const s::string& msg,
      cc::CallFunc* yes, cc::CallFunc* no);

  static YesNo* Create(const s::string& msg);

  virtual XScene* Realize() override;
  virtual ~YesNo();
};


NS_END(fusilli)
#endif

