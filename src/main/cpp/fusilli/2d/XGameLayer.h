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

#if !defined(__XGAMELAYER_H__)
#define __XGAMELAYER_H__

#include "deprecated/CCDictionary.h"
#include "ash/System.h"
#include "ash/Engine.h"
#include "XLayer.h"

NS_ALIAS(c, cocos2d)
NS_ALIAS(s, std)
NS_ALIAS(a, ash)
NS_BEGIN(fusilli)


enum class GameMode {
  TWO_PLAY,
  ONE_PLAY,
  NET_PLAY
};


//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL XGameLayer : public XLayer {

protected:

  virtual void SetGameMode(const GameMode);
  virtual void OnNewGame(const GameMode);

  s::map<int,bool> keyboard;
  a::Engine* engine;
  int level;
  void* actor;
  GameMode mode;

private:

  NO__COPYASSIGN(XGameLayer)

public:

  virtual int GetIID() { return 2; }
  virtual void PKInput();

  virtual ~XGameLayer();
  XGameLayer();

  c::Dictionary* GetLCfg();
  bool KeyPoll(int key);

  virtual void InitEngine();
  virtual void InitAsh() = 0;

  virtual void update(float);
  virtual XLayer* Realize();

  const s::map<int,bool>& Keys();

  XLayer* GetBackgd(int tag= 1);
  XLayer* GetHUD(int tag=3);

  const Box4 GetEnclosureBox();
  void NewGame(const GameMode m);

};





NS_END(fusilli)
#endif

