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

#include "Engine.h"
#include "Entity.h"
NS_BEGIN(ash)

//////////////////////////////////////////////////////////////////////////////
//
Entity::Entity(const sstr& group, not_null<Engine*> e) {
  this->group=group;
  this->engine= e;
}

//////////////////////////////////////////////////////////////////////////////
//
Entity::~Entity() {
  F__LOOP(it, parts) { delete it->second; }
  //printf("Entity dtor\n");
}

//////////////////////////////////////////////////////////////////////////////
//
void Entity::checkin(not_null<Component*> c) {
  auto z = c->typeId();
  if (has(z)) {
    throw "cannot reassign component";
  }
  parts.insert(S__PAIR(COMType, Component*, z, c));
  engine->notifyModify(this);
}

//////////////////////////////////////////////////////////////////////////////
//
void Entity::markDelete() {
  dead=true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Entity::purge(const COMType& z) {
  auto it = parts.find(z);

  if (it != parts.end()) {
    auto rc= it->second;
    parts.erase(it);
    delete rc;
    engine->notifyModify(this);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
Component* Entity::get(const COMType& z) {
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
bool Entity::has(const COMType& z) {
  return parts.find(z) != parts.end();
}


NS_END(ash)


