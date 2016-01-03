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

#define MDECL_NFAC_TPID(x) \
    virtual const ash::NodeType typeId() { return x; }


class Node;
//////////////////////////////////////////////////////////////////////////////
//
class FS_DLL NodeFactory {
protected:

  owner<Node*> reifyXXXNode(const s_vec<sstr>&, const s_vec<COMType>&);
  owner<Node*> reifyXXXNode(const s_map<sstr,COMType>&);

public:

  virtual owner<Node*> reifyNode() = 0;
  virtual const NodeType typeId() = 0;

  virtual ~NodeFactory() {}
  NodeFactory() {}
  NOCPYASS(NodeFactory)
};

//////////////////////////////////////////////////////////////////////////////
//
class FS_DLL NodeRegistry {
private:

  // owns the factories
  s_map<NodeType,NodeFactory*> regos;

public:

  owner<Node*> reifyNode(const NodeType&);
  static NodeRegistry* self();

  void rego(not_null<NodeFactory*>);
  void derego(const NodeType&);

  virtual ~NodeRegistry();
  NodeRegistry();
  NOCPYASS(NodeRegistry)
};


NS_END(ash)
#endif


