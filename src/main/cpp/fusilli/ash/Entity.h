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

#include "Ash.h"
NS_BEGIN(ash)

//////////////////////////////////////////////////////////////////////////////
//
class Engine;
class FS_DLL Entity : public f::FDListItem<Entity> {

  //owns all the parts
  s_map<COMType, Component*> parts;
  DECL_PTR(Engine,engine)
  DECL_TD(sstr, group)
  DECL_BF(dead)

public:

  Entity(const sstr &group, not_null<Engine*>);
  NODFT(Entity)
  NOCPYASS(Entity)

  // takeover the component
  void checkin(not_null<Component*>);
  void purge(const COMType&);

  Component* get(const COMType& );
  bool has(const COMType&);

  const sstr groupId() { return group; }
  virtual ~Entity();

  bool isOk() { return !dead; };
  void markDelete();

  const s_vec<Component*> getAll();
};


NS_END



