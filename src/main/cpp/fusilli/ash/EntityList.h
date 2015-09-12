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

#if !defined(__ENTITYLIST_H__)
#define __ENTITYLIST_H__

#include "Entity.h"
NS_USING(std)
NS_BEGIN(ash)

//////////////////////////////////////////////////////////////////////////////
//
class FS_DLL EntityList {
private:
  DISALLOW_COPYASSIGN(EntityList)
  friend class Engine;

  virtual ~EntityList();
  EntityList();

public:
  Entity* head;
  Entity* tail;

  const vector<Entity*> List();

  void Remove(Entity* );
  void Add(Entity* );
  void RemoveAll() ;

};


NS_END(ash)
#endif
