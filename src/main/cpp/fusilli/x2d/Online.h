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

#if !defined(__ONLINE_H__)
#define __ONLINE_H__

#include "core/Odin.h"
#include "XScene.h"
NS_ALIAS(c,cocos2d)
NS_ALIAS(s,std)
NS_ALIAS(ws, fusii::wsock)
NS_BEGIN(fusii)

class OnlineLayer;
//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Online : public XScene {
friend class OnlineLayer;
protected:

  void SetActions(c::CallFunc*, c::CallFunc* );
  void OnSessionEvent(const ws::Event&);
  void OnOdinEvent(const ws::Event&);
  void OnNetworkEvent(const ws::Event&);

  void OnPlayReq(const stdstr&, const stdstr&);
  void OnContinue();
  void OnCancel(c::Ref*);

  virtual void Decorate(OnlineLayer*);
  Online();

  c::CallFunc* yes;
  c::CallFunc* no;
  ws::WSockSS* wss;

private:

  NO__CPYASS(Online)

public:

  static Online* Create(not_null<Online*>, c::CallFunc* yes, c::CallFunc* no);
  virtual XScene* Realize();
  virtual ~Online();

  CREATE_FUNC(Online)
};



NS_END(fusii)
#endif

