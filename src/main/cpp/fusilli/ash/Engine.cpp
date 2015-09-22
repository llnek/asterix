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
NS_ALIAS(s,std)
NS_BEGIN(ash)

//////////////////////////////////////////////////////////////////////////////
//
template <typename T>
void ObjList<T>::Add(not_null<T*> e ) {
  if (ENP(head)) {
    head = tail = e.get();
    head->previous= nullptr;
    head->next = nullptr;
  } else {
    tail->next = e.get();
    e->previous = tail;
    e->next= nullptr;
    tail = e.get();
  }
}

//////////////////////////////////////////////////////////////////////////////
//
template <typename T>
void ObjList<T>::Release(not_null<T*> p) {
  auto e= p.get();
  if (head == e) {
    head = head->next;
  }
  if (tail == e) {
    tail = tail->previous;
  }
  if (e->previous ) {
    e->previous->next = e->next;
  }
  if (e->next) {
    e->next->previous = e->previous;
  }
  e->previous = nullptr;
  e->next = nullptr;
}

//////////////////////////////////////////////////////////////////////////////
//
template <typename T>
void ObjList<T>::Clear() {
  while (NNP(head)) {
    auto e= head;
    head = head->next;
    delete e;
  }
  head = tail = nullptr;
}

//////////////////////////////////////////////////////////////////////////////
//
template <typename T>
const s::vector<T*> ObjList<T>::List() {
  s::vector<T*> v;
  for (auto p= head; NNP(p); p=p->next) {
    v.push_back(p);
  }
  return v;
}

//////////////////////////////////////////////////////////////////////////////
//
template <typename T>
ObjList<T>::~ObjList() {
  printf("ObjList dtor\n");
  Clear();
}

//////////////////////////////////////////////////////////////////////////////
//
template <typename T>
ObjList<T>::ObjList()
  : head(nullptr)
  , tail(nullptr) {
}

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
  for (auto it = nodeLists.begin(); it != nodeLists.end(); ++it) {
    delete *it;
  }
  nodeLists.clear();
  for (auto it= groups.begin();
      it != groups.end(); ++it) {
    delete it->second;
  }
  groups.clear();
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
  for (auto it = groups.begin(); it != groups.end(); ++it) {
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
Entity* Engine::CreateEntity(const stdstr& group) {
  auto e= Entity::Create(group, this);
  auto it= groups.find(group);
  ObjList<Entity>* el;
  if (it != groups.end()) {
    el= it->second;
  } else {
    el= new ObjList<Entity>();
    groups.insert(s::pair<stdstr, ObjList<Entity>*>(group,el));
  }
  el->Add(e);
  addList.push_back(e);
  return e;
}

//////////////////////////////////////////////////////////////////////////////
//
void Engine::NotifyModify(not_null<Entity*> e) {
  bool fnd=false;
  for (auto it= modList.begin();
      it != modList.end(); ++it) {
    if (e.get() == *it) { fnd=true; break; }
  }
  if (!fnd) {
    modList.push_back(e.get());
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Engine::PurgeEntity(not_null<Entity*> e) {
  auto it = groups.find(e->GroupId());
  if (it != groups.end()) {
    PurgeEntity(it->second,e.get());
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
  auto rego = NodeRegistry::GetInstance();
  NodeList* nl = NodeList::Create(nodeType);
  for (auto it = groups.begin(); it != groups.end(); ++it) {
    auto el= it->second;
    Node* n= nullptr;
    for (auto e= el->head; NNP(e); e=e->next) {
      if (ENP(n)) {
        n= rego->CreateNode(nodeType);
      }
      if (n->BindEntity(not_null<Entity*>(e))) {
        nl->Add(not_null<Node*>(n));
        n=nullptr;
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
  systemList.Add(s.get());
}

//////////////////////////////////////////////////////////////////////////////
//
void Engine::PurgeSystem(not_null<System*> s ) {
  systemList.Release(s.get());
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
  for (auto it = nodeLists.begin();
      it != nodeLists.end(); ++it) {
    auto nl= *it;
    nl->RemoveEntity(e);
  }
}

//////////////////////////////////////////////////////////////////////////////
// sync existing nodes, add if necessary
void Engine::OnAddEntity(Entity* e) {
  auto rego = NodeRegistry::GetInstance();
  for (auto it = nodeLists.begin();
      it != nodeLists.end(); ++it) {
    auto nl= *it;
    auto n= rego->CreateNode(nl->GetType());
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
  auto rego = NodeRegistry::GetInstance();
  for (auto it = nodeLists.begin();
      it != nodeLists.end(); ++it) {
    auto nl = *it;
    auto t = nl->GetType();
    Node* n;
    if (nl->ContainsWithin(e)) {
      if (!nl->IsCompatible(e)) {
        nl->RemoveEntity(e);
      }
    } else {
      n= rego->CreateNode(t);
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
void Engine::HouseKeeping() {
  for (auto it= freeList.begin();
      it != freeList.end(); ++it) {
    auto e = *it;
    OnPurgeEntity(e);
    delete e;
  }
  for (auto it= addList.begin();
      it != addList.end(); ++it) {
    auto e = *it;
    OnAddEntity(e);
  }
  for (auto it= modList.begin();
      it != modList.end(); ++it) {
    auto e= *it;
    OnModifyEntity(e);
  }
  freeList.clear();
  modList.clear();
  addList.clear();
}






NS_END(ash)


