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

//////////////////////////////////////////////////////////////////////////////
//
class FS_DLL System {
protected:

  System(int priority);

private:

  DISALLOW_COPYASSIGN(System)
  System();

  int priority;
  bool active;

public:

  System* previous;
  System* next;

  virtual ~System();

  virtual void RemoveFromEngine(Engine*);
  virtual void AddToEngine(Engine*);
  virtual bool Update(float time);
  bool Is(const SystemType& );

  virtual const SystemType TypeId() = 0;

  int Priority() { return priority; }
  bool IsActive() { return active; }

  void Restart();
  void Suspend();
};


NS_END(ash)
#endif
