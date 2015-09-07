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
class CC_DLL AlienNotionNode : public NodeFactory {
public:
  virtual Node* CreateNode() override;
  virtual ~AlienMotionNode();
  AlienMotionNode();
private:
  DISALLOW_COPYASSIGN(AlienMotionNode)
};


//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL CannonCtrlNode : public NodeFactory {
public:

  virtual Node* CreateNode() override;
  virtual ~CannonCtrlNode();
  CannonCtrlNode();

private:
  DISALLOW_COPYASSIGN(CannonCtrlNode)
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL ShipMotionNode : public NodeFactory {
public:

  virtual Node* CreateNode() override;
  virtual ~ShipMotionNode();
  ShipMotionNode();
private:
  DISALLOW_COPYASSIGN(ShipMotionNode)
}



NS_END(invaders)


