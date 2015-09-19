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

#include "gnodes.h"
NS_BEGIN(invaders)

//////////////////////////////////////////////////////////////////////////////
//
a::Node* AlienMotionNode::CreateNode() {
  s::map<stdstr,a::COMType> s;
  s.insert(pair<stdstr,a::COMType>("aliens", "n/AlienSquad"));
  s.insert(pair<stdstr,a::COMType>("looper", "n/Looper"));
  return new a::Node(s);
}

//////////////////////////////////////////////////////////////////////////
//
AlienMotionNode::AlienMotionNode() {
}

//////////////////////////////////////////////////////////////////////////
//
AlienMotionNode::~AlienMotionNode() {
}

//////////////////////////////////////////////////////////////////////////////
//
const a::NodeType AlienMotionNode::TypeId() {
  return "n/AlienMotionNode";
}

//////////////////////////////////////////////////////////////////////////////
//
a::Node* CannonCtrlNode::CreateNode() {
  s::map<stdstr, a::COMType> s;
  s.insert(pair<stdstr, a::COMType>("looper", "n/Looper"));
  s.insert(pair<stdstr, a::COMType>("cannon", "n/Cannon"));
  s.insert(pair<stdstr, a::COMType>("ship", "n/Ship"));
  return new a::Node(s);
}

//////////////////////////////////////////////////////////////////////////
//
CannonCtrlNode::CannonCtrlNode() {
}

//////////////////////////////////////////////////////////////////////////
//
CannonCtrlNode::~CannonCtrlNode() {
}

//////////////////////////////////////////////////////////////////////////////
//
const a::NodeType CannonCtrlNode::TypeId() {
  return "n/CannonCtrlNode";
}

//////////////////////////////////////////////////////////////////////////////
//
a::Node* ShipMotionNode::CreateNode() {
  s::map<stdstr, a::COMType> s;
  s.insert(pair<stdstr, a::COMType>("vel", "n/Velocity"));
  s.insert(pair<stdstr, a::COMType>("motion", "n/Motion"));
  s.insert(pair<stdstr, a::COMType>("ship", "n/Ship"));
  return new a::Node(s);
}

//////////////////////////////////////////////////////////////////////////
//
ShipMotionNode::ShipMotionNode() {
}

//////////////////////////////////////////////////////////////////////////
//
ShipMotionNode::~ShipMotionNode() {
}


//////////////////////////////////////////////////////////////////////////////
//
const a::NodeType ShipMotionNode::TypeId() {
  return "n/ShipMotionNode";
}



NS_END(invaders)


