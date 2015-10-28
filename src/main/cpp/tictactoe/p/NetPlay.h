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

#if !defined(__NETPLAY_H__)
#define __NETPLAY_H__

#include "x2d/Online.h"

NS_ALIAS(ws, fusii::odin)
NS_ALIAS(f, fusii)
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL NetPlay : public f::Online {
protected:

  virtual void OnPlayReply(const ws::Event&);
  virtual void OnStart(const ws::Event&);

  int player;

  NO__CPYASS(NetPlay)

public:

  virtual void ShowWaitOthers(f::OnlineLayer*);
  virtual void DecoUI(f::OnlineLayer*);

  DECL_CTOR(NetPlay)
};



NS_END(tttoe)
#endif

