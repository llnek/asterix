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

#include <vector>
#include <map>
#include "Ash.h"
NS_USING(std)
NS_BEGIN(ash)

class Component;
class Entity;

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Entity {
public:

  Entity* previous;
  Entity* next;

  virtual ~Entity();

  Component* Remove(const COMType&);
  void Add(Component*);

  Component* Get(const COMType& );
  bool Has(const COMType&);

  void MarkDelete();
  bool IsOk() { return !dead; };

  const vector<Component*> GetAll();

private:

  map<string,Component*> components;
  string group;
  Engine* engine;
  bool dead;
  DISALLOW_COPYASSIGN(Entity)
  Entity(const string& group, Engine*);
  Entity();
};



NS_END(ash)
#endif
