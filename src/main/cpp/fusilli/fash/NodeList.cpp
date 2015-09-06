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

#include "NodeList.h"
NS_BEGIN(ash)



NodeList::~NodeList() {
}

NodeList::NodeList() {
  head = nullptr;
  tail = nullptr;
}

void NodeList::Add(Node* n) {
  if ( !head ) {
    head = tail = n;
  } else {
    tail->next = n;
    n->previous = tail;
    tail = n;
  }
}

void NodeList::Remove(Node* node) {
  if (head == node ) {
    head = head->next;
  }
  if (tail == node ) {
    tail = tail->previous;
  }
  if (node->previous ) {
    node->previous->next = node->next;
  }
  if ( node->next ) {
    node->next->previous = node->previous;
  }
}

void NodeList::RemoveAll() {
  while (head != nullptr) {
    auto node = head;
    head = node->next;
    node->previous = nullptr;
    node->next = nullptr;
  }
  tail = nullptr;
}

bool NodeList::IsEmpty() {
  return head == nullptr;
}

void NodeList::RemoveEntity(Entity* e) {
  Node* p = head;
  while (p != nullptr) {
    if (p->BelongsTo(e)) {
      Remove(p);
      delete p;
      p=head;
    } else {
      p = p->next;
    }
  }
}

bool NodeList::ContainsWithin(Entity* e) {
  for (auto p= head; p != nullptr; p = p->next) {
    if (p->BelongsTo(e)) { return true; }
  }
  return false;
}

bool NodeList::CompliesWith(Entity* e) {
  auto rego = NodeRegistry::GetInstance();
  auto n = rego->CreateNode(nType)
  auto ok= n->BindEntity(e);
  delete n;
  if (! ok)  {
    RemoveEntity(e);
  }
}








NS_END(ash)


