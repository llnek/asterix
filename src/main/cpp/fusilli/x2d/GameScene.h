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

#define MGMSLIVE() fusii::GameScene::self()->isLive()
#define MGMS() fusii::GameScene::self()
#define MGML() fusii::GameScene::get()

#include "platform/CCCommon.h"
#include "core/XPool.h"
#include "core/Odin.h"
#include "core/JSON.h"
#include "XScene.h"
#include "GameLayer.h"
NS_ALIAS(ws, fusii::odin)
NS_BEGIN(fusii)


//////////////////////////////////////////////////////////////////////////////
//
enum class GMode { ONE = 1, TWO, NET, NICHTS = -1 };

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GContext : public SContext {
  GContext() { mode=GMode::ONE; SNPTR(odin) }
  GContext(GMode m) : GContext() { mode=m; }
  virtual ~GContext() { mc_del_ptr(odin) }
  ws::OdinIO* odin;
  GMode mode;
  NOCPYASS(GContext)
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL GameScene : public XScene {
protected:

  static void bind(not_null<GameScene*>);

  s_map<sstr, XPool*> pools;
  s_que<sstr> msgQ;

  int state;
  int level;

public:

  virtual GameLayer* getGLayer() = 0;
  virtual bool isLive() = 0;
  virtual void stop() = 0;
  virtual void play() = 0;

  static GameScene* self();
  static GameLayer* get();

  virtual void sendMsgEx(const MsgTopic& topic, void* msg) = 0;
  void sendMsg(const MsgTopic& topic) {
    sendMsgEx(topic, nullptr);
  }

  XPool* reifyPool(const sstr& n);
  XPool* getPool(const sstr& n);

  ws::OdinIO* wsock();
  GMode getMode();
  bool isOnline();

  c::Dictionary* getCurLevel();
  f::JsonObj* getLCfg();
  void resetPools();

  s_que<sstr>& msgQueue() { return msgQ; }

  NOCPYASS(GameScene)
  DECLCZ(GameScene)

};


NS_END(fusii)
#endif

