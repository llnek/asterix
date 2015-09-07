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

#include "SystemList.h"
NS_BEGIN(ash)


//////////////////////////////////////////////////////////////////////////////
//
void SystemList::Add(System* system ) {
  if (!head ) {
    head = tail = system;
    system->next = system->previous = nullptr;
  } else {
    System* node;
    for (node = tail; node != nullptr; node = node->previous ) {
      if (node->Priority() <= system->Priority() ) {
        break;
      }
    }
    if (node == tail) {
      tail->next = system;
      system->previous = tail;
      system->next = nullptr;
      tail = system;
    } else if (!node) {
      system->next = head;
      system->previous = nullptr;
      head->previous = system;
      head = system;
    } else {
      system->next = node->next;
      system->previous = node;
      node->next->previous = system;
      node->next = system;
    }
  }
}


void SystemList::Remove(System* system ) {
  if (head == system ) {
    head = head->next;
  }
  if (tail == system ) {
    tail = tail->previous;
  }
  if ( system->previous ) {
    system->previous->next = system->next;
  }
  if ( system->next != nullptr) {
    system->next->previous = system->previous;
  }
}

void SystemList::RemoveAll() {
  while (head != nullptr) {
    auto system = head;
    head = head->next;
    system->previous = nullptr;
    system->next = nullptr;
  }
  tail = nullptr;
}


System* SystemList::Get(const SystemType& type ) {
  for (auto system = head; system != nullptr; system = system->next ) {
    if (system->Is( type ) ) {
      return system;
    }
  }
  return nullptr;
}


SystemList::~SystemList() {
  while (head != nullptr) {
    auto s= head;
    head = head->next;
    delete s;
  }
}

SystemList::SystemList() {
  head= nullptr;
  tail= nullptr;
}

NS_END(ash)


