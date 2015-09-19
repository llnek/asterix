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

#include "ash/NodeFactory.h"
#include "ash/Node.h"
#include "cobjs.h"
NS_BEGIN(invaders)


//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL AlienMotionNode : public a::NodeFactory {
private:
  NO__COPYASSIGN(AlienMotionNode)
  AlienMotionNode();

public:

  static const a::NodeType TypeId();
  DEFCREATE_FUNC(AlienMotionNode)

  virtual a::Node* CreateNode() ;
  virtual ~AlienMotionNode();

};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL CannonCtrlNode : public a::NodeFactory {
private:
  NO__COPYASSIGN(CannonCtrlNode)
  CannonCtrlNode();

public:

  static const a::NodeType TypeId();
  DEFCREATE_FUNC(CannonCtrlNode)

  virtual a::Node* CreateNode();
  virtual ~CannonCtrlNode();


};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL ShipMotionNode : public a::NodeFactory {
private:
  NO__COPYASSIGN(ShipMotionNode)
  ShipMotionNode();

public:

  static const a::NodeType TypeId();
  DEFCREATE_FUNC(ShipMotionNode)

  virtual a::Node* CreateNode();
  virtual ~ShipMotionNode();

};



NS_END(invaders)


