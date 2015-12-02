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
#include "CObjs.h"
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL BoardNode : public a::NodeFactory {
public:

  virtual const a::NodeType typeId() { return "n/BoardNode"; }

  virtual owner<a::Node*> reifyNode() {
    return reifyXXXNode(
        s::vector<stdstr> {
          "selection", "players", "robot", "grid", "view" },
        s::vector<a::COMType> {
          "n/UISelection", "n/Players", "n/SmartAlgo", "n/Grid", "n/PlayView" });
  }

  NOCPYASS(BoardNode)
  IMPL_CTOR(BoardNode)
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL GUINode : public a::NodeFactory {
public:

  virtual const a::NodeType typeId() { return "n/GUINode"; }

  virtual owner<a::Node*> reifyNode() {
    return reifyXXXNode(
        s::vector<stdstr> { "selection", "view" },
        s::vector<a::COMType> { "n/UISelection", "n/PlayView" });
  }

  NOCPYASS(GUINode)
  IMPL_CTOR(GUINode)
};


NS_END(tttoe)
#endif


