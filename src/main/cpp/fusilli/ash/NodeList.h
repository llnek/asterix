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
NS_BEGIN(ash)

class Entity;
class Node;

//////////////////////////////////////////////////////////////////////////////
//
class FS_DLL NodeList {
private:
  DECL_TD(NodeType, nType)
public:

  const NodeType getType() { return nType; }

  NodeList(const NodeType&);
  virtual ~NodeList();

  bool containsWithin(not_null<Entity*>);
  bool isCompatible(not_null<Entity*>);
  void removeEntity(not_null<Entity*> );

  void purge(not_null<Node*> );
  void add(not_null<Node*> );
  bool isEmpty();
  void clear();
  int size();

  NOCPYASS(NodeList)
  NODFT(NodeList)

  DECL_PTR(Node ,head)
  DECL_PTR(Node ,tail)

};



NS_END(ash)
#endif


