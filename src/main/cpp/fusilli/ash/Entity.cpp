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
NS_BEGIN(ash)


//////////////////////////////////////////////////////////////////////////////
//
Entity* Entity::Create(const stdstr& group, Engine* e) {
  auto ent= new Entity();
  ent->group=group;
  ent->engine= e;
  return ent;
}

//////////////////////////////////////////////////////////////////////////////
//
Entity::~Entity() {
  for (auto it= parts.begin(); it != parts.end(); ++it) {
    delete *it;
  }
  parts.clear();
}

//////////////////////////////////////////////////////////////////////////////
//
Entity::Entity()
  : previous(nullptr)
  , next(nullptr)
  , dead(false)
  , engine(nullptr) {

}

//////////////////////////////////////////////////////////////////////////////
//
void Entity::Rego(Component* c) {
  auto z = c->TypeId();
  if (Has(z)) {
    throw "cannot reassign component";
  }
  parts.insert(pair<COMType,Component*>(z,c));
  c->par= this;
  engine->NotifyModify(this);
}

//////////////////////////////////////////////////////////////////////////////
//
void Entity::MarkDelete() {
  dead=true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Entity::Remove(const COMType& z) {
  auto it = parts.find(z);
  Component* rc= nullptr;

  if (it != parts.end()) {
    rc= it->second;
    parts.erase(it);
  }
  mc_del_ptr(rc);
  engine->NotifyModify(this);
}

//////////////////////////////////////////////////////////////////////////////
//
Component* Entity::Get(const COMType& z) {
  auto it=  parts.find(z);
  Component* c= nullptr;
  if (it != parts.end()) {
    c= it->second;
  }
  return c;
}

//////////////////////////////////////////////////////////////////////////////
//
const s::vector<Component*> Entity::GetAll() {
  s::vector<Component*> v;
  for (auto it = parts.begin();
      it != parts.end(); ++it) {
    v.push_back(it->second);
  }
  return v;
}

//////////////////////////////////////////////////////////////////////////////
//
bool Entity::Has(const COMType& z) {
  return parts.find(z) != parts.end();
}


NS_END(ash)


