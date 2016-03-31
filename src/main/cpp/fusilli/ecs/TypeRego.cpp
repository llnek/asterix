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

#include "TypeRego.h"
#include "Node.h"
NS_BEGIN(ecs)

//////////////////////////////////////////////////////////////////////////////
//
TypeRegistry::~TypeRegistry() {
  //printf("TypeRegistry dtor\n");
  //shallow delete the caches
  F__LOOP(it, _rego) { delete it->second; }
}

//////////////////////////////////////////////////////////////////////////////
//
CompoCache* TypeRegistry::getCache(const COMType &c) {
  auto it= _rego.find(c);
  if (it != _rego.end()) {
    return it->second;
  } else {
    return P_NIL;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void TypeRegistry::unbind(not_null<Component*> c, not_null<Node*> e) {
  unbind( c->typeId(), e);
}

//////////////////////////////////////////////////////////////////////////////
//
void TypeRegistry::unbind(const COMType &cid, not_null<Node*> e) {
  auto it= _rego.find(cid);
  if (it != _rego.end()) {
    auto eid= e->getEid();
    auto m= it->second;
    auto it2= m->find(eid);
    if (it2 != m->end()) {
      m->erase(it2);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void TypeRegistry::bind(not_null<Component*> c, not_null<Node*> e) {
  auto cid= c->typeId();
  auto eid= e->getEid();
  CompoCache *m;

  auto it= _rego.find(cid);
  if (it != _rego.end()) {
    m= it->second;
  } else {
    m= mc_new(CompoCache);
    _rego.insert(S__PAIR(COMType,CompoCache*,cid,m));
  }
  m->insert(S__PAIR(NodeId,Component*, eid, c));
}



NS_END



