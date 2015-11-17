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

#if !defined(__GAMESCENE_H__)
#define __GAMESCENE_H__

#define MGMS() fusii::GameScene::Self()
#define MGML() fusii::GameScene::Get()

#include "platform/CCCommon.h"
#include "core/XPool.h"
#include "core/Odin.h"
#include "XScene.h"
#include "XGameLayer.h"
NS_ALIAS(ws, fusii::odin)
NS_BEGIN(fusii)

enum class GMode {
  ONE,
  TWO,
  NET
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL GameScene : public XScene {
protected:

  static void Bind(not_null<GameScene*>);

  s::map<stdstr, XPool*> pools;
  s::queue<stdstr> msgQ;
  ws::OdinIO* odin;

  GMode mode;
  int level;

  virtual XGameLayer* GetGLayer() = 0;
  virtual void SetMode(GMode);

  NO__CPYASS(GameScene)
  GameScene();

public:

  void SetOnlineChannel(owner<ws::OdinIO*> s) { odin= s; }

  static GameScene* Reify(not_null<GameScene*>, GMode);
  static XGameLayer* Get();
  static GameScene* Self();

  virtual bool IsOnline() { return NNP(odin); }
  virtual bool IsLive() = 0;
  virtual void Stop() = 0;
  virtual void Play() = 0;

  XPool* ReifyPool(const stdstr& n);
  XPool* GetPool(const stdstr& n);

  c::Dictionary* GetLCfg();
  void ResetPools();

  s::queue<stdstr>& MsgQueue() { return msgQ; }
  ws::OdinIO* WSOCK() { return odin; }

  virtual ~GameScene();
};


NS_END(fusii)
#endif

