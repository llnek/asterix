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
#include "aeon/fusilli.h"
NS_BEGIN(ecs)

#define MDECL_COMP_TPID(x) \
    virtual const ecs::COMType typeId() { return x; }

//////////////////////////////////////////////////////////////////////////
//
typedef s::string SystemType;
typedef s::string COMType;
typedef s::string NodeType;
typedef int NodeId;

//////////////////////////////////////////////////////////////////////////
//
class FS_DLL Component {
protected:
  Component() {}
public:
  virtual bool isAuto() { return false; }
  virtual const COMType typeId() = 0;
  virtual void dispose() {}
  virtual ~Component() {}
};

class Node;
typedef s::map<NodeId,Component*> CompoCache ;
typedef s::map<NodeId,Node*> NodeCache ;

//////////////////////////////////////////////////////////////////////////
//
enum SPV {
  PreUpdate =  100,
  NetPlay=    200,
  AI=     300,
  Motion=     400,
  Move=       500,
  Logic=   600,
  Collide=  700,
  Resolve=    800,
  Render=     900,

  Error = -1
};



NS_END



