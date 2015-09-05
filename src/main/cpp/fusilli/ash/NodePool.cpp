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

#include "NodePool.h"
NS_BEGIN(Ash)



NodePool::NodePool(nodeClass, components) {
  this->components = components;
  this->nodeClass= nodeClass;
  cacheTail= nullptr;
  tail= nullptr;
}

Node* NodePool::Get() {
  if (tail != nullptr) {
    auto node = tail;
    tail = tail->previous;
    node->previous = nullptr;
    return node;
  } else {
    return new this.nodeClass();
  }
}

void NodePool::Dispose(Node* node ) {
  DictElement* pe;
  CCDICT_FOREACH(components, pe) {
    const string n= static_cast<String*>(pe->getObject())->getCString();
    node->RemoveField(n);
  }
  node->entity = nullptr;
  node->next = nullptr;
  node->previous = tail;
  tail = node;
}

void NodePool::Cache(Node* node) {
  node->previous = cacheTail;
  cacheTail = node;
}

void NodePool::ReleaseCache() {
  while (cacheTail != nullptr) {
    auto node = cacheTail;
    cacheTail = node->previous;
    Dispose(node);
  }
}




NS_END(Ash)

