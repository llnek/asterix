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

#include "NodeRego.h"
#include "Node.h"
#include "Engine.h"
NS_BEGIN(ash)

  /*
A* myA = new A;
Will_Get_Function_Pointer(1.00,2.00, &myA->Minus)
You cannot. You should use something like

void Will_Get_Function_Pointer(A* object, float a, float b,
float (A::*pt2Func)(float, float))
{
   (object->*pt2Func)(a, b);
}
And calls it as

A* myA = new A;
Will_Get_Function_Pointer(myA, 1.00, 2.00, &A::Minus);
*/

//////////////////////////////////////////////////////////////////////////////
//
Engine::~Engine() {
  F__LOOP(it, groups) { delete it->second; }
  F__LOOP(it, nodeLists) { delete *it; }
//  printf("Engine dtor\n");
}

//////////////////////////////////////////////////////////////////////////////
//
Engine::Engine() {
}

//////////////////////////////////////////////////////////////////////////////
//
const s_vec<Entity*> Engine::getEntities(const sstr &g) {
  auto it=groups.find(g);
  s_vec<Entity*> v;
  if (it != groups.end()) {
    return it->second->list();
  } else {
    return v;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
const s_vec<Entity*> Engine::getEntities() {
  s_vec<Entity*> rc;
  F__LOOP(it, groups) {
    auto v= it->second->list();
    rc.insert(rc.end(), v.begin(), v.end());
  }
  return rc;
}

//////////////////////////////////////////////////////////////////////////////
//
Entity* Engine::reifyEntity(const sstr &g) {
  auto e= mc_new_2(Entity,g, this);
  auto it= groups.find(g);
  EList *el;
  if (it != groups.end()) {
    el= it->second;
  } else {
    el= mc_new(EList);
    groups.insert(S__PAIR(sstr, EList*, g, el));
  }
  el->add(e);
  dirty=true;
  addList.push_back(e);
  return e;
}

//////////////////////////////////////////////////////////////////////////////
//
void Engine::notifyModify(not_null<Entity*> e) {
  bool fnd=false;
  F__LOOP(it, modList) {
    if (e == *it)
    { fnd=true; break; }
  }
  if (!fnd) {
    modList.push_back(e);
    dirty=true;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Engine::purgeEntity(not_null<Entity*> e) {
  auto it = groups.find(e->groupId());
  if (it != groups.end()) {
    purgeEntity(it->second, e);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Engine::purgeEntity(EList* el, Entity* e) {
  freeList.push_back(e);
  e->markDelete();
  el->release(e);
  dirty=true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Engine::purgeEntities(const sstr &g) {
  auto it = groups.find(g);
  if (it != groups.end()) {
    auto el=it->second;
    while (NNP(el->head)) {
      purgeEntity(el, el->head);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
NodeList* Engine::getNodeList( const NodeType& nodeType) {
  auto nl = mc_new_1(NodeList, nodeType);
  auto rego = NodeRegistry::self();
  F__LOOP(it, groups) {
    auto el= it->second;
    Node *n= nullptr;
    for (auto e= el->head; NNP(e); e=e->next) {
      if (ENP(n)) {
        n= rego->reifyNode(nodeType);
      }
      if (n->bindEntity(e)) {
        nl->add(n);
        SNPTR(n)
      }
    }
  }
  nodeLists.push_back(nl);
  return nl;
}

//////////////////////////////////////////////////////////////////////////////
//
void Engine::regoSystem(not_null<System*> s) {
  s->addToEngine( this );
  systemList.add(s);
}

//////////////////////////////////////////////////////////////////////////////
//
void Engine::purgeSystem(not_null<System*> s ) {
  s->removeFromEngine(this);
  systemList.remove(s);
}

//////////////////////////////////////////////////////////////////////////////
//
void Engine::purgeSystems() {
  systemList.removeAll();
}

//////////////////////////////////////////////////////////////////////////////
//
void Engine::update(float time) {
  updating = true;
  for (auto s= systemList.head; NNP(s); s= s->next) {
    if (s->isActive()) {
      if (! s->update(time)) { break; }
    }
  }
  if (dirty) { houseKeeping(); }
  dirty=false;
  updating = false;
}

//////////////////////////////////////////////////////////////////////////////
// get rid of nodes bound to this entity
void Engine::onPurgeEntity(Entity *e) {
  F__LOOP(it, nodeLists) {
    auto nl= *it;
    nl->removeEntity(e);
  }
}

//////////////////////////////////////////////////////////////////////////////
// sync existing nodes, add if necessary
void Engine::onAddEntity(Entity *e) {
  F__LOOP(it, nodeLists) {
    maybeBind(*it, e);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Engine::maybeBind(NodeList* nl, Entity *e) {
  auto rego = NodeRegistry::self();
  auto n= rego->reifyNode(nl->getType());
  if (n->bindEntity(e)) {
    nl->add(n);
  } else {
    delete n;
  }
}

//////////////////////////////////////////////////////////////////////////////
// sync changes
void Engine::onModifyEntity(Entity *e) {
  F__LOOP(it, nodeLists) {
    auto nl = *it;
    if (nl->containsWithin(e)) {
      if (!nl->isCompatible(e)) {
        nl->removeEntity(e);
      }
    } else {
      maybeBind(nl,e);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Engine::forceSync() {
  houseKeeping();
}

//////////////////////////////////////////////////////////////////////////////
//
void Engine::houseKeeping() {
  F__LOOP(it, freeList) {
    auto e = *it;
    onPurgeEntity(e);
    delete e;
  }
  F__LOOP(it, addList) {
    auto e = *it;
    onAddEntity(e);
  }
  F__LOOP(it, modList) {
    auto e= *it;
    onModifyEntity(e);
  }
  freeList.clear();
  modList.clear();
  addList.clear();
}






NS_END(ash)


