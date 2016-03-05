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
#include "Entity.h"
NS_BEGIN(ecs)

//////////////////////////////////////////////////////////////////////////////
//
Entity::Entity(not_null<Engine*> e, EntId eid) {
  this->engine= e;
  this->eid= eid;
}

//////////////////////////////////////////////////////////////////////////////
//
Entity::~Entity() {
  //printf("Entity dtor\n");
  F__LOOP(it, parts) {
    auto c= it->second;
    engine->rego()->unbind(c,this);
    delete c;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Entity::checkin(not_null<Component*> c) {
  auto z = c->typeId();
  assert(! has(z));
  engine->rego()->bind(c,this);
  parts.insert(S__PAIR(COMType, Component*, z, c));
}

//////////////////////////////////////////////////////////////////////////////
//
void Entity::purge(const COMType &z) {
  auto it = parts.find(z);
  if (it != parts.end()) {
    auto c= it->second;
    parts.erase(it);
    engine->rego()->unbind(c,this);
    delete c;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
Component* Entity::get(const COMType &z) {
  auto it=  parts.find(z);
  if (it != parts.end()) {
    return it->second;
  } else {
    return nullptr;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
const s_vec<Component*> Entity::getAll() {
  s_vec<Component*> v;
  F__LOOP(it, parts) {
    v.push_back(it->second);
  }
  return v;
}

//////////////////////////////////////////////////////////////////////////////
//
bool Entity::has(const COMType &z) {
  return parts.find(z) != parts.end();
}


NS_END


