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

#include "NodeRegistry.h"
#include "Node.h"
NS_BEGIN(ash)

//////////////////////////////////////////////////////////////////////////////
//
static NodeRegistry* _singleton;

//////////////////////////////////////////////////////////////////////////////
//
NodeRegistry* NodeRegistry::Self() {
  if (ENP(_singleton)) {
    _singleton= new NodeRegistry();
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
NodeRegistry::NodeRegistry() {
}

//////////////////////////////////////////////////////////////////////////////
//
void NodeRegistry::Register(not_null<NodeFactory*> f) {
  Deregister(f->TypeId());
  regos.insert(S__PAIR(NodeType,NodeFactory*,f->TypeId(),f));
}

//////////////////////////////////////////////////////////////////////////////
//
void NodeRegistry::Deregister(const NodeType& t) {
  auto it= regos.find(t);
  if (it != regos.end()) {
    auto v= it->second;
    regos.erase(it);
    delete v;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
owner<Node*> NodeRegistry::ReifyNode(const NodeType& t) {
  auto it=regos.find(t);
  Node* rc= nullptr;
  if (it != regos.end()) {
    auto f= it->second;
    rc= f->ReifyNode();
  }
  return rc;
}

//////////////////////////////////////////////////////////////////////////////
//
owner<Node*> NodeFactory::ReifyXXXNode(const s::vector<stdstr>& flds,
    const s::vector<COMType>& types) {

  assert(flds.size() == types.size());
  s::map<stdstr,a::COMType> s;

  for (int i=0; i < flds.size(); ++i) {
    s.insert(S__PAIR(stdstr, COMType, flds[i], types[i] ));
  }

  return a::Node::Reify(s);
}


NS_END(ash)

