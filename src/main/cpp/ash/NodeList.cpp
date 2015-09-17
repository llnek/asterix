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


//////////////////////////////////////////////////////////////////////////////
//
NodeList::~NodeList() {
}

//////////////////////////////////////////////////////////////////////////////
//
NodeList::NodeList()
  : head(nullptr)
  , tail(nullptr) {
}

//////////////////////////////////////////////////////////////////////////////
//
void NodeList::Add(Node* n) {
  if ( !head ) {
    head = tail = n;
  } else {
    tail->next = n;
    n->previous = tail;
    tail = n;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
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

//////////////////////////////////////////////////////////////////////////////
//
void NodeList::RemoveAll() {
  while (NNP(head)) {
    auto node = head;
    head = node->next;
    node->previous = nullptr;
    node->next = nullptr;
  }
  tail = nullptr;
}

//////////////////////////////////////////////////////////////////////////////
//
bool NodeList::IsEmpty() {
  return ENP(head);
}

//////////////////////////////////////////////////////////////////////////////
//
void NodeList::RemoveEntity(Entity* e) {
  Node* n = head;
  while (NNP(n)) {
    if (n->BelongsTo(e)) {
      Remove(n);
      delete n;
      n=head;
    } else {
      n = n->next;
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
bool NodeList::ContainsWithin(Entity* e) {
  for (auto n= head; NNP(n); n = n->next) {
    if (n->BelongsTo(e)) { return true; }
  }
  return false;
}

//////////////////////////////////////////////////////////////////////////////
//
bool NodeList::IsCompatible(Entity* e) {
  auto rego = NodeRegistry::GetInstance();
  auto n = rego->CreateNode(nType);
  auto ok= n->BindEntity(e);
  delete n;
  if (! ok)  {
    RemoveEntity(e);
  }
  return ok;
}








NS_END(ash)


