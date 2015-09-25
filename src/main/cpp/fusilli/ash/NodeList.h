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

#if !defined(__NODELIST_H__)
#define __NODELIST_H__

#include "Ash.h"
NS_USING(Guide)
NS_BEGIN(ash)


class Entity;
class Node;

//////////////////////////////////////////////////////////////////////////////
//
class FS_DLL NodeList {
private:

  NO__CPYASS(NodeList)
  NodeType nType;
  NodeList();

public:

  static owner<NodeList*> Create(const NodeType&);

  const NodeType GetType() { return nType; }
  virtual ~NodeList();

  bool ContainsWithin(not_null<Entity*>);
  bool IsCompatible(not_null<Entity*>);
  void RemoveEntity(not_null<Entity*> );

  void Purge(not_null<Node*> );
  void Add(not_null<Node*> );
  void Clear();
  bool IsEmpty();

  int Size();

  Node* head;
  Node* tail;

};



NS_END(ash)
#endif


