
#include "Entity.h"

Entity::Entity() {
  m_comps= Dictionary::create();
  //this.componentAdded = new signals.Signal();
  //this.componentRemoved = new signals.Signal();
}

void Entity::Add(c) {
  const string id= c->TypeId();
  if (m_comps->objectForKey(id) != nullptr) {
    m_comps->removeObjectForKey(id);
  }
  m_comps->setObject(c, id);
  //this.componentAdded.dispatch( this, componentClass );
}

void Entity::Remove(const string& type) {
  auto r= m_comps->objectForKey(type);
  if (r != nullptr) {
    m_comps->removeObjectForKey(type);
    //this.componentRemoved.dispatch( this, componentClass );
  }
  return r;
}

Ref* Entity::Get(const string& type) {
  return m_comps->objectForKey(type);
}

Array* Entity::GetAll() {
  auto arr = Array::create();
  DictElement* pElement;
  CCDICT_FOREACH(dict, pElement)
  {
    arr->addObject(pElement->getObject());
  }
  return arr;
}

bool Entity::Has(const string& type) {
  return m_comps->objectForKey(type) != nullptr;
}

