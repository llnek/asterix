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

#include "C.h"

NS_BEGIN(gmath)

//////////////////////////////////////////////////////////////////////////////
//
const CCT_PT getPosAsGrid(GVars *ss, const f::Cell2I &cell) {
  auto wz= cx::visSize();
  auto ux= ss->unitSize;
  auto borderValue = 0.6;

  if (c::ApplicationProtocol::Platform::OS_IPAD ==
      XCFG()->getPlatform()) {
    borderValue = 0.75;
  }

  return CCT_PT(
    wz.width * 0.625 + (ux.width + borderValue) * (cell.x-5),
    wz.height/2 - (ux.width + borderValue) * (cell.y-5));
}


NS_END




