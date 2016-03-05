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
NS_BEGIN(ecs)

//////////////////////////////////////////////////////////////////////////////
//
TypeRegistry::~TypeRegistry() {
  F__LOOP(it, rego) { delete it->second; }
  //printf("TypeRegistry dtor\n");
}

//////////////////////////////////////////////////////////////////////////////
//
const CompoCache* TypeRegistry::getCache(const COMType &c) {
  auto it= rego.find(c);
  if (it != rego.end()) {
    return it->second;
  } else {
    return nullptr;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void TypeRegistry::unbind(not_null<Component*> c, not_null<Entity*> e) {
  auto cid= c->typeId();
  auto eid= e->getEid();
  auto it= rego.find(cid);
  if (it != rego.end()) {
    auto m= it->second;
    auto it2= m->find(eid);
    if (it2 != m->end()) {
      m->erase(it2);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void TypeRegistry::bind(not_null<Component*> c, not_null<Entity*> e) {
  auto cid= c->typeId();
  auto eid= e->getEid();
  auto it= rego.find(cid);
  CompoCache *m;
  if (it != rego.end()) {
    m= it->second;
  } else {
    m= mc_new(CompoCache);
    rego.insert(S__PAIR(COMType,CompoCache*,cid,m));
  }
  m->insert(S__PAIR(EntId,Component*, eid, c));
}



NS_END



