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

#if !defined(__CONFIG_H__)
#define __CONFIG_H__

#include "core/XConfig.h"
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Config : public f::XConfig {
protected:

  void InitAssets();
  void InitCsts();

  NO__CPYASS(Config)
  Config();
  float scale;

public:

  virtual const stdstr AppId() { return "tictactoe"; }
  virtual const stdstr Color() { return "white"; }

  virtual const stdstr AppKey() {
    return  "bd5f79bb-eb41-4ed5-bb44-2529dc27ed3c";
  }

  virtual ResolutionPolicy Policy() {
    return ResolutionPolicy::FIXED_HEIGHT;
  }

  virtual const c::Size GameSize() {
    return c::Size(320,480);
  }

  virtual void HandleResolution(const c::Size& rs);
  virtual const stdstr GetWSUrl();
  virtual c::Scene* StartWith();

  virtual void SetGameId(const stdstr& );
  virtual void SetRoomId(const stdstr& );

  virtual const stdstr GetGameId();
  virtual const stdstr GetRoomId();

  virtual void RunOnce();
  virtual ~Config() {}

  static owner<f::XConfig*> Reify();
};



NS_END(tttoe)
#endif

