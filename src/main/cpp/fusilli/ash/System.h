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
NS_USING(Guide)
NS_BEGIN(ash)

class Engine;
//////////////////////////////////////////////////////////////////////////////
//
class FS_DLL System {
protected:

  System(int priority);

private:

  NO__CPYASS(System)
  System();
  void Init();

  int priority;
  bool active;

public:

  virtual void RemoveFromEngine(not_null<Engine*>);
  virtual void AddToEngine(not_null<Engine*>);
  virtual bool Update(float time);
  bool Is(const SystemType& );

  virtual const SystemType TypeId() = 0;

  int Priority() { return priority; }
  bool IsActive() { return active; }

  void Restart();
  void Suspend();

  virtual ~System();

  System* previous;
  System* next;

};


NS_END(ash)
#endif



