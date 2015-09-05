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

#include "MatchingFamily.h"

MatchingFamily::MatchingFamily(nodeClass, Engine* engine) {
  this->nodeClass = nodeClass;
  this->engine = engine;
  this->nodes = new NodeList();
  this->entities = Dictionary::create();
  this->entities->retain();
  this->components = Dictionary::create();
  this->components->retain();

  this->nodePool = new NodePool( this.nodeClass, this.components );
  this->nodePool->Dispose( this->nodePool->Gget() );

  auto nodeClassPrototype = this->nodeClass->prototype;

  for(var property in nodeClassPrototype) {
      ///TODO - tidy this up...
      if(nodeClassPrototype.hasOwnProperty(property) &&
          property != "types" &&
          property != "next" &&
          property != "previous" &&
          property != "constructor" &&
          property != "super" &&
          property != "extend" &&
          property != "entity") {
          var componentObject = nodeClassPrototype.types[property];
          this.components.add(componentObject, property);
      }
  }
}

void MatchingFamily::NewEntity(Entity* e) {
  AddIfMatch(e);
}

void MatchingFamily::ComponentAddedToEntity(Entity* e, componentClass) {
  AddIfMatch(e);
}

void MatchingFamily::ComponentRemovedFromEntity(Entity* e, componentClass) {
  if (this->components->Has(componentClass)) {
    RemoveIfMatch(e);
  }
}

void MatchingFamily::RemoveEntity(Entity* e) {
  RemoveIfMatch(e);
}

void MatchingFamily::CleanUp() {
  for (auto node = nodes->head; node != nullptr; node = node->next) {
    entities->Remove(node->entity);
  }
  nodes->RemoveAll();
}

void MatchingFamily::AddIfMatch(Entity* e) {
  if (entities->Has(e)) { return; }
  DictElement* pe;
  bool ok=true;
  CCDICT_FOREACH(components, pe) {
    if (ok) {
      const string compClass= pe->getStrKey();
      if (!e->Has(compClass)) { ok=false; }
    }
  }
  if (!ok) {return;}

  auto node = nodePool->Get();
  node->entity = e;
  pe=nullptr;
  CCDICT_FOREACH(components, pe) {
    const string compClass= pe->getStrKey();
    const string compName= static_cast<String*>(pe->getObject())->getCString();
    node->setObject(e->Get(compClass), compName);
  };
  entities->Add(e, node);
  //e->componentRemoved.add(this.componentRemovedFromEntity, this);
  nodes->Add(node);
}

void MatchingFamily::RemoveIfMatch(Entity* e) {
    var entities = this.entities,
        nodes = this.nodes,
        engine = this.engine,
        nodePool = this.nodePool;

  if (entities->Has(e)) {
    auto node = entities->Retrieve(e);
    //e->componentRemoved.remove(this.componentRemovedFromEntity, this);
    entities->Remove(e);
    nodes->Remove(node);
    if (engine->updating) {
      nodePool->cache(node);
      //engine->UpdateComplete.add(this.releaseNodePoolCache, this);
    } else {
      nodePool->Dispose(node);
    }
  }
}

void MatchingFamily::ReleaseNodePoolCache() {
  //engine->UpdateComplete.remove(this.releaseNodePoolCache);
  nodePool->ReleaseCache();
}



NS_END(Ash)

