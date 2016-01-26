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

#include "NodeRego.h"
#include "Node.h"
NS_BEGIN(ash)

//////////////////////////////////////////////////////////////////////////////
//
static NodeRegistry *_singleton;

//////////////////////////////////////////////////////////////////////////////
//
NodeRegistry* NodeRegistry::self() {
  if (ENP(_singleton)) {
    _singleton= mc_new(NodeRegistry);
  }
  return _singleton;
}

//////////////////////////////////////////////////////////////////////////////
//
NodeRegistry::~NodeRegistry() {
  F__LOOP(it, regos) { delete it->second; }
  //printf("NodeRegistry dtor\n");
}

//////////////////////////////////////////////////////////////////////////////
//
void NodeRegistry::rego(not_null<NodeFactory*> f) {
  derego(f->typeId());
  regos.insert(S__PAIR(NodeType,NodeFactory*,f->typeId(),f));
}

//////////////////////////////////////////////////////////////////////////////
//
void NodeRegistry::derego(const NodeType &t) {
  auto it= regos.find(t);
  if (it != regos.end()) {
    auto v= it->second;
    regos.erase(it);
    delete v;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
owner<Node*> NodeRegistry::reifyNode(const NodeType &t) {
  auto it=regos.find(t);
  return (it != regos.end()) ? it->second->reifyNode() : nullptr;
}

//////////////////////////////////////////////////////////////////////////////
//
owner<Node*> NodeFactory::reifyXXXNode(const s_map<sstr,COMType> &s) {
  return mc_new1(Node,s);
}

//////////////////////////////////////////////////////////////////////////////
//
owner<Node*> NodeFactory::reifyXXXNode(const s_vec<sstr> &flds,
    const s_vec<COMType> &types) {

  assert(flds.size() == types.size());
  s_map<sstr,a::COMType> s;

  for (int i=0; i < flds.size(); ++i) {
    s.insert(S__PAIR(sstr, COMType, flds[i], types[i] ));
  }

  return mc_new1(Node,s);
}


NS_END



