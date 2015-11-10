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

#if !defined(__UTILS_H__)
#define __UTILS_H__

#include "core/CCSX.h"
#include "n/cobjs.h"
#define BD_SZ 3

namespace tttoe {

  const s::vector<s::array<int,BD_SZ>> MapGoalSpace();

  const s::array<fusii::Box4, BD_SZ * BD_SZ>
    MapGridPos(float scale = 1.0f);

  const stdstr PkFlip(const stdstr& img, bool flip);

  const stdstr XrefImg(int value);

  c::Sprite* DrawSymbol(not_null<PlayView*> view,
      float x, float y,
      int value, bool flip=false);


};

#endif

