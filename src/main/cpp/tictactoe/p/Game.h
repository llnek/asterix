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

#if !defined(__GAME_H__)
#define __GAME_H__

#include "x2d/XGameLayer.h"
#include "x2d/MainGame.h"
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////
//
class CC_DLL GameLayer : public f::XGameLayer {
private:
  NO__CPYASS(GameLayer)
  EFactory* factory;
  void MkAsh();
public:

  virtual void SendMsg(const stdstr& topic, void* msg);
  virtual void Reset(bool newFlag) ;
  virtual void Replay() ;
  virtual void Play();
  virtual void OnGameOver();

  void OnPlayerKilled();
  void OnEarnScore(int);
  void SpawnPlayer();

  virtual int GetIID() { return 2; }
  virtual void InizGame();

  DECL_CTOR(GameLayer)
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Game : public f::MainGame {
protected:

  virtual f::XGameLayer* GetGLayer();
  NO__CPYASS(Game)

public:

  virtual f::XScene* Realize();

  IMPL_CTOR(Game)

  virtual void Stop();
  virtual void Play();
};


NS_END(tttoe)
#endif


