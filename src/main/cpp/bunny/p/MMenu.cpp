// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Kenneth Leung. All rights reserved.

#include "core/XConfig.h"
#include "core/CCSX.h"
#include "MMenu.h"
#include "Splash.h"
#include "Game.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(bunny)

//////////////////////////////////////////////////////////////////////////////
//
void MMenu::decoUI() {

  auto off= cx::reifyMenuBtn("unmute.png");
  auto on= cx::reifyMenuBtn("mute.png");
  auto sz= CC_CSIZE(off);
  auto wb= cx::visBox();
  auto tile= CC_CHT(off)/GOLDEN_RATIO;

  addAudioIcons(s_arr<c::MenuItem*,2>{off,on},
      cx::anchorTR(),
      CCT_PT(wb.right-tile, wb.top-tile));

  if (XCFG()->hasAudio()) {
    cx::pauseMusic();
  }
}


NS_END



