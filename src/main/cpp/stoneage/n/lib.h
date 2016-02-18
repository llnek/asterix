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
#include "core/ComObj.h"
#include "core/CCSX.h"


NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(stoneage)

#define TYPE_GEM_BLUE  0
#define TYPE_GEM_YELLOW 1
#define TYPE_GEM_RED 2
#define TYPE_GEM_GREEN 3
#define TYPE_GEM_PINK  4
#define TYPE_GEM_WHITE 5
#define GEMSET_SIZE 6

#define TILE_SIZE  60
#define GRID_SIZE_X  8
#define GRID_SIZE_Y  11
#define GRID_SPACE  6

#define Z_GRID  1
#define Z_SWAP_1  2
#define Z_SWAP_2  3
#define Z_DIAMOND  4

#define POINTS  10
#define COMBO_POINTS  50
#define DIAMOND_POINTS  100


//////////////////////////////////////////////////////////////////////////////
//
const sstr getGemPng(int type);
int getGemType(int pos);

NS_END




