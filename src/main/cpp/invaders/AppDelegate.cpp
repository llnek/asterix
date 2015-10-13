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

#include "ash/NodeRegistry.h"
#include "n/gnodes.h"
#include "p/Config.h"
#include "AppDelegate.h"
NS_USING(invaders)
NS_USING(fusii)
NS_USING(ash)

//////////////////////////////////////////////////////////////////////////////
//
AppDelegate::AppDelegate() {

  // bootstrap app here

  // step.1: register all ash::node factories here
  auto r= NodeRegistry::Self();

  r->Register( new AlienMotionNode());
  r->Register( new ShipMotionNode());
  r->Register( new CannonCtrlNode());

  // step.2: set up app-config
  XConfig::Bind(Config::Reify());
}

//////////////////////////////////////////////////////////////////////////////
//
AppDelegate::~AppDelegate() {
  //delete NodeRegistry::Self();
  //delete XConfig::Self();
}



