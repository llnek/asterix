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

#include "Entity.h"
#include "Node.h"
NS_BEGIN(ash)


//////////////////////////////////////////////////////////////////////////////
//
owner<Node*> Node::Reify(const s::map<stdstr, COMType>& s) {
  auto node= mc_new(Node);
  F__LOOP(it, s) {
    node->types.insert(S__PAIR(COMType, stdstr, it->second, it->first));
  }
  return node;
}

//////////////////////////////////////////////////////////////////////////////
//
Node::~Node() {
//  printf("Node dtor!\n");
}

//////////////////////////////////////////////////////////////////////////////
//
Node::Node() {
  SNPTR(previous)
  SNPTR(next)
  SNPTR(entity)
}

//////////////////////////////////////////////////////////////////////////////
//
bool Node::BindEntity(not_null<Entity*> e) {
  if (NNP(entity) ||
      !e->IsOk() ) {
    return false;
  }
  values.clear();
  F__LOOP(it, types) {
    auto f= it->second;
    auto t= it->first;
    auto c= e->Get(t);
    if (NNP(c)) {
      values.insert(S__PAIR(stdstr, Component*, f, c));
    } else {
      // this entity is no good, doesn't have the right
      // components
      values.clear();
      break;
    }
  }
  if ( values.size() > 0) {
    entity=e.get();
    return true;
  } else {
    return false;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
bool Node::BelongsTo(not_null<Entity*> e) {
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

