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

#if !defined(__ENTITY_H__)
#define __ENTITY_H__

#include "Ash.h"
NS_BEGIN(ash)

class Engine;
//////////////////////////////////////////////////////////////////////////////
//
class FS_DLL Entity {
private:

  s::map<stdstr, Component*> parts;
  Engine* engine;
  stdstr group;
  bool dead;

  NO__CPYASS(Entity)
  Entity();

public:

  static owner<Entity*> reify(const stdstr& group, not_null<Engine*>);

  Entity* previous;
  Entity* next;

  virtual ~Entity();

  void checkin(not_null<Component*>);
  void purge(const COMType&);

  Component* get(const COMType& );
  bool has(const COMType&);

  const stdstr groupId() { return group; }

  bool isOk() { return !dead; };
  void markDelete();

  const s::vector<Component*> getAll();

};



NS_END(ash)
#endif

