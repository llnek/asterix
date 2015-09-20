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

#if !defined(__NODE_H__)
#define __NODE_H__

#include "Ash.h"
#include <vector>
#include <map>

NS_ALIAS(s, std)
NS_BEGIN(ash)


class Entity;
//////////////////////////////////////////////////////////////////////////////
//
class FS_DLL Node {
private:

  NO__COPYASSIGN(Node)
  Node();

  s::map<stdstr, Component*> values;
  s::map<COMType, stdstr> types;
  Entity* entity;

public:

  static Node* Create(const s::map<stdstr, COMType>& );
  virtual ~Node();

  Entity* GetEntity() { return entity; }
  Component* Get(const stdstr& field);
  bool BindEntity(Entity* );
  bool BelongsTo(Entity*);

  Node* previous;
  Node* next;

};

NS_END(ash)

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
T* NodeFld(ash::Node* n, const stdstr& fld) {
  auto v= n->Get(fld);
  if (NNP(v)) {
    return static_cast<T*>(v);
  } else {
    return nullptr;
  }
}



#endif


