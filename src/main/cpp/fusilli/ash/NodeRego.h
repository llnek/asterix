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

#if !defined(__NODEREGO_H__)
#define __NODEREGO_H__

#include "Ash.h"
NS_BEGIN(ash)

class Node;
//////////////////////////////////////////////////////////////////////////////
//
class FS_DLL NodeFactory {
protected:

  owner<Node*> reifyXXXNode(const s::vector<stdstr>&, const s::vector<COMType>&);

public:

  virtual owner<Node*> reifyNode() = 0;
  virtual const NodeType typeId() = 0;

  NO__CPYASS(NodeFactory)
  IMPL_CTOR(NodeFactory)
};

//////////////////////////////////////////////////////////////////////////////
//
class FS_DLL NodeRegistry {
private:

  s::map<NodeType,NodeFactory*> regos;

  NO__CPYASS(NodeRegistry)
  NodeRegistry();

public:

  static NodeRegistry* self();
  virtual ~NodeRegistry();

  void rego(not_null<NodeFactory*>);
  void derego(const NodeType&);

  owner<Node*> reifyNode(const NodeType&);
};


NS_END(ash)
#endif


