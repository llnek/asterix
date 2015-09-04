#include "NodeList.h"


NodeList::NodeList() {
  m_head = nullptr;
  m_tail = nullptr;
  //this.nodeAdded = new signals.Signal();
  //this.nodeRemoved = new signals.Signal();
}

NodeList::Add(Node* node ) {
  if( ! m_head) {
    m_head = m_tail = node;
  } else {
    m_tail->next = node;
    node->previous = m_tail;
    m_tail = node;
  }
  //this.nodeAdded.dispatch( node );
}

NodeList::Remove(Node* node ) {
  if( m_head == node ) {
    m_head = m_head->next;
  }
  if( m_tail == node ) {
    m_tail = m_tail->previous;
  }
  if( node->previous ) {
    node->previous->next = node->next;
  }
  if( node.next ) {
    node->next->previous = node->previous;
  }
  //this.nodeRemoved.dispatch( node );
}

NodeList::RemoveAll() {
  while( m_head ) {
    auto node = m_head;
    m_head = node->next;
    node->previous = nullptr;
    node->next = nullptr;
    //this.nodeRemoved.dispatch( node );
  }
  m_tail = nullptr;
}

bool NodeList::Empty() {
  return m_head == nullptr;
}

void NodeList::Swap(Node* node1, Node* node2 ) {
  if( node1->previous == node2 ) {
    node1->previous = node2->previous;
    node2->previous = node1;
    node2->next = node1->next;
    node1->next = node2;
  } else if( node2->previous == node1 ) {
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
  if( m_head == node1 ) {
    m_head = node2;
  } else if( m_head == node2 ) {
    m_head = node1;
  }
  if( m_tail == node1 ) {
    m_tail = node2;
  } else if( m_tail == node2 ) {
    m_tail = node1;
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
  if( m_head == m_tail ) {
    return;
  }
  auto remains = m_head->next;
  for (auto node = remains; node; node = remains) {
    remains = node->next;
    for (auto other = node->previous; other; other = other->previous) {
      if ( sortFunction( node, other ) >= 0 ) {
        if ( node != other->next ) {
            if ( m_tail == node ) {
              m_tail = node->previous;
            }
            node->previous->next = node->next;
            if ( node.next ) {
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
    if( !other ) {
      if( m_tail == node ) {
        m_tail = node->previous;
      }
      node->previous->next = node->next;
      if( node->next ) {
        node->next->previous = node->previous;
      }
      node->next = m_head;
      m_head->previous = node;
      node->previous = nullptr;
      m_head = node;
    }
  }
}

void NodeList::MergeSort( sortFunction ) {
  if( m_head == m_tail ) {
    return;
  }
  Node* start = m_head;
  Node* end;
  Array lists;
  while( start ) {
    end = start;
    while ( end->next != nullptr &&
        sortFunction( end, end->next ) <= 0 ) {
      end = end->next;
    }
    auto next = end->next;
    start->previous = end->next = nullptr;
    lists.addObject( start );
    start = next;
  }
  while( lists.count() > 1 ) {
    lists.addObject( Merge( lists.removeObjectAtIndex(0),
                            lists.removeObjectAtIndex(0), sortFunction ) );
  }
  m_tail = m_head = lists.getObjectAtIndex(0);
  while( m_tail->next ) {
    m_tail = m_tail->next;
  }
  lists.removeAllObjects();
}

Node* NodeList::Merge(Node* head1, Node* head2, sortFunction) {
  Node* node, head;
  if ( sortFunction( head1, head2 ) <= 0 ) {
    head = node = head1;
    head1 = head1->next;
  } else {
    head = node = head2;
    head2 = head2->next;
  }
  while( head1 && head2 ) {
    if( sortFunction( head1, head2 ) <= 0 ) {
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
  if( head1 ) {
    node->next = head1;
    head1->previous = node;
  } else {
    node->next = head2;
    head2->previous = node;
  }
  return head;
}

