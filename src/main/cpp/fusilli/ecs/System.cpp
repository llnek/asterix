// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Kenneth Leung. All rights reserved.

#include "System.h"
NS_BEGIN(ecs)

//////////////////////////////////////////////////////////////////////////////
//
void SystemList::add(not_null<System*> sys) {
  auto s = sys.get();
  if (E_NIL(_head)) {
    s->_next = s->_prev = P_NIL;
    _head = _tail = s;
  } else {
    System *node= P_NIL;
    for (node = _tail; N_NIL(node); node = node->_prev) {
      if (node->priority() <= s->priority() ) {
        break;
      }
    }
    if (node == _tail ) {
      _tail->_next = s;
      s->_prev = _tail;
      s->_next = P_NIL;
      _tail = s;
    }
    else
    if ( ENP(node )) {
      s->_prev = P_NIL;
      s->_next = _head;
      _head->_prev = s;
      _head = s;
    }
    else {
      s->_next = node->_next;
      s->_prev = node;
      node->_next->_prev = s;
      node->_next = s;
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
System* SystemList::get(const SystemType &type) {
  for (auto s = _head; N_NIL(s); s = s->_next) {
    if (s->isa(type) ) { return s; }
  }
  return P_NIL;
}


NS_END



