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

#pragma once

#include "Ecs.h"
NS_BEGIN(ecs)

class Entity;
//////////////////////////////////////////////////////////////////////////////
//
class FS_DLL Node : public f::FDListItem<Node> {

  //NOT owner
  s_map<sstr, Component*> values;
  s_map<COMType, sstr> types;
  DECL_PTR(Entity ,entity)

public:

  Entity* getEntity() { return entity; }

  bool bindEntity(not_null<Entity*> );
  bool belongsTo(not_null<Entity*>);
  Component* get(const sstr &field);

  Node(const s_map<sstr, COMType>& );
  virtual ~Node() {}
  NODFT(Node)
  NOCPYASS(Node)

};

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
T* nodeFld(not_null<ecs::Node*> n, const sstr &fld) {
  auto v= n->get(fld);
  if (NNP(v)) {
    return (T*) v;
  } else {
    return nullptr;
  }
}


NS_END



