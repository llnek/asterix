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

#include "2d/CCPixieNode.h"
NS_BEGIN(rocket)

typedef enum lineTypes {

  LINE_TEMP,
  LINE_DASHED,
  LINE_NONE

} LineType;

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL LineContainer : public c::DrawNode {

  LineContainer() {}

public:

  static LineContainer* create();

  DECL_FZ(energyDecrement)
  DECL_FZ(lineAngle)
  DECL_FZ(energyLineX)
  DECL_FZ(energyHeight)
  DECL_IZ(dash)
  DECL_IZ(dashSpace)

  DECL_TD(LineType, lineType)
  DECL_FZ(lineLength)
  DECL_FZ(energy)
  DECL_TD(c::Vec2, pivot)
  DECL_TD(c::Vec2, tip)

  void setEnergyDecrement(float);
  void update(float);
  void reset();

  virtual bool init();
};


NS_END



