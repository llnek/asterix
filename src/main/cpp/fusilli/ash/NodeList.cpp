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
NS_BEGIN(Ash)

NodeList::NodeList() {
  head = nullptr;
  tail = nullptr;
  //this.nodeAdded = new signals.Signal();
  //this.nodeRemoved = new signals.Signal();
}

void NodeList::Add(Node* n) {
  if ( !head ) {
    head = tail = n;
  } else {
    tail->next = n;
    n->previous = tail;
    tail = n;
  }
  //this.nodeAdded.dispatch( n);
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
  //this.nodeRemoved.dispatch( node );
}

void NodeList::RemoveAll() {
  while (head != nullptr) {
    auto node = head;
    head = node->next;
    node->previous = nullptr;
    node->next = nullptr;
    //this.nodeRemoved.dispatch( node );
  }
  tail = nullptr;
}

boolNodeList::IsEmpty() {
  return head == nullptr;
}

void NodeList::Swap(Node* node1, Node* node2 ) {
  if (node1->previous == node2 ) {
    node1->previous = node2->previous;
    node2->previous = node1;
    node2->next = node1->next;
    node1->next = node2;
  } else if (node2->previous == node1 ) {
    node2->previous = node1->previous;
    node1->previous = node2;
    node1->next = node2->next;
    node2->next = node1;
  } else {
    auto temp = node1->previous;
    node1->previous = node2->previous;
    node2->previous = temp;
    temp = node1->next;
    node1->next = node2->next;
    node2->next = temp;
  }
  if (head == node1 ) {
    head = node2;
  } else if (head == node2 ) {
    head = node1;
  }
  if(tail == node1 ) {
    tail = node2;
  } else if( tail == node2 ) {
    tail = node1;
  }
  if( node1->previous ) {
    node1->previous->next = node1;
  }
  if( node2->previous ) {
    node2->previous->next = node2;
  }
  if( node1->next ) {
      node1->next->previous = node1;
  }
  if( node2->next ) {
      node2->next->previous = node2;
  }
}

void NodeList::InsertionSort( sortFunction ) {
  if (head == tail ) { return; }
  auto remains = head->next;
  for( auto node = remains; node != nullptr; node = remains ) {
    remains = node->next;
    for( auto other = node->previous; other != nullptr; other = other->previous ) {
      if( sortFunction( node, other ) >= 0 ) {
        if( node != other->next ) {
          if( tail == node ) {
            tail = node->previous;
          }
          node->previous->next = node->next;
          if( node->next ) {
            node->next->previous = node->previous;
          }
          node->next = other->next;
          node->previous = other;
          node->next->previous = node;
          other->next = node;
        }
        break;
      }
    }
    if ( !other ) {
      if( tail == node ) {
        tail = node->previous;
      }
      node->previous->next = node->next;
      if( node->next ) {
        node->next->previous = node->previous;
      }
      node->next = head;
      head->previous = node;
      node->previous = nullptr;
      head = node;
    }
  }
}

void NodeList::MergeSort( sortFunction ) {
  if( head == tail ) { return; }
  auto lists = [],
  Node* start = head;
  Node* end;
  while ( start != nullptr ) {
    end = start;
    while( end->next != nullptr &&
        sortFunction( end, end->next ) <= 0 ) {
      end = end->next;
    }
    auto next = end->next;
    start->previous = end->next = nullptr;
    lists.push( start );
    start = next;
  }
  while( lists.length > 1 ) {
    lists.push( Merge( lists.shift(), lists.shift(), sortFunction ) );
  }
  tail = head = lists[0];
  while( tail->next != nullptr ) {
    tail = tail->next;
  }
}

Node* NodeList::Merge(Node* head1, Node* head2, sortFunction ) {
  var node,
      head;
  if (sortFunction(head1, head2 ) <= 0 ) {
    head = node = head1;
    head1 = head1->next;
  } else {
    head = node = head2;
    head2 = head2->next;
  }
  while( head1 != nullptr &&
      head2 != nullptr) {
    if (sortFunction(head1, head2 ) <= 0 ) {
      node->next = head1;
      head1->previous = node;
      node = head1;
      head1 = head1->next;
    } else {
      node->next = head2;
      head2->previous = node;
      node = head2;
      head2 = head2->next;
    }
  }
  if (head1 != nullptr) {
    node->next = head1;
    head1->previous = node;
  } else {
    node->next = head2;
    head2->previous = node;
  }
  return head;
}



NS_END(Ash)


