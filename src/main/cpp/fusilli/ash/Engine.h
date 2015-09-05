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

#include "Ash.h"
#include <vector>
#include <map>
NS_BEGIN(ash)

class EntityList;
class SystemList;
class NodeList;
class System;
class Signal;
class Family;
class Entity;

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Engine {

private:

  map<NodeMask,Family*> families;
  EntityList entityList;
  SystemList systemList;
  bool updating;
  Signal updateComplete;

public:

  void ComponentAdded(Entity*, const ComponentClass& );

  virtual ~Engine();
  Engine();

  const vector<Entity*> GetEntities();
  const vector<Entity*> GetSystems();

  void RemoveEntity(Entity* );
  void AddEntity(Entity* );
  void RemoveAllEntities() ;

  NodeList*  GetNodeList(const NodeMask& );
  void ReleaseNodeList(const NodeMask& );

  System* GetSystem(const SystemType& );
  void RemoveSystem (System* );
  void AddSystem(System* );
  void RemoveAllSystems();

  void Update(float time);

};




NS_END(ash)
#endif


