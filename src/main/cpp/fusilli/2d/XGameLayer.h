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
#include "XLayer.h"
NS_USING(cocos2d)
NS_USING(std)
NS_BEGIN(fusilli)

enum class GameMode {
  TWO_PLAY,
  ONE_PLAY,
  NET_PLAY
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL XGameLayer : public XLayer {

public:

  virtual const string Moniker() { return "GAME"; }
  virtual void PKInput() override;

  virtual ~XGameLayer();
  XGameLayer();

  Dictionary* GetLCfg();
  bool KeyPoll(int key);

  void InitEngine();

  virtual void update(float) override;
  virtual bool IsOperational() = 0;

  const map<int,bool> Keys();

  XLayer* GetBackgd(const string&);
  XLayer* GetHUD();

  const Box4 GetEnclosureBox();
  void NewGame(const GameMode m);

  CREATE_FUNC(XGameLayer)

protected:

  void SetGameMode(const GameMode m);

  map<int,bool> keyboard;
  int level;
  void* actor;
  GameMode mode;

private:

  DISALLOW_COPYASSIGN(XGameLayer)
};





NS_END(fusilli)
#endif

