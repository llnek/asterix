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

#if !defined(__ENGINE_H__)
#define __ENGINE_H__

#include <vector>
#include <map>
#include "Ash.h"
NS_BEGIN(ash)

class EntityList;
class SystemList;
class NodeList;
class System;
class Entity;

//////////////////////////////////////////////////////////////////////////////
//
class FS_DLL Engine {
private:

  void RemoveEntity(EntityList* el, Entity* e);
  void OnModifyEntity(Entity* e);
  void OnRemoveEntity(Entity* e);
  void OnAddEntity(Entity* e);
  void HouseKeeping();

  map<string,EntityList*> groups;
  vector<NodeList*> nodeLists;
  vector<Entity*> freeList;
  vector<Entity*> modList;
  vector<Entity*> addList;

  SystemList systemList;
  bool updating;
  bool dirty;

public:

  virtual ~Engine();
  Engine();

  const vector<Entity*> GetEntities(const string& group);
  const vector<System*> GetSystems();

  Entity* CreateEntity(const string& group);
  void NotifyModify(Entity*);
  void RemoveEntity(Entity* );
  void RemoveEntities(const string& group) ;

  NodeList*  GetNodeList(const string& group, const NodeType& );
  void ReleaseNodeList(NodeList*& );

  System* GetSystem(const SystemType& );
  void RemoveSystem (System* );
  void AddSystem(System* );
  void RemoveSystems();

  void Update(float time);

};




NS_END(ash)
#endif


