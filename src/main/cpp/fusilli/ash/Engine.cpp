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
  F__LOOP(it, nodeLists) {
    delete *it;
  }
  F__LOOP(it, groups) {
    delete it->second;
  }
//  printf("Engine dtor\n");
}

//////////////////////////////////////////////////////////////////////////////
//
Engine::Engine() {
  updating=false;
  dirty=false;
}

//////////////////////////////////////////////////////////////////////////////
//
const s::vector<Entity*>
Engine::GetEntities(const stdstr& group) {
  auto it=groups.find(group);
  s::vector<Entity*> v;
  if (it != groups.end()) {
    return it->second->List();
  } else {
    return v;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
const s::vector<Entity*>
Engine::GetEntities() {
  s::vector<Entity*> rc;
  F__LOOP(it, groups) {
    auto v= it->second->List();
    rc.insert(rc.end(), v.begin(), v.end());
  }
  return rc;
}

//////////////////////////////////////////////////////////////////////////////
//
const s::vector<System*> Engine::GetSystems() {
  return systemList.List();
}

//////////////////////////////////////////////////////////////////////////////
//
Entity* Engine::ReifyEntity(const stdstr& group) {
  auto e= Entity::Reify(group, this);
  auto it= groups.find(group);
  ObjList<Entity>* el;
  if (it != groups.end()) {
    el= it->second;
  } else {
    el= mc_new(ObjList<Entity>);
    groups.insert(S__PAIR(stdstr, ObjList<Entity>*, group, el));
  }
  el->Add(e);
  dirty=true;
  addList.push_back(e);
  return e;
}

//////////////////////////////////////////////////////////////////////////////
//
void Engine::NotifyModify(not_null<Entity*> e) {
  bool fnd=false;
  F__LOOP(it, modList) {
    if (e == *it) { fnd=true; break; }
  }
  if (!fnd) {
    dirty=true;
    modList.push_back(e);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Engine::PurgeEntity(not_null<Entity*> e) {
  auto it = groups.find(e->GroupId());
  if (it != groups.end()) {
    PurgeEntity(it->second,e);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Engine::PurgeEntity(ObjList<Entity>* el, Entity* e) {
  e->MarkDelete();
  el->Release(e);
  dirty=true;
  freeList.push_back(e);
}

//////////////////////////////////////////////////////////////////////////////
//
void Engine::PurgeEntities(const stdstr& group) {
  auto it = groups.find(group);
  if (it != groups.end()) {
    auto el=it->second;
    while (NNP(el->head)) {
      PurgeEntity(el, el->head);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
NodeList* Engine::GetNodeList( const NodeType& nodeType) {
  auto nl = NodeList::Reify(nodeType);
  auto rego = NodeRegistry::Self();
  F__LOOP(it, groups) {
    auto el= it->second;
    Node* n= nullptr;
    for (auto e= el->head; NNP(e); e=e->next) {
      if (ENP(n)) {
        n= rego->ReifyNode(nodeType);
      }
      if (n->BindEntity(e)) {
        nl->Add(n);
        SNPTR(n)
      }
    }
  }
  nodeLists.push_back(nl);
  return nl;
}

//////////////////////////////////////////////////////////////////////////////
//
void Engine::RegoSystem(not_null<System*> s) {
  s->AddToEngine( this );
  systemList.Add(s);
}

//////////////////////////////////////////////////////////////////////////////
//
void Engine::PurgeSystem(not_null<System*> s ) {
  systemList.Release(s);
  s->RemoveFromEngine(this);
  delete s;
}

//////////////////////////////////////////////////////////////////////////////
//
void Engine::PurgeSystems() {
  while (NNP(systemList.head)) {
    PurgeSystem(systemList.head);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Engine::Update(float time) {
  updating = true;
  for (auto s= systemList.head; NNP(s); s= s->next) {
    if (s->IsActive()) {
      if (! s->Update(time)) { break; }
    }
  }
  if (dirty) {
    HouseKeeping();
  }
  dirty=false;
  updating = false;
}

//////////////////////////////////////////////////////////////////////////////
// get rid of nodes bound to this entity
void Engine::OnPurgeEntity(Entity* e) {
  F__LOOP(it, nodeLists) {
    auto nl= *it;
    nl->RemoveEntity(e);
  }
}

//////////////////////////////////////////////////////////////////////////////
// sync existing nodes, add if necessary
void Engine::OnAddEntity(Entity* e) {
  auto rego = NodeRegistry::Self();
  F__LOOP(it, nodeLists) {
    auto nl= *it;
    auto n= rego->ReifyNode(nl->GetType());
    if (n->BindEntity(e)) {
      nl->Add(n);
    } else {
      delete n;
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
// sync changes
void Engine::OnModifyEntity(Entity* e) {
  auto rego = NodeRegistry::Self();
  F__LOOP(it, nodeLists) {
    auto nl = *it;
    auto t = nl->GetType();
    Node* n;
    if (nl->ContainsWithin(e)) {
      if (!nl->IsCompatible(e)) {
        nl->RemoveEntity(e);
      }
    } else {
      n= rego->ReifyNode(t);
      if (n->BindEntity(e)) {
        nl->Add(n);
      } else {
        delete n;
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Engine::ForceSync() {
  HouseKeeping();
}

//////////////////////////////////////////////////////////////////////////////
//
void Engine::HouseKeeping() {
  F__LOOP(it, freeList) {
    auto e = *it;
    OnPurgeEntity(e);
    delete e;
  }
  F__LOOP(it, addList) {
    auto e = *it;
    OnAddEntity(e);
  }
  F__LOOP(it, modList) {
    auto e= *it;
    OnModifyEntity(e);
  }
  freeList.clear();
  modList.clear();
  addList.clear();
}






NS_END(ash)


