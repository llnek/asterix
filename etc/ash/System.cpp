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

#include "System.h"
NS_BEGIN(ecs)

//////////////////////////////////////////////////////////////////////////////
//
void SystemList::add(not_null<System*> sys) {
  auto s = sys.get();
  if (ENP(head)) {
    s->next = s->previous = nullptr;
    head = tail = s;
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
System* SystemList::get(const SystemType &type) {
  for (auto s = head; NNP(s); s = s->next) {
    if (s->isa(type) ) { return s; }
  }
  return nullptr;
}


NS_END



