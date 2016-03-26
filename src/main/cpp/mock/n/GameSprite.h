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
NS_BEGIN(mock)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GameSprite : public f::CPixie {

	CC_SYNTHESIZE(Point, _nextPosition, NextPosition);
	CC_SYNTHESIZE(float, _width, Width);
	CC_SYNTHESIZE(float, _height, Height);
	CC_SYNTHESIZE(Point, _vector, Vector);


    inline virtual void place () { this->setPosition(_nextPosition); };


    inline void setSize() {
    	_width = this->boundingBox().size.width;
		_height = this->boundingBox().size.height;
    }

};


NS_END




