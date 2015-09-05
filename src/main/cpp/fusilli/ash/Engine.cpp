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
using namespace std;
NS_BEGIN(Ash)

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

Engine::Engine() {
  updating= false;
}

const vector<Entity*> Engine::GetEntities() {
  return entityList.List();
}

const vector<Entity*> Engine::GetSystems() {
  return systemList.List();
}

void Engine::AddEntity(Entity* e) {
  e->componentAdded.Add(&Engine::ComponentAdded, this);
  entityList.Add(e);
  for (auto it = families.begin(); it != families.end(); ++it) {
    it->second->NewEntity(e);
  }
}

void Engine::RemoveEntity(Entity* e) {
  for (auto it = families.begin(); it != families.end(); ++it) {
    it->second->RemoveEntity(e);
  }
  entityList.Remove(e);
  e->componentAdded.Remove(&Engine::ComponentAdded, this );
}

void Engine::RemoveAllEntities() {
  while (this.entityList.head != nullptr) {
    RemoveEntity(entityList.head);
  }
}

void Engine::ComponentAdded(Entity* e, const string& componentClass) {
  for (auto it = families.begin(); it != families.end(); ++it) {
    it->second->ComponentAddedToEntity(e, componentClass);
  }
}

NodeList* Engine::GetNodeList(const string& nodeType) {
  auto it = families.find(nodeType);
  Family* fam;
  if (it != families.end()) {
    fam = it->second;
  } else {
    fam = NewFamily(nodeType);
  }
  return fam->nodes;
}

Family* Engine::NewFamily(const string& nodeType) {
  auto fam= new MatchingFamily(nodeType, this);
  families.insert(pair<string,Family>(nodeType, fam));
  for (auto e = entityList->head; e != nullptr; e = e->next) {
    fam->NewEntity(e);
  }
  return fam;
}

void Engine::ReleaseNodeList(const string& nodeType) {
  auto it = families.find(nodeType);
  Family* fam;
  if (it != families.end()) {
    fam = it->second;
    fam->CleanUp();
    families.erase(it);
    delete fam;
  }
}

void Engine::AddSystem(System* s) {
  s->AddToEngine( this );
  systemList.Add(s);
}

System* Engine::GetSystem(const string& type) {
  return systemList.Get(type);
}

void Engine::RemoveSystem(System* s ) {
  systemList.Remove(s);
  s->RemoveFromEngine( this );
}

void Engine::RemoveAllSystems() {
  while (systemList.head ) {
    RemoveSystem(systemList.head);
  }
}

void Engine::Update(float time) {
  updating = true;
  for (auto s= systemList.head; s != nullptr; s= s->next) {
    s->Update(time);
  }
  updating = false;
  updateComplete.Dispatch();
}



NS_END(Ash)


