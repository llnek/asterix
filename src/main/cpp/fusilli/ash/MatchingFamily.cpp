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

#include "NodeList.h"
#include "Engine.h"
#include "MatchingFamily.h"
NS_USING(std)
NS_BEGIN(ash)


//////////////////////////////////////////////////////////////////////////////
//
MatchingFamily::~MatchingFamily() {
  delete nodePool;
  delete nodes;
}

//////////////////////////////////////////////////////////////////////////////
//
MatchingFamily::MatchingFamily(const NodeMask& m, Engine* engine) {
  nodes = new NodeList();
  nodeClass = m;
  this->engine = engine;

  nodePool = new NodePool(nodeClass, components);
  nodePool->Dispose(nodePool->Get() );
}

//////////////////////////////////////////////////////////////////////////////
//
void MatchingFamily::NewEntity(Entity* e) {
  AddIfMatch(e);
}

//////////////////////////////////////////////////////////////////////////////
//
void MatchingFamily::AddedTo(Entity* e, const COMType& z) {
  AddIfMatch(e);
}

//////////////////////////////////////////////////////////////////////////////
//
void MatchingFamily::RemovedFrom(Entity* e, const COMType& z) {
  if (components.find(z) != components.end()) {
    RemoveIfMatch(e);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void MatchingFamily::RemoveEntity(Entity* e) {
  RemoveIfMatch(e);
}

//////////////////////////////////////////////////////////////////////////////
//
void MatchingFamily::CleanUp() {
  for (auto node = nodes->head; node != nullptr; node = node->next) {
    entities.Remove(node->entity);
  }
  nodes.RemoveAll();
}

//////////////////////////////////////////////////////////////////////////////
//
void MatchingFamily::AddIfMatch(Entity* e) {
  if (entities.Has(e)) { return; }
  for (auto it = components.begin(); it != components.end(); ++it) {
    if (!e->Has(it->first)) {
      return;
    }
  }
  auto node = nodePool->Get();
  node->entity = e;
  for (auto it = components.begin(); it != components.end(); ++it) {
    const COMType compClass= it->first;
    const string compName= it->second;
    node->Add(compName, e->Get(compClass));
  }
  entities.Add(e, node);
  e->componentRemoved.Add(&MatchingFamily::RemovedFrom, this);
  nodes->Add(node);
}

//////////////////////////////////////////////////////////////////////////////
//
void MatchingFamily::RemoveIfMatch(Entity* e) {
  auto it= entities.find(e);
  if (it != entities.end()) {
    auto node = it->second;
    e->componentRemoved.Remove(&MatchingFamily::RemovedFrom, this);
    entities.erase(it);
    nodes->Remove(node);
    if (engine->updating) {
      nodePool->Cache(node);
      engine->UpdateComplete.Add(&MatchingFamily::ReleaseNodePoolCache, this);
    } else {
      nodePool->Dispose(node);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void MatchingFamily::ReleaseNodePoolCache() {
  engine->UpdateComplete.Remove(&MatchingFamily::ReleaseNodePoolCache);
  nodePool->ReleaseCache();
}



NS_END(ash)

