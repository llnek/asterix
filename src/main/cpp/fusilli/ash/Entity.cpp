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

#include "Component.h"
#include "Entity.h"
NS_USING(std)
NS_BEGIN(ash)

//////////////////////////////////////////////////////////////////////////////
//
Entity::~Entity() {
}

//////////////////////////////////////////////////////////////////////////////
//
Entity::Entity() {
  previous = nullptr;
  next= nullptr;
}

//////////////////////////////////////////////////////////////////////////////
//
void Entity::Add(Component* c) {
  auto z = c->TypeId();
  auto it= components.find(z);
  if (it != components.end()) {
    delete Remove(z);
  }
  components.insert(pair<COMType,Component*>(z,c));
}

//////////////////////////////////////////////////////////////////////////////
//
Component* Entity::Remove(const COMType& z) {
  auto it = components.find(z);
  Component* rc= nullptr;

  if (it != components.end()) {
    rc= it->second;
    components.erase(it);
  }
  return rc;
}

//////////////////////////////////////////////////////////////////////////////
//
Component* Entity::Get(const COMType& z) {
  auto it=  components.find(z);
  Component* c= nullptr;
  if (it != components.end()) {
     c= it->second;
  }
  return c;
}

//////////////////////////////////////////////////////////////////////////////
//
const vector<Component*> Entity::GetAll() {
  vector<Component*> v;
  for (auto it = components.begin(); it != components.end(); ++it) {
    v.push_back(it->second);
  }
  return v;
}

//////////////////////////////////////////////////////////////////////////////
//
bool Entity::Has(const COMType& z) {
  return components.find(z) != components.end();
}


NS_END(ash)


