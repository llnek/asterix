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

#include "core/XConfig.h"
#include "XGameLayer.h"
#include "MainGame.h"
NS_BEGIN(fusii)

static MainGame* _singleton;

//////////////////////////////////////////////////////////////////////////////
//
XGameLayer* MainGame::Get() {
  return  _singleton->GetGLayer();
}

//////////////////////////////////////////////////////////////////////////////
//
MainGame* MainGame::Self() {
  return _singleton;
}

//////////////////////////////////////////////////////////////////////////////
//
MainGame::~MainGame() {
  for (auto it=pools.begin(); it != pools.end(); ++it) {
    delete it->second;
  }
  if (NNP(seedData)) { seedData->release(); }
}

//////////////////////////////////////////////////////////////////////////////
//
MainGame::MainGame() {
  mode = GMode::ONE;
  SNPTR(seedData)
  SNPTR(wss)
  level = 1;
  running=false;
}

//////////////////////////////////////////////////////////////////////////////
//
c::Dictionary* MainGame::GetLCfg() {
  return XCFG()->GetLevelCfg(s::to_string(level));
}

//////////////////////////////////////////////////////////////////////////////
//
XPool* MainGame::GetPool(const stdstr& key) {
  auto it = pools.find(key);
  if (it != pools.end()) {
    return it->second;
  } else {
    return nullptr;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
XPool* MainGame::ReifyPool(const stdstr& key) {
  auto it = pools.find(key);
  if (it != pools.end()) {
    delete it->second;
    pools.erase(it);
  }
  auto p=new XPool();
  pools.insert(s::pair<stdstr, XPool*>(key, p));
  return p;
}

//////////////////////////////////////////////////////////////////////////////
//
void MainGame::ResetPools() {
  for (auto it = pools.begin(); it != pools.end(); ++it) {
    it->second->ClearAll();
  }
}

//////////////////////////////////////////////////////////////////////////
//
void MainGame::SetMode(GMode m, c::Dictionary* d) {
  this->seedData = d;
  d->retain();
  this->mode= m;
}

//////////////////////////////////////////////////////////////////////////
//
MainGame* MainGame::Reify(not_null<MainGame*> g, GMode mode) {
  return Reify(g, mode, c::Dictionary::create());
}

//////////////////////////////////////////////////////////////////////////
//
MainGame* MainGame::Reify(not_null<MainGame*> g,
    GMode mode,
    not_null<c::Dictionary*> options) {

  g->SetMode(mode, options);
  g->Realize();
  return g;
}

//////////////////////////////////////////////////////////////////////////
//
void MainGame::Bind(not_null<MainGame*> m) {
  _singleton=m;
}

NS_END(fusii)

