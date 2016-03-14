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
#include "nlohmann/json.hpp"
#include "System.h"
#include "Node.h"

NS_ALIAS(j, nlohmann)
NS_BEGIN(ecs)

//////////////////////////////////////////////////////////////////////////////
//
class TypeRegistry;
class FS_DLL Engine {

  DECL_TD(SystemList, _systemList)
  DECL_TD(NodeCache, _ents)

  DECL_PTR(TypeRegistry, _types)
  DECL_BF(_updating)
  DECL_IZ(_lastId)

  s_vec<Node*> _garbo;
  NOCPYASS(Engine)

  NodeId generateEid();

protected:

  virtual void initEntities() = 0;
  virtual void initSystems() = 0;
  DECL_TD(j::json, _config)

public:

  const s_vec<Node*> getNodes(const s_vec<COMType>&);
  const s_vec<Node*> getNodes(const COMType&);
  const s_vec<Node*> getNodes();

  void getNodes(const s_vec<COMType>&, s_vec<Node*>&);
  void getNodes(const COMType&, s_vec<Node*>&);
  void getNodes(s_vec<Node*>&);

  Node* reifyNode(const sstr &n, bool take=false);
  TypeRegistry* rego() { return _types; }

  void purgeSystem (not_null<System*>);
  void purgeSystems();
  void purgeNodes();
  void purgeNode(not_null<Node*>);

  void regoSystem(not_null<System*> );

  void doHouseKeeping();
  void ignite();
  void update(float time);

  j::json getCfg() { return _config; }

  Engine(j::json c) { _config=c; }
  Engine();
  virtual ~Engine();

};



NS_END


