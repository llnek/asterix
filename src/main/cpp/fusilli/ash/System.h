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

#if !defined(__SYSTEM_H__)
#define __SYSTEM_H__

#include "Ash.h"
NS_BEGIN(ash)

class Engine;
//////////////////////////////////////////////////////////////////////////////
//
class FS_DLL System {
protected:

  System(int priority);
  System();

  void init();

  int _priority;
  bool active;

  NOCPYASS(System)

public:

  virtual void removeFromEngine(not_null<Engine*>);
  virtual void addToEngine(not_null<Engine*>);
  virtual bool update(float time);
  bool isa(const SystemType& );

  virtual const SystemType typeId() = 0;
  virtual ~System();

  int priority() { return _priority; }
  bool isActive() { return active; }

  void restart();
  void suspend();

  System* previous;
  System* next;

};


NS_END(ash)
#endif



