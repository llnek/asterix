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

//////////////////////////////////////////////////////////////////////////////
//
#include "nlohmann/json.hpp"
#include "System.h"
#include "Entity.h"

NS_ALIAS(j, nlohmann)
NS_BEGIN(ecs)

//////////////////////////////////////////////////////////////////////////////
//
class TypeRegistry;
class FS_DLL Engine {

  DECL_TD(SystemList, systemList)
  DECL_TD(EntityCache, ents)

  DECL_PTR(TypeRegistry, types)
  DECL_BF(updating)
  DECL_IZ(lastId)

  s_vec<Entity*> garbo;
  NOCPYASS(Engine)

  EntId generateEid();

protected:

  virtual void initEntities() = 0;
  virtual void initSystems() = 0;
  DECL_TD(j::json, config)

public:

  const s_vec<Entity*> getEntities(const s_vec<COMType>&);
  const s_vec<Entity*> getEntities(const COMType&);
  const s_vec<Entity*> getEntities();

  void getEntities(const s_vec<COMType>&, s_vec<Entity*>&);
  void getEntities(const COMType&, s_vec<Entity*>&);
  void getEntities(s_vec<Entity*>&);

  TypeRegistry* rego() { return types; }

  void purgeSystem (not_null<System*>);
  void purgeSystems();
  void purgeEntities();
  void purgeEntity(Entity*);

  void regoSystem(not_null<System*> );
  Entity* reifyEntity();

  void doHouseKeeping();
  void ignite();
  void update(float time);

  j::json getCfg() { return config; }

  Engine(j::json c) { config=c; }
  Engine();
  virtual ~Engine();

};



NS_END


