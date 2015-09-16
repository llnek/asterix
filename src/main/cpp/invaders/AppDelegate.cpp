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
#include "p/Config.h"
#include "AppDelegate.h"
NS_ALIAS(cc, cocos2d)
NS_ALIAS(fs, fusilli)
NS_ALIAS(p, invaders)


//////////////////////////////////////////////////////////////////////////////
//
AppDelegate::AppDelegate() {
  auto r= fs::NodeRegistry::GetInstance();

  r->Register( p::ShipMotionNode::Create());
  r->Register( p::CannonCtrlNode::Create());
  r->Register( p::AlienNotionNode::Create());

  fs::XConfig::SetInstance(new p::Config());
}

//////////////////////////////////////////////////////////////////////////////
//
AppDelegate::~AppDelegate()
{
}


