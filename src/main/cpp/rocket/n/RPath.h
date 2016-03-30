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

#include "2d/CCDrawNode.h"
NS_BEGIN(rocket)

enum LineType {

  LINE_TEMP,
  LINE_DASHED,
  LINE_NONE

};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL RPath : public c::DrawNode, public ecs::Component {


public:

  static owner<RPath*> create() {
    return f::reifyRefType<RPath>();
  }
  RPath() {}

  MDECL_COMP_TPID("n/RPath");

  __decl_fz(energyDecrement)
  __decl_fz(lineAngle)
  __decl_fz(energyLineX)
  __decl_fz(energyHeight)
  __decl_iz(dash)
  __decl_iz(dashSpace)

  __decl_td(LineType, lineType)
  __decl_fz(lineLength)
  __decl_fz(energy)
  __decl_td(c::Vec2, pivot)
  __decl_td(c::Vec2, tip)

  void setEnergyDecrement(float);
  void update(float);
  void reset();

  virtual bool init();

};


NS_END



