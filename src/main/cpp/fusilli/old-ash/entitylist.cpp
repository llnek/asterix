#include "EntityList.h"


EntityList::Add(Entity* e ) {
  m_list->addObject(e);
}

EntityList::Remove(Entity* e ) {
  m_list->removeObject(e);
}

EntityList::RemoveAll() {
  m_list->removeAll();
}


EntityList::~EntityList() {
  m_list->release();
}

EntityList::EntityList() {
  m_list = Array::create();
  m_list.retain();
}

