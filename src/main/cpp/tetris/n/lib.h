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

#include "x2d/XLayer.h"
#include "n/C.h"

NS_BEGIN(tetris)

//////////////////////////////////////////////////////////////////////////////
//

owner<Shape*> previewShape(const ShapeInfo&, float x, float y);

owner<Shape*> reifyShape(s_vec<FArrBrick>& ,
    float x, float y,
    const ShapeInfo&);

int topLine(not_null<ecs::Node*>);

void clearOldBricks(s_vec<Brick*>&);
void disposeShape(Shape*);

const s_vec<c::Vec2>
findBBox(s_vec<FArrBrick>&, BModel*,
    float px, float py, int rID, bool skipCollide = false);

bool maybeCollide(s_vec<FArrBrick>&, float tl_x, float tl_y );

const f::Cell2D xrefTile(float x, float y);
const f::Cell2D xrefTile(const c::Vec2&);

void initDropper(Dropper*);

void setDropper(not_null<c::Node*>, Dropper*, float r, float s);

void lock(not_null<ecs::Node*>, Shape*);

bool testFilledRow(s_vec<FArrBrick>&, int r);

void flashFilled(s_vec<FArrBrick>&,
    FilledLines*,
    const s_vec<int>& lines);

void pauseForClearance(not_null<ecs::Node*>, bool b, float delay);

bool moveDown(s_vec<FArrBrick>&, Shape*);

bool shiftRight(s_vec<FArrBrick>&, Shape*);
bool shiftLeft(s_vec<FArrBrick>&, Shape*);

bool rotateRight(s_vec<FArrBrick>&, Shape*);
bool rotateLeft(s_vec<FArrBrick>&, Shape*);



NS_END



