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

#if !defined(__ASH_H__)
#define __ASH_H__

#include "aeon/fusilli.h"
#include <string>
NS_ALIAS(s, std)
NS_BEGIN(ash)


//////////////////////////////////////////////////////////////////////////
//
typedef s::string SystemType;
typedef s::string COMType;
typedef s::string NodeType;

//////////////////////////////////////////////////////////////////////////
//
class FS_DLL Component {
protected:
  Component() {}
private:
  NO__COPYASSIGN(Component)
public:
  virtual const COMType TypeId() = 0;
  virtual ~Component() {
    printf("Component dtor\n");
  }
};

//////////////////////////////////////////////////////////////////////////
//
enum SPV {
    PreUpdate =  100,
    NetPlay=    200,
    Select=     300,
    Motion=     400,
    Move=       500,
    Logic=   600,
    Collide=  700,
    Resolve=    800,
    Render=     900
};


NS_END(ash)
#endif


