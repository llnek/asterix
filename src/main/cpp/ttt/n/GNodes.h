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
#pragma once
//////////////////////////////////////////////////////////////////////////////
#include "ash/NodeRego.h"
#include "ash/Node.h"
#include "CObjs.h"
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL BoardNode : public a::NodeFactory {

  virtual owner<a::Node*> reifyNode() {
    return reifyXXXNode({
          {"players", "n/Players" },
          {"select", "n/CellPos"},
          {"grid", "n/Grid"}
    });
  }

  MDECL_NFAC_TPID( "n/BoardNode")
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL RobotNode : public a::NodeFactory {

  virtual owner<a::Node*> reifyNode() {
    return reifyXXXNode({
        { "player", "n/Player"},
        { "robot", "n/SmartAI"}
    });
  }

  MDECL_NFAC_TPID( "n/RobotNode")
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL HumanNode : public a::NodeFactory {

  virtual owner<a::Node*> reifyNode() {
    return reifyXXXNode({
        { "motion", "n/Gesture"},
        { "player", "n/Player"}
    });
  }

  MDECL_NFAC_TPID( "n/HumanNode")
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL ArenaNode : public a::NodeFactory {

  virtual owner<a::Node*> reifyNode() {
    return reifyXXXNode({
        {"squares", "n/CSquares"},
        {"slots", "n/GVars" }
    });
  }

  MDECL_NFAC_TPID( "n/ArenaNode")
};



NS_END(tttoe)


