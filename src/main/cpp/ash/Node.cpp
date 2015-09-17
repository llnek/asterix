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

#include "Node.h"
NS_BEGIN(ash)


//////////////////////////////////////////////////////////////////////////////
//
Node* Node::Create(const s::map<stdstr, COMType>& s) {
  auto node= new Node();
  for (auto it = s.begin();
      it != s.end(); ++it) {
    node->types.insert(pair<COMType, stdstr>(it->second,it->first));
  }
  return node;
}

//////////////////////////////////////////////////////////////////////////////
//
Node::~Node() {
}

//////////////////////////////////////////////////////////////////////////////
//
Node::Node()
  : entity( nullptr)
  , previous( nullptr)
  , next( nullptr) {

}

//////////////////////////////////////////////////////////////////////////////
//
bool Node::BindEntity(Entity* e) {
  if (NNP(entity) ||
      ENP(e) ||
      !e->IsOk() ) {
    return false;
  }
  values.clear();
  for (auto it= types.begin(); it != types.end(); ++it) {
    auto f= it->second;
    auto t= it->first;
    auto c= e->Get(t);
    if (NNP(c)) {
      values.insert(pair<stdstr, Component*>(f,c));
    } else {
      values.clear();
      break;
    }
  }
  return values.size() > 0;
}

//////////////////////////////////////////////////////////////////////////////
//
bool Node::BelongsTo(Entity* e) {
  return entity == e;
}

//////////////////////////////////////////////////////////////////////////////
//
Component* Node::Get(const stdstr& field) {
  auto it = values.find(field);
  if (it != values.end()) {
    return it->second;
  } else {
    return nullptr;
  }
}




NS_END(ash)

