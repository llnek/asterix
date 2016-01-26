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

#include "NodeRego.h"
#include "NodeList.h"
#include "Entity.h"
#include "Node.h"
NS_BEGIN(ash)

//////////////////////////////////////////////////////////////////////////////
//
NodeList::NodeList(const NodeType &t) {
  this->nType = t;
}

//////////////////////////////////////////////////////////////////////////////
//
void NodeList::removeEntity(not_null<Entity*> e) {
  Node *n = head;
  Node *c;
  while (NNP(n)) {
    if (n->belongsTo(e)) {
      c= n->next;
      purge(n);
      n=c;
    } else {
      n = n->next;
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
bool NodeList::containsWithin(not_null<Entity*> e) {
  for (auto n= head; NNP(n); n = n->next) {
    if (n->belongsTo(e)) { return true; }
  }
  return false;
}

//////////////////////////////////////////////////////////////////////////////
//
bool NodeList::isCompatible(not_null<Entity*> e) {
  auto rego = NodeRegistry::self();
  auto n = rego->reifyNode(nType);
  auto ok= n->bindEntity(e);
  delete n;
  return ok;
}








NS_END


