// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Ken Leung. All rights reserved.

#pragma once

//////////////////////////////////////////////////////////////////////////////
//

#include "Ecs.h"
NS_BEGIN(ecs)

//////////////////////////////////////////////////////////////////////////////
//
class Engine;
class FS_DLL Node : public f::Poolable {
friend class Engine;

  //owns all the parts
  s_map<COMType, Component*> _parts;

  DECL_PTR(Engine, _engine)
  DECL_BF(_dead)
  DECL_TD(NodeId, _eid)
  DECL_TD(sstr, _name)

  Node(not_null<Engine*>, const sstr&, NodeId);
  void die() { _dead=true; }

  NOCPYASS(Node)
  NODFT(Node)
  virtual ~Node();

public:

  // takeover the component
  void checkin(not_null<Component*>);
  void purge(const COMType&);

  Component* get(const COMType& );
  bool has(const COMType&);

  NodeId getEid() { return _eid; }
  bool isOk() { return !_dead; };

  const s_vec<Component*> getAll();
};


NS_END



