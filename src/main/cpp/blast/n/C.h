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
#include "lib.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(blast)

//////////////////////////////////////////////////////////////////////////////
//
struct BackgroundManager : public c::Node {
  virtual bool init() {
    if ( !c::Node::init() ) { return false; }
      auto star = c::DrawNode::create();
    auto wz= cx::visRect();
    addChild(star);
    // draw a white circle with varying opacity & position
    for (auto i = 0; i < MAX_STARS; ++i) {
      auto alpha = 0.25 + cx::rand() * 0.75;
      star->drawDot(
          c::Vec2(cx::randInt(wz.size.width), cx::randInt(wz.size.height)),
                    1, c::ccc4f(1, 1, 1, alpha));
    }
    return true;
  }
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL CPlayer : public f::CPixie {
  CPlayer(not_null<c::Node*> n)
  : CPixie(n) {}
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GVars : public ecs::Component {
  MDECL_COMP_TPID( "n/GVars" )

  static const int powerup_frequency_size = 10;
  static const int skill6_formations_size = 17;
  static const int skill5_formations_size = 17;
  static const int skill4_formations_size = 17;
  static const int skill3_formations_size = 17;
  static const int skill2_formations_size = 10;
  static const int skill1_formations_size = 2;

  s_arr<int,skill1_formations_size> skill1_formations;
  s_arr<int,skill2_formations_size> skill2_formations;
  s_arr<int,skill3_formations_size> skill3_formations;
  s_arr<int,skill4_formations_size> skill4_formations;
  s_arr<int,skill5_formations_size> skill5_formations;
  s_arr<int,skill6_formations_size> skill6_formations;
  s_arr<int,powerup_frequency_size> powerup_frequency;


};



NS_END




