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

#include "NodeFactory.h"
#include "Node.h"
#include "cobjs.h"
NS_USING(fusilli)
NS_BEGIN(invaders)

//////////////////////////////////////////////////////////////////////////////
//
Node* AlienMotionNode::CreateNode() {
  map<string,COMType> s;
  s.insert(pair<string,COMType>("aliens", "fusilli/AlienSquad"));
  s.insert(pair<string,COMType>("looper", "fusilli/Looper"));
  return new Node(s);
}

//////////////////////////////////////////////////////////////////////////////
//
const NodeType AlienMotionNode::TypeId() {
  return "invaders/AlienMotionNode";
}

//////////////////////////////////////////////////////////////////////////////
//
Node* CannonCtrlNode::CreateNode() {
  map<string,COMType> s;
  s.insert(pair<string,COMType>("looper", "fusilli/Looper"));
  s.insert(pair<string,COMType>("cannon", "fusilli/Cannon"));
  s.insert(pair<string,COMType>("ship", "fusilli/Ship"));
  return new Node(s);
}

//////////////////////////////////////////////////////////////////////////////
//
const NodeType CannonCtrlNode::TypeId() {
  return "invaders/CannonCtrlNode";
}

//////////////////////////////////////////////////////////////////////////////
//
Node* ShipMotionNode::CreateNode() {
  map<string,COMType> s;
  s.insert(pair<string,COMType>("velocity", "fusilli/Velocity"));
  s.insert(pair<string,COMType>("motion", "fusilli/Motion"));
  s.insert(pair<string,COMType>("ship", "fusilli/Ship"));
  return new Node(s);
}

//////////////////////////////////////////////////////////////////////////////
//
const NodeType ShipMotionNode::TypeId() {
  return "invaders/ShipMotionNode";
}

NS_END(invaders)


