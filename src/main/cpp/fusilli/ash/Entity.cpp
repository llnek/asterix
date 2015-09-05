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

Entity::Entity() {
  components = Dictionary::create();
  components->retain();
  previous = nullptr;
  next= nullptr;
  //this.componentAdded = new signals.Signal();
  //this.componentRemoved = new signals.Signal();
}

void Entity::Add(component) {
  auto componentClass = component->ClassId();
  if (components->objectForKey(componentClass) != nullptr) {
    Remove(componentClass);
  }
  components->setObject(component, componentClass);
  //this.componentAdded.dispatch( this, componentClass );
}

void Entity::Remove(componentClass) {
  auto component = components->Retrieve(componentClass);
  if (component != nullptr) {
    components->Remove(componentClass);
    //componentRemoved.dispatch( this, componentClass );
  }
  return component;
}

void Entity::Get(const string& componentClass) {
  return components->Retrieve(componentClass);
}

        /**
         * Get all components from the entity.
         * @return {Array} Contains all the components on the entity
         */
void Entity::GetAll() {
  Array* arr= Array::create();
  DictElement* pe;
  CCDICT_FOREACH(components, pe) {
    arr->addObject(pe->getObject());
  }
  return arr;
}

bool Entity::Has(const string& componentClass) {
  return components->Has(componentClass);
}



NS_END(Ash)


