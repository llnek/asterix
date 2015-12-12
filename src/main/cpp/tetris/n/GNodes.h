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

#include "ash/NodeRego.h"
#include "ash/Node.h"
#include "CObjs.h"

NS_BEGIN(tetris)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL ArenaNode : public a::NodeFactory {
public:

  virtual const a::NodeType typeId() { return "n/ArenaNode"; }
  virtual owner<a::Node*> reifyNode() {
    return reifyXXXNode(
        s_vec<sstr> { "collision", "gbox", "blocks", "motion",
        "pauser", "dropper", "shell" , "flines", "cpad" },
        s_vec<a::COMType> { "n/TileGrid", "n/GridBox", "n/BlockGrid", "n/Motion",
        "n/Pauser", "n/Dropper", "n/ShapeShell" , "n/FilledLines", "n/CtrlPad" });
  }

  NOCPYASS(ArenaNode)
  IMPLCZ(ArenaNode)
};


NS_END(tetris)
#endif



