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

#include "EntityList.h"
NS_BEGIN(Ash)


//////////////////////////////////////////////////////////////////////////////
//
void EntityList::Add(Entity* e ) {
  if ( ! head ) {
    head = tail = e;
  } else {
    tail->next = e;
    e->previous = tail;
    tail = e;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void EntityList::Remove(Entity* e) {
  if (head == e) {
    head = head->next;
  }
  if (tail == e) {
    tail = tail->previous;
  }
  if (e->previous ) {
    e->previous->next = e->next;
  }
  if (e->next) {
    e->next->previous = e->previous;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void EntityList::RemoveAll() {
  while(head != nullptr) {
    auto e= head;
    head = head->next;
    e->previous = nullptr;
    e->next = nullptr;
  }
  tail = nullptr;
}




NS_END(Ash)
