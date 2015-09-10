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

#include "XGameLayer.h"
NS_USING(std)
NS_USING(fusilli)
NS_BEGIN(invaders)


//////////////////////////////////////////////////////////////////////////
//
class CC_DLL BGLayer : public XLayer {
public:
  virtual XLayer* Realize() {
    CenterImage(XConfig::GetInstance()->GetImage("game.bg"));
    return this;
  }
  virtual ~BGLayer();
  BGLayer();
  virtual const string Moniker() { return "BackLayer"; }
private:
  DISALLOW_COPYASSIGN(BGLayer)
};

//////////////////////////////////////////////////////////////////////////
//
class CC_DLL GameLayer : public XGameLayer {
public:

  virtual void PKInput() override;

  virtual void Reset(bool newFlag) override;
  virtual bool IsOperational() override;
  virtual void Replay() override;
  virtual void Play(bool newFlag);

  void SpawnPlayer();
  void OnPlayerKilled(msg);
  void OnNewGame(int mode);
  void OnEarnScore(msg);
  void OnDone();

  virtual ~GameLayer();
  GameLayer();

private:
  DISALLOW_COPYASSIGN(GameLayer)
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL MainGame : public XScene {
public:
  virtual XScene* Realize() override;
  virtual ~MainGame();
  MainGame();

private:
  DISALLOW_COPYASSIGN(MainGame)
};


NS_END(invaders)


