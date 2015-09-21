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
#include "s/Factory.h"
NS_ALIAS(s,std)
NS_ALIAS(f,fusilli)
NS_BEGIN(invaders)


//////////////////////////////////////////////////////////////////////////
//
class CC_DLL BGLayer : public f::XLayer {
private:
  NO__COPYASSIGN(BGLayer)
public:
  virtual f::XLayer* Realize() {
    CenterImage(f::XConfig::GetInstance()->GetImage("game.bg"));
    return this;
  }
  virtual ~BGLayer();
  BGLayer();
  virtual int GetIID() { return 1; }
  CREATE_FUNC(BGLayer)
};

//////////////////////////////////////////////////////////////////////////
//
class CC_DLL GameLayer : public f::XGameLayer {
private:
  NO__COPYASSIGN(GameLayer)
  Factory* factory;
  void InitAsh();
public:
  virtual void Reset(bool newFlag) ;
  virtual void Replay() ;
  virtual void Play(bool newFlag);

  virtual void OnNewGame(const f::GMode);
  void SpawnPlayer();
  void OnPlayerKilled();

  void OnEarnScore(int);
  void OnDone();

  virtual ~GameLayer();
  GameLayer();

  virtual f::XLayer* Realize();

  CREATE_FUNC(GameLayer)
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Game : public f::XScene {
private:
  NO__COPYASSIGN(Game)
public:

  virtual XScene* Realize();
  virtual ~Game();

  static Game* Create();
};


NS_END(invaders)


