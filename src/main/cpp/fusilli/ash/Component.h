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

#if !defined(__COMPONENT_H__)
#define __COMPONENT_H__

#include "platform/CCCommon.h"
#include "core/fusilli.h"
#include "Ash.h"
NS_BEGIN(ash)


//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Component {
friend class Entity;
protected:

  Component() { par=nullptr;}

  Entity* par;

private:

  NO__COPYASSIGN(Component)

public:

  virtual const COMType TypeId() = 0;
  bool HasParent() { return par; }
  virtual ~Component() {}

};



NS_END(ash)
#endif


