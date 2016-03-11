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

#include "TypeRego.h"
#include "Engine.h"
#include "Node.h"
NS_BEGIN(ecs)

//////////////////////////////////////////////////////////////////////////////
//
Node::Node(not_null<Engine*> e, NodeId eid) {
  this->_engine= e;
  this->_eid= eid;
}

//////////////////////////////////////////////////////////////////////////////
//
Node::~Node() {
  //printf("Node dtor\n");
  F__LOOP(it, _parts) {
    auto c= it->second;
    _engine->rego()->unbind(c,this);
    delete c;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Node::checkin(not_null<Component*> c) {
  auto z = c->typeId();
  assert(! has(z));
  _engine->rego()->bind(c,this);
  _parts.insert(S__PAIR(COMType, Component*, z, c));
}

//////////////////////////////////////////////////////////////////////////////
//
void Node::purge(const COMType &z) {
  auto it = _parts.find(z);
  if (it != _parts.end()) {
    auto c= it->second;
    _parts.erase(it);
    _engine->rego()->unbind(c,this);
    delete c;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
Component* Node::get(const COMType &z) {
  auto it=  _parts.find(z);
  if (it != _parts.end()) {
    return it->second;
  } else {
    return nullptr;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
const s_vec<Component*> Node::getAll() {
  s_vec<Component*> v;
  F__LOOP(it, _parts) {
    v.push_back(it->second);
  }
  return v;
}

//////////////////////////////////////////////////////////////////////////////
//
bool Node::has(const COMType &z) {
  return _parts.find(z) != _parts.end();
}


NS_END


