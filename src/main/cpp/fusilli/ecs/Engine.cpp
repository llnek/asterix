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
#include "Engine.h"
NS_BEGIN(ecs)

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
  F__LOOP(it, ents) { delete it->second; }
  doHouseKeeping();
  delete types;
//  printf("Engine dtor\n");
}

//////////////////////////////////////////////////////////////////////////////
//
Engine::Engine() {
  types= mc_new(TypeRegistry);
}

//////////////////////////////////////////////////////////////////////////////
//
void Engine::getEntities(const s_vec<COMType> &cs, s_vec<Entity*> &rc) {
  s_vec<CompoCache*> ccs;
  bool missed=false;
  int pmin= INT_MAX;
  CompoCache *pm= nullptr;

  //find shortest cache
  F__LOOP(it,cs) {
    auto cid= *it;
    auto c= types->getCache(cid);
    if (!c) {
      CCLOG("cache missed when looking for intersection on %s", cid.c_str());
      missed=true;
      break;
    }
    if (c->size() < pmin) {
      pmin= c->size();
      pm=c;
    }
    ccs.push_back(c);
  }

  CCLOG("intesection on %d caches", (int)ccs.size());

  if (ccs.size() > 0) {
    F__POOP(it,pm) {
      auto eid= it->first;
      auto sum=0;

      // look for intersection
      F__LOOP(it2,ccs) {
        auto c= *it2;
        if (c==pm) { ++sum; continue;}
        auto it3= c->find(eid);
        if (it3 != c->end()) {
          ++sum;
        }
      }

      // if found in all caches...
      if (sum == ccs.size()) {
        // all matched
        auto it4= ents.find(eid);
        if (it4 != ents.end()) {
          rc.push_back(it4->second);
        }
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
const s_vec<Entity*> Engine::getEntities(const s_vec<COMType> &cs) {
  s_vec<Entity*> rc;
  getEntities(cs, rc);
  return rc;
}

//////////////////////////////////////////////////////////////////////////////
//
void Engine::getEntities(const COMType &c, s_vec<Entity*> &rc) {
  auto cc= types->getCache(c);
  if (NNP(cc)) F__POOP(it,cc) {
    auto z= it->first;
    auto it2= ents.find(z);
    if (it2 != ents.end()) {
      rc.push_back(it2->second);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
const s_vec<Entity*> Engine::getEntities(const COMType &c) {
  s_vec<Entity*> rc;
  getEntities(c,rc);
  return rc;
}


//////////////////////////////////////////////////////////////////////////////
//
void Engine::getEntities(s_vec<Entity*> &rc) {
  F__LOOP(it, ents) {
    rc.push_back(it->second);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
const s_vec<Entity*> Engine::getEntities() {
  s_vec<Entity*> rc;
  getEntities(rc);
  return rc;
}

//////////////////////////////////////////////////////////////////////////////
//
EntId Engine::generateEid() {
  auto rc= ++lastId;
  if (rc < INT_MAX) {} else {
    throw "too many entities";
  }
  return rc;
}

//////////////////////////////////////////////////////////////////////////////
//
Entity* Engine::reifyEntity() {
  auto eid= this->generateEid();
  auto e= mc_new2(Entity,this, eid);
  ents.insert(S__PAIR(EntId,Entity*,eid,e));
  return e;
}

//////////////////////////////////////////////////////////////////////////////
//
void Engine::purgeEntity(Entity* e) {
  // cannot purge twice!
  assert(e->isOk());
  e->die();
  garbo.push_back(e);
}

//////////////////////////////////////////////////////////////////////////////
//
void Engine::purgeEntities() {
  F__LOOP(it,ents) {
    delete it->second;
  }
  ents.clear();
  doHouseKeeping();
}

//////////////////////////////////////////////////////////////////////////////
//
void Engine::regoSystem(not_null<System*> s) {
  systemList.add(s);
}

//////////////////////////////////////////////////////////////////////////////
//
void Engine::purgeSystem(not_null<System*> s ) {
  systemList.purge(s);
}

//////////////////////////////////////////////////////////////////////////////
//
void Engine::purgeSystems() {
  systemList.clear();
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
  doHouseKeeping();
  updating = false;
}

//////////////////////////////////////////////////////////////////////////////
//
void Engine::ignite() {
  initEntities();
  initSystems();
  for (auto s= systemList.head; NNP(s); s=s->next) {
    s->preamble();
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Engine::doHouseKeeping() {
  F__LOOP(it, garbo) {
    delete *it;
  }
  garbo.clear();
}





NS_END


