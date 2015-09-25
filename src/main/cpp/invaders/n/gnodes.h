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

#if !defined(__GNODES_H__)
#define __GNODES_H__


#include "ash/NodeRegistry.h"
#include "ash/Node.h"
#include "cobjs.h"
NS_ALIAS(a,ash)
NS_BEGIN(invaders)


//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL AlienMotionNode : public a::NodeFactory {
private:
  NO__CPYASS(AlienMotionNode)

public:

  virtual const a::NodeType TypeId() {
    return "n/AlienMotionNode"; }

  AlienMotionNode();

  virtual owner<a::Node*> CreateNode() ;
  virtual ~AlienMotionNode();

};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL CannonCtrlNode : public a::NodeFactory {
private:
  NO__CPYASS(CannonCtrlNode)

public:

  virtual const a::NodeType TypeId() {
    return "n/CannonCtrlNode"; }

  CannonCtrlNode();

  virtual owner<a::Node*> CreateNode();
  virtual ~CannonCtrlNode();

};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL ShipMotionNode : public a::NodeFactory {
private:
  NO__CPYASS(ShipMotionNode)

public:

  virtual const a::NodeType TypeId() {
    return "n/ShipMotionNode"; }

  ShipMotionNode();

  virtual owner<a::Node*> CreateNode();
  virtual ~ShipMotionNode();

};



NS_END(invaders)
#endif


