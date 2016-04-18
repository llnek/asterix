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

#include "core/XConfig.h"
#include "core/COMP.h"
#include "core/CCSX.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(pumpkins)

//////////////////////////////////////////////////////////////////////////////
class CC_DLL Lightning : public f::DrawNode {

  bool inix(const CCT_PT&, const CCT_PT&, c::Color4F, bool);
  void generateKeyPoints(const CCT_PT&, const CCT_PT&);
  void drawSegments();
  void drawNextSegment(float dt);
  Lightning() {}

  __decl_vec(CCT_PT, _keyPoints)
  __decl_iz(_numKeyPoints)
  __decl_iz(_lastKeyPoint)
  __decl_md(c::Color4F, _color)

public:

  static owner<Lightning*> create(
      const CCT_PT&, const CCT_PT&,
      c::Color4F, bool is_animated);

};


NS_END



