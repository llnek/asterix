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

#include "Family.h"
NS_BEGIN(Ash)

Family::Family(nodeObject, Engine* engine) {
}

Family::GetNodeList() {
  return nodes;
}

void Family::NewEntity(Entity* e) {
  throw std::exception("should be overriden");
}

void Family::RemoveEntity(Entity* e) {
  throw std::exception("should be overriden");
}

void Family::ComponentAddedToEntity(Entity* e, componentClass) {
  throw std::exception("should be overriden");
}

void Family::ComponentRemovedFromEntity(Entity* e, componentClass) {
  throw std::exception("should be overriden");
}

void Family::CleanUp() {
  throw std::exception("should be overriden");
}



NS_END(Ash)
