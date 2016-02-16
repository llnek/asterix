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

//////////////////////////////////////////////////////////////////////////////
//
const sstr getGemPng(int type);


NS_END




