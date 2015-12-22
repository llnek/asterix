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

#include "x2d/XLayer.h"
#include "n/GNodes.h"

NS_BEGIN(tetris)

//////////////////////////////////////////////////////////////////////////////
//
owner<Shape*> reifyShape(not_null<f::XLayer*>,
    s_vec<f::FArrInt>& ,
    float x, float y, const ShapeInfo&);

owner<Shape*> previewShape(not_null<f::XLayer*>,
    float x, float y,
    const ShapeInfo&);

int topLine(not_null<a::Node*>);

void clearOldBricks(s_vec<Brick*>&);
void disposeShape(Shape*);

const s_vec<c::Vec2>
findBBox(s_vec<f::FArrInt>&, BModel*,
    float px, float py, int rID, bool skipCollide = false);

bool maybeCollide(s_vec<f::FArrInt>&,
    float tl_x, float tl_y, float br_x, float br_y);

const f::Cell2D xrefTile(float x, float y);

void initDropper(Dropper*);

void setDropper(not_null<c::Node*>, Dropper*, float r, float s);

void lockBricks(s_vec<f::FArrInt>&, s_vec<FArrBrick>&, Brick*);

void lock(not_null<a::Node*>, Shape*);

void postLock(not_null<a::Node*>,
    s_vec<f::FArrInt>&,
    s_vec<FArrBrick>&);

bool testFilledRow(s_vec<f::FArrInt>&, int r);

void flashFilled(s_vec<FArrBrick>&,
    FilledLines*,
    const s_vec<int>& lines);

void pauseForClearance(not_null<a::Node*>, bool b, float delay);

bool moveDown(not_null<f::XLayer*> , s_vec<f::FArrInt>&, Shape*);

bool shiftRight(not_null<f::XLayer*>, s_vec<f::FArrInt>&, Shape*);
bool shiftLeft(not_null<f::XLayer*>, s_vec<f::FArrInt>&, Shape*);

bool rotateRight(not_null<f::XLayer*>, s_vec<f::FArrInt>&, Shape*);
bool rotateLeft(not_null<f::XLayer*>, s_vec<f::FArrInt>&, Shape*);

NS_END(tetris)
#endif



