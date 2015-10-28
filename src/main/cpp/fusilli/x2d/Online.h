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

NS_ALIAS(ws, fusii::odin)
NS_ALIAS(c,cocos2d)
NS_ALIAS(s,std)
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL OnlineLayer : public XLayer {
public:

  void Login(const stdstr&, const stdstr&);
  virtual XLayer* Realize();

  NO__CPYASS(OnlineLayer)
  IMPL_CTOR(OnlineLayer)
};


//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Online : public XScene {
friend class OnlineLayer;
protected:

  virtual void SetActions(c::CallFunc*, c::CallFunc* );
  virtual void OnSessionEvent(const ws::Event&);
  virtual void OnNetworkEvent(const ws::Event&);
  virtual void OnOdinEvent(const ws::Event&);

  virtual void OnPlayRequest(const stdstr&, const stdstr&);
  virtual void OnCancel(c::Ref*);

  virtual void ShowWaitOthers(OnlineLayer*) = 0;
  virtual void OnStart(const ws::Event&) = 0;
  virtual void DecoUI(OnlineLayer*) = 0;
  virtual void OnPlayReply(const ws::Event&) = 0;

  Online();

  ws::WSockSS* odin;
  c::CallFunc* yes;
  c::CallFunc* no;

  NO__CPYASS(Online)

public:

  static Online* Reify(not_null<Online*>, c::CallFunc* yes, c::CallFunc* no);

  virtual XScene* Realize();
  virtual ~Online();
};



NS_END(fusii)
#endif

