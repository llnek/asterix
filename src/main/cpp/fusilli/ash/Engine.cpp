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

#include "Engine.h"
#include "Entity.h"
#include "System.h"
NS_USING(std)
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
}

//////////////////////////////////////////////////////////////////////////////
//
Engine::Engine() {
  updating= false;
}

//////////////////////////////////////////////////////////////////////////////
//
const vector<Entity*> Engine::GetEntities() {
  return entityList.List();
}

//////////////////////////////////////////////////////////////////////////////
//
const vector<System*> Engine::GetSystems() {
  return systemList.List();
}

//////////////////////////////////////////////////////////////////////////////
//
void Engine::AddEntity(Entity* e) {
  entityList.Add(e);
  OnAddEntity(e);
}

//////////////////////////////////////////////////////////////////////////////
//
void Engine::RemoveEntity(Entity* e) {
  entityList.Remove(e);
  OnRemoveEntity(e);
}

//////////////////////////////////////////////////////////////////////////////
//
void Engine::RemoveEntities() {
  while (entityList.head != nullptr) {
    RemoveEntity(entityList.head);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Engine::OnRemoveEntity(Entity* e) {
  for (auto it = nodeLists.begin(); it != nodeLists.end(); ++it) {
    auto nl= *it;
    nl->RemoveEntity(e);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Engine::OnAddEntity(Entity* e) {
  auto rego = NodeRegistry::GetInstance();
  for (auto it = nodeLists.begin(); it != nodeLists.end(); ++it) {
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
//
NodeList* Engine::GetNodeList(const NodeType& nodeType) {
  auto rego = NodeRegistry::GetInstance();
  auto nl = new NodeList();
  Node* n= nullptr;
  for (auto e= entityList.head; e != nullptr; e=e->next) {
    if (n==nullptr) {
      n= rego->CreateNode(nodeType);
    }
    if (n->BindEntity(e)) {
      nl->Add(n);
      n=nullptr;
    }
  }
  nodeLists.push_back(nl);
  return nl;
}

//////////////////////////////////////////////////////////////////////////////
//
void Engine::ReleaseNodeList(NodeList*& nl) {
  for (auto it=nodeLists.begin(); it != nodeLists.end(); ++it) {
    if (nl == *it) {
      nodeLists.erase(it);
      break;
    }
  }
  delete nl;
  nl=nullptr;
}

//////////////////////////////////////////////////////////////////////////////
//
void Engine::AddSystem(System* s) {
  s->AddToEngine( this );
  systemList.Add(s);
}

//////////////////////////////////////////////////////////////////////////////
//
System* Engine::GetSystem(const SystemType& type) {
  return systemList.Get(type);
}

//////////////////////////////////////////////////////////////////////////////
//
void Engine::RemoveSystem(System* s ) {
  systemList.Remove(s);
  s->RemoveFromEngine( this );
}

//////////////////////////////////////////////////////////////////////////////
//
void Engine::RemoveSystems() {
  while (systemList.head ) {
    RemoveSystem(systemList.head);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Engine::Update(float time) {
  updating = true;
  for (auto s= systemList.head; s != nullptr; s= s->next) {
    if (s->IsActive()) {
      s->Update(time);
    }
  }
  updating = false;
}



NS_END(ash)


