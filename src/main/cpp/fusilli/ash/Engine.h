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
NS_BEGIN(ash)

//////////////////////////////////////////////////////////////////////////////
// owns all items in this list
template <typename T> class FS_DLL ObjList {
private:
  NOCPYASS(ObjList)
public:
  const s::vector<T*> list();
  void release(not_null<T*>);
  void add(not_null<T*> );
  void clear() ;
  T* head;
  T* tail;
  DECLCZ(ObjList)
};

//////////////////////////////////////////////////////////////////////////////
//
template <typename T>
void ObjList<T>::add(not_null<T*> e) {
  if (ENP(head)) {
    head = tail = e;
    SNPTR(head->previous)
    SNPTR(head->next)
  } else {
    tail->next = e;
    e->previous = tail;
    SNPTR(e->next)
    tail = e;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
template <typename T>
void ObjList<T>::release(not_null<T*> e) {
  if (head == e) {
    head = head->next;
  }
  if (tail == e) {
    tail = tail->previous;
  }
  if (e->previous ) {
    e->previous->next = e->next;
  }
  if (e->next) {
    e->next->previous = e->previous;
  }
  SNPTR(e->previous)
  SNPTR(e->next)
}

//////////////////////////////////////////////////////////////////////////////
//
template <typename T>
void ObjList<T>::clear() {
  while (NNP(head)) {
    auto e= head;
    head = head->next;
    delete e;
  }
  SNPTR(head)
  SNPTR(tail)
}

//////////////////////////////////////////////////////////////////////////////
//
template <typename T>
const s::vector<T*> ObjList<T>::list() {
  s::vector<T*> v;
  for (auto p= head; NNP(p); p=p->next) {
    v.push_back(p);
  }
  return v;
}

//////////////////////////////////////////////////////////////////////////////
//
template <typename T>
ObjList<T>::~ObjList() {
  //printf("ObjList dtor\n");
  clear();
}

//////////////////////////////////////////////////////////////////////////////
//
template <typename T>
ObjList<T>::ObjList() {
  SNPTR(head)
  SNPTR(tail)
}

//////////////////////////////////////////////////////////////////////////////
//
class FS_DLL Engine {
private:

  void purgeEntity(ObjList<Entity>*, Entity*);
  void onModifyEntity(Entity*);
  void onPurgeEntity(Entity*);
  void onAddEntity(Entity*);
  void houseKeeping();

  s::map<stdstr, ObjList<Entity>*> groups;
  s::vector<NodeList*> nodeLists;
  s::vector<Entity*> freeList;
  s::vector<Entity*> modList;
  s::vector<Entity*> addList;

  ObjList<System> systemList;
  bool updating;
  bool dirty;

  NOCPYASS(Engine)

public:

  const s::vector<Entity*> getEntities(const stdstr& group);
  const s::vector<Entity*> getEntities();
  const s::vector<System*> getSystems();

  Entity* reifyEntity(const stdstr& group);

  void notifyModify(not_null<Entity*>);
  void purgeEntity(not_null<Entity*> );
  void purgeEntities(const stdstr& group) ;

  NodeList* getNodeList(const NodeType& );

  void purgeSystem (not_null<System*> );
  void purgeSystems();
  void forceSync();
  void regoSystem(not_null<System*> );

  void update(float time);

  DECLCZ(Engine)
};




NS_END(ash)
#endif


