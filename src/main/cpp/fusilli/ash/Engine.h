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

#include "NodeList.h"
#include "System.h"
#include "Entity.h"
NS_BEGIN(ash)

//////////////////////////////////////////////////////////////////////////////
//
typedef f::FDList<Entity> EList;

//////////////////////////////////////////////////////////////////////////////
//
class FS_DLL Engine {

  void purgeEntity(EList*, Entity*);
  void maybeBind(NodeList*,Entity*);
  void onModifyEntity(Entity*);
  void onPurgeEntity(Entity*);
  void onAddEntity(Entity*);
  void houseKeeping();
  s_vec<NodeList*> nodeLists;
  s_map<sstr, EList*> groups;
  s_vec<Entity*> freeList;
  s_vec<Entity*> modList;
  s_vec<Entity*> addList;
  DECL_TD(SystemList, systemList)
  DECL_BF(updating)
  DECL_BF(dirty)

public:

  const s_vec<Entity*> getEntities(const sstr &grp);
  const s_vec<Entity*> getEntities();

  Entity* reifyEntity(const sstr &grp);
  Entity* reifyEntity();

  void purgeEntities(const sstr &grp);
  void purgeEntities();
  void purgeEntity(not_null<Entity*>);

  void purgeSystem (not_null<System*> );
  void purgeSystems();

  NodeList* getNodeList(const NodeType& );
  void notifyModify(not_null<Entity*>);
  void regoSystem(not_null<System*> );
  void forceSync();
  void ignite();
  void update(float time);

  virtual ~Engine();
  Engine() {}
  NOCPYASS(Engine)

protected:

  virtual void initEntities() = 0;
  virtual void initSystems() = 0;
};


NS_END


