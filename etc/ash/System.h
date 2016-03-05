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

#include "NodeRego.h"
NS_BEGIN(ecs)

#define MDECL_SYS_TPID(x) \
  virtual const ecs::SystemType typeId() { return x; }

#define MDECL_SYS_PRIORITY(x) \
  virtual int priority() { return x; }

#define MDECL_SYS_PREAMBLE() \
  virtual void preamble();

#define MDECL_SYS_UPDATE() \
  virtual bool update(float);

//////////////////////////////////////////////////////////////////////////////
//
class Engine;
class FS_DLL System : public f::FDListItem<System> {
protected:

  DECL_PTR(Engine ,engine)
  DECL_BT(active)

public:

  virtual const SystemType typeId() = 0;
  virtual bool update(float time) = 0;
  virtual void preamble()= 0;
  virtual int priority() = 0;

  bool isActive() { return active; }
  bool isa(const SystemType&  t) {
    return typeId() == t;
  }

  void restart() { active=true; }
  void suspend() { active=false; }

  System(not_null<Engine*> e) { engine= e; }
  virtual ~System() {}
  NODFT(System)
  NOCPYASS(System)
};

//////////////////////////////////////////////////////////////////////////////
//
struct FS_DLL SystemList  : public f::FDList<System> {

  virtual void add(not_null<System*> );
  System* get(const SystemType&);

  virtual ~SystemList() {}
  SystemList() {}
  NOCPYASS(SystemList)
};

NS_END

#include "Engine.h"



