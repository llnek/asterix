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

#include "Ecs.h"
NS_BEGIN(ecs)

#define __decl_sys_tpid(x) \
  virtual const ecs::SystemType typeId() { return x; }

#define __decl_sys_priority(x) \
  virtual int priority() { return x; }

#define __decl_sys_preamble() \
  virtual void preamble();

#define __decl_sys_update() \
  virtual bool update(float);

//////////////////////////////////////////////////////////////////////////////
//
class Engine;
class FS_DLL System : public f::FDListItem<System> {
protected:

  __decl_ptr(Engine, _engine)
  __decl_bt(_active)

  __decl_nocpyass(System)
  __decl_nodft(System)

public:

  virtual const SystemType typeId() = 0;
  virtual bool update(float time) = 0;
  virtual void preamble()= 0;
  virtual int priority() = 0;

  Engine* getEngine() { return _engine; }
  bool isActive() { return _active; }
  bool isa(const SystemType&  t) {
    return typeId() == t;
  }

  void suspend() { _active=false; }
  void restart() { _active=true; }

  System(not_null<Engine*> e) { _engine= e; }
  virtual ~System() {}
};

//////////////////////////////////////////////////////////////////////////////
//
class FS_DLL SystemList  : public f::FDList<System> {
  __decl_nocpyass(SystemList)
public:
  virtual void add(not_null<System*> );
  System* get(const SystemType&);

  virtual ~SystemList() {}
  SystemList() {}
};


NS_END
#include "Engine.h"



