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

#include "x2d/XGameLayer.h"
#include "x2d/MainGame.h"
#include "s/Factory.h"
NS_ALIAS(s,std)
NS_ALIAS(f,fusii)
NS_BEGIN(invaders)

//////////////////////////////////////////////////////////////////////////
//
class CC_DLL GameLayer : public f::XGameLayer {
private:
  NO__CPYASS(GameLayer)
  Factory* factory;
  void InitAsh();
public:
  virtual void Reset(bool newFlag) ;
  virtual void Replay() ;
  virtual void Play(bool newFlag);

  virtual void SendMsg(const stdstr& topic, void* msg);
  virtual void OnNewGame();
  void SpawnPlayer();

  void OnPlayerKilled();
  void OnEarnScore(int);

  virtual void OnGameOver();

  virtual int GetIID() { return 2; }
  virtual f::XLayer* Realize();

  DECL_CTOR(GameLayer)
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Game : public f::MainGame {
private:
  NO__CPYASS(Game)
  bool running;
public:

  virtual XScene* Realize();
  DECL_CTOR(Game)
  virtual void Resume();
  virtual void Pause();
  virtual void Run();
  virtual bool IsRunning();

  static Game* Reify();
};


NS_END(invaders)


