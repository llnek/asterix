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

#define MGMS() fusii::GameScene::self()
#define MGML() fusii::GameScene::get()

#include "platform/CCCommon.h"
#include "core/XPool.h"
#include "core/Odin.h"
#include "XScene.h"
#include "GameLayer.h"
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

  static void bind(not_null<GameScene*>);

  s::map<stdstr, XPool*> pools;
  s::queue<stdstr> msgQ;
  ws::OdinIO* odin;

  GMode mode;
  int level;

  void setOnlineChannel(ws::OdinIO* s) { odin= s; }
  virtual GameLayer* getGLayer() = 0;
  void setMode(GMode m) { mode= m; }

  NO__CPYASS(GameScene)
  GameScene();

public:

  static GameScene* reify(not_null<GameScene*>, GMode, not_null<ws::OdinIO*>);
  static GameScene* reify(not_null<GameScene*>, GMode);

  static GameScene* self();
  static GameLayer* get();

  virtual bool isOnline() { return NNP(odin); }
  virtual bool isLive() = 0;
  virtual void stop() = 0;
  virtual void play() = 0;

  virtual void sendMsg(const stdstr& topic, void* msg) = 0;
  void sendMsg(const stdstr& topic) {
    sendMsg(topic, nullptr);
  }

  XPool* reifyPool(const stdstr& n);
  XPool* getPool(const stdstr& n);

  c::Dictionary* getLCfg();
  void resetPools();

  s::queue<stdstr>& msgQueue() { return msgQ; }
  ws::OdinIO* wsock() { return odin; }

  virtual ~GameScene();
};


NS_END(fusii)
#endif

