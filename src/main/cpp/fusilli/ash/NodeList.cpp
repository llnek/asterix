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

#include "NodeRegistry.h"
#include "NodeList.h"
#include "Entity.h"
#include "Node.h"
NS_BEGIN(ash)


//////////////////////////////////////////////////////////////////////////////
//
NodeList::~NodeList() {
//  printf("NodeList dtor\n");
  Clear();
}

//////////////////////////////////////////////////////////////////////////////
//
NodeList::NodeList() {
  head = tail = nullptr;
}

//////////////////////////////////////////////////////////////////////////////
//
owner<NodeList*> NodeList::Create(const NodeType& t) {
  auto n = new NodeList();
  n->nType = t;
  return n;
}

//////////////////////////////////////////////////////////////////////////////
//
void NodeList::Add(not_null<Node*> n) {
  if (ENP(head)) {
    head = tail = n;
  } else {
    tail->next = n;
    n->previous = tail;
    n->next= nullptr;
    tail = n;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
int NodeList::Size() {
  auto n= head;
  int c=0;
  while (NNP(n)) {
    n = n->next;
    ++c;
  }
  return c;
}

//////////////////////////////////////////////////////////////////////////////
//
void NodeList::Purge(not_null<Node*> node) {
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
  delete node.get();
}

//////////////////////////////////////////////////////////////////////////////
//
void NodeList::Clear() {
  while (NNP(head)) {
    auto n = head;
    head = head->next;
    delete n;
  }
  head = nullptr;
  tail = nullptr;
}

//////////////////////////////////////////////////////////////////////////////
//
bool NodeList::IsEmpty() {
  return ENP(head);
}

//////////////////////////////////////////////////////////////////////////////
//
void NodeList::RemoveEntity(not_null<Entity*> e) {
  Node* n = head;
  Node* c;
  while (NNP(n)) {
    if (n->BelongsTo(e)) {
      c= n->next;
      Purge(n);
      n=c;
    } else {
      n = n->next;
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
bool NodeList::ContainsWithin(not_null<Entity*> e) {
  for (auto n= head; NNP(n); n = n->next) {
    if (n->BelongsTo(e)) { return true; }
  }
  return false;
}

//////////////////////////////////////////////////////////////////////////////
//
bool NodeList::IsCompatible(not_null<Entity*> e) {
  auto rego = NodeRegistry::GetInstance();
  auto n = rego->CreateNode(nType);
  auto ok= n->BindEntity(e);
  delete n;
  return ok;
}








NS_END(ash)


