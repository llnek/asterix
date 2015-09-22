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

#include "NodeList.h"
#include "System.h"
#include "Entity.h"
#include <vector>
#include <map>

NS_USING(Guide)
NS_ALIAS(s,std)
NS_BEGIN(ash)

//////////////////////////////////////////////////////////////////////////////
// owns all items in this list
template <typename T>
class FS_DLL ObjList {
private:
  NO__COPYASSIGN(ObjList)
public:
  const s::vector<T*> List();
  void Release(not_null<T*>);
  void Add(not_null<T*> );
  void Clear() ;
  T* head;
  T* tail;
  virtual ~ObjList();
  ObjList();
};

//////////////////////////////////////////////////////////////////////////////
//
class FS_DLL Engine {
private:

  void PurgeEntity(ObjList<Entity>*, Entity*);
  void OnModifyEntity(Entity*);
  void OnPurgeEntity(Entity*);
  void OnAddEntity(Entity*);
  void HouseKeeping();

  s::map<stdstr, ObjList<Entity>*> groups;
  s::vector<NodeList*> nodeLists;
  s::vector<Entity*> freeList;
  s::vector<Entity*> modList;
  s::vector<Entity*> addList;

  ObjList<System> systemList;
  bool updating;
  bool dirty;

  Engine();

  NO__COPYASSIGN(Engine)

public:

  DEFCREATE_FUNC(Engine)

  virtual ~Engine();

  const s::vector<Entity*> GetEntities(const stdstr& group);
  const s::vector<Entity*> GetEntities();
  const s::vector<System*> GetSystems();

  Entity* CreateEntity(const stdstr& group);

  void NotifyModify(not_null<Entity*>);
  void PurgeEntity(not_null<Entity*> );
  void PurgeEntities(const stdstr& group) ;

  NodeList* GetNodeList(const NodeType& );

  void PurgeSystem (not_null<System*> );
  void PurgeSystems();
  void RegoSystem(not_null<System*> );

  void Update(float time);

};




NS_END(ash)
#endif


