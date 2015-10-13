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

#if !defined(__NODEREGISTRY_H__)
#define __NODEREGISTRY_H__

#include <vector>
#include <map>
#include "Ash.h"
NS_ALIAS(s, std)
NS_BEGIN(ash)

class Node;
//////////////////////////////////////////////////////////////////////////////
//
class FS_DLL NodeFactory {
protected:
  NodeFactory() {}
private:
  NO__CPYASS(NodeFactory)
public:
  virtual const NodeType TypeId() = 0;
  virtual owner<Node*> ReifyNode() = 0;
  virtual ~NodeFactory() {}
};

//////////////////////////////////////////////////////////////////////////////
//
class FS_DLL NodeRegistry {
private:

  s::map<NodeType,NodeFactory*> regos;

  NO__CPYASS(NodeRegistry)
  NodeRegistry();

public:

  static NodeRegistry* Self();
  virtual ~NodeRegistry();

  void Register(not_null<NodeFactory*>);
  void Deregister(const NodeType&);

  owner<Node*> ReifyNode(const NodeType&);

};


NS_END(ash)
#endif


