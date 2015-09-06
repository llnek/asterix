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
NS_USING(std)
NS_BEGIN(ash)

static NodeRegistry* _singleton;


//////////////////////////////////////////////////////////////////////////////
//
NodeRegistry* NodeRegistry::GetInstance() {
  if (_singleton == nullptr) {
    _singleton= new NodeRegistry();
  }
  return _singleton;
}

//////////////////////////////////////////////////////////////////////////////
//
NodeRegistry::~NodeRegistry() {

}

//////////////////////////////////////////////////////////////////////////////
//
NodeRegistry::NodeRegistry() {

}

//////////////////////////////////////////////////////////////////////////////
//
void NodeRegistry::Register(const NodeType& t, NodeFactory* f) {
  Deregister(t);
  regos.insert(pair<NodeType,NodeFactory*>(t,f));
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
Node* NodeRegistry::CreateNode(const NodeType& t) {
  auto it=regos.find(t);
  Node* rc= nullptr;
  if (it != regos.end()) {
    auto f= it->second;
    rc= f.CreateNode();
  }
  return rc;
}




NS_END(ash)
