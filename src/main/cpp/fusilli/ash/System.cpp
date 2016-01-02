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

#include "System.h"
#include "Engine.h"
NS_BEGIN(ash)

//////////////////////////////////////////////////////////////////////////////
//
void System::restart() {
  active=true;
}

//////////////////////////////////////////////////////////////////////////////
//
void System::suspend() {
  active=false;
}

//////////////////////////////////////////////////////////////////////////////
//
void System::removeFromEngine(not_null<Engine*> e) {
}

//////////////////////////////////////////////////////////////////////////////
//
void System::addToEngine(not_null<Engine*> e) {
}

//////////////////////////////////////////////////////////////////////////////
//
bool System::update(float time) {
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
bool System::isa(const SystemType &type) {
  return type == typeId();
}

//////////////////////////////////////////////////////////////////////////////
//
void SystemList::add(not_null<System*> sys) {
  auto s = sys.get();
  if (ENP(head)) {
    head = tail = s;
    s->next = s->previous = nullptr;
  } else {
    System *node=nullptr;
    for(node = tail; NNP(node); node = node->previous) {
      if (node->priority() <= s->priority() ) {
        break;
      }
    }
    if (node == tail ) {
      tail->next = s;
      s->previous = tail;
      s->next = nullptr;
      tail = s;
    }
    else
    if ( ENP(node )) {
      s->previous = nullptr;
      s->next = head;
      head->previous = s;
      head = s;
    }
    else {
      s->next = node->next;
      s->previous = node;
      node->next->previous = s;
      node->next = s;
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void SystemList::remove(not_null<System*> sys) {
  auto s = sys.get();
  if (head == s) {
    head = head->next;
  }
  if (tail == s) {
    tail = tail->previous;
  }
  if (NNP(s->previous )) {
    s->previous->next = s->next;
  }
  if (NNP(s->next )) {
    s->next->previous = s->previous;
  }
  mc_del_ptr(s);
}

//////////////////////////////////////////////////////////////////////////////
//
void SystemList::removeAll() {
  clear();
}

//////////////////////////////////////////////////////////////////////////////
//
System* SystemList::get(const SystemType &type) {
  for (auto s = head; NNP(s); s = s->next) {
    if (s->isa(type) ) {
      return s;
    }
  }
  return nullptr;
}

//////////////////////////////////////////////////////////////////////////////
//
void SystemList::clear() {
  while (NNP(head)) {
    auto e= head;
    head = head->next;
    delete e;
  }
  SNPTR(head)
  SNPTR(tail)
}


NS_END(ash)


