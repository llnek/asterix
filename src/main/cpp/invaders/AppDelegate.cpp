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
#include "support/XConfig.h"
#include "n/gnodes.h"
#include "p/Splash.h"
#include "p/Config.h"
#include "AppDelegate.h"
NS_USING(cocos2d)
NS_USING(fusilli)
NS_USING(invaders)


//////////////////////////////////////////////////////////////////////////////
//
AppDelegate::AppDelegate() {
  auto r= NodeRegistry::GetInstance();

  r->Register(ShipMotionNode::TypeId(), ShipMotionNode::Create());
  r->Register(CannonCtrlNode::TypeId(), CannonCtrlNode::Create());
  r->Register(AlienNotionNode::TypeId(), AlienNotionNode::Create());

  XConfig::SetInstance(new Config());
}

//////////////////////////////////////////////////////////////////////////////
//
AppDelegate::~AppDelegate()
{
}


