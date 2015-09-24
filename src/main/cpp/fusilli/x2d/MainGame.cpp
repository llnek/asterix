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
#include "MainGame.h"
NS_BEGIN(fusilli)


static XScene* _singleton;
static int _gameLayer;

//////////////////////////////////////////////////////////////////////////////
//
void MainGame::Set(not_null<XScene*> g, int gy) {
  _gameLayer= gy;
  _singleton=g;
}

//////////////////////////////////////////////////////////////////////////////
//
XGameLayer* MainGame::Get() {
  //return DCAST(XGameLayer*, _singleton->GetLayer(_gameLayer));
  return SCAST(XGameLayer*, _singleton->GetLayer(_gameLayer));
}

//////////////////////////////////////////////////////////////////////////////
//
XScene* MainGame::Self() {
  return _singleton;
}

//////////////////////////////////////////////////////////////////////////////
//
MainGame::~MainGame() {
}

//////////////////////////////////////////////////////////////////////////////
//
MainGame::MainGame() {
}





NS_END(fusilli)

