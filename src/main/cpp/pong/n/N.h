// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Ken Leung. All rights reserved.
#pragma once
//////////////////////////////////////////////////////////////////////////////

#include "ash/NodeRego.h"
#include "C.h"
#include "lib.h"

NS_BEGIN(pong)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL BallNode : public a::NodeFactory {

  virtual owner<a::Node*> reifyNode() {
    return reifyXXXNode({
        {"ball", "n/Ball" }
    });
  }

  MDECL_NFAC_TPID( "n/BallNode")
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL FauxPaddleNode : public a::NodeFactory {

  virtual owner<a::Node*> reifyNode() {
    return reifyXXXNode({
        {"lastpos", "n/Position" },
        {"paddle", "n/Paddle" },
        {"faux", "n/Faux" }
    });
  }

  MDECL_NFAC_TPID( "n/FauxPaddleNode")
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL PaddleNode : public a::NodeFactory {

  virtual owner<a::Node*> reifyNode() {
    return reifyXXXNode({
      {"lastpos" , "n/Position" },
      {"motion" , "n/Gesture" },
      {"paddle" , "n/Paddle" }
    });
  }

  MDECL_NFAC_TPID( "n/PaddleNode")
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL ArenaNode : public a::NodeFactory {

  virtual owner<a::Node*> reifyNode() {
    return reifyXXXNode({
      {"players" , "n/Players" },
      {"slots", "n/GVars"}
    });
  }

  MDECL_NFAC_TPID( "n/ArenaNode")
};


NS_END(pong)


