#include "SystemList.h"


SystemList::Add(System* s ) {
  auto len = m_list->count();
  auto x = s->Priority();
  int pos= -1;
  for (int i= len-1; i >= 0; --i) {
    auto y= static_cast<System*>(m_list->getObjectAtIndex(i));
    if (y->Priority() <= x) {
      pos= i+1;
      break;
    }
  }
  if (pos >= 0 && pos < len) {
    m_list->insertObject(pos, s);
  } else {
    m_list->addObject(s);
  }
}

SystemList::Remove(System* s ) {
  m_list->removeObject(s);
}

SystemList::RemoveAll() {
  m_list->removeAllObjects();
}

SystemList::Get(const string& type ) {
  for (auto it = m_list->begin(); it != m_list->end(); ++it) {
    auto rc= static_cast<System*>(*it);
    if (rc->Is(type)) {
      return rc;
    }
  }
  return nullptr;
}
