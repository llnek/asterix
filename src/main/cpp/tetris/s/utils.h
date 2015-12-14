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

#include "n/CObjs.h"
NS_BEGIN(tetris)

//////////////////////////////////////////////////////////////////////////////
//
Shape* reifyShape(not_null<f::XLayer*> layer, cmap, Shape *shape);
Shape* previewShape(not_null<f::XLayer*> layer, Shape *shape);

float topLine(not_null<a::Node*> node);

const s_vec<Brick*> reifyBricks(not_null<f::XLayer*> layer,
    const sstr& png,
    float x, float y, const s_vec<c::Vec2>& bs);

void clearOldBricks(s_vec<Brick*>& bs);
void disposeShape(Shape *shape);

const s_vec<c::Vec2>
findBBox(cmap, BModel *model,
    float px, float py, int rID, bool skipCollide = false);

bool maybeCollide(cmap, float tl_x, float tl_y, float br_x, float br_y);

const f::Cell2D xrefTile(float x, float y);

void initDropper(Dropper *dp);

void setDropper(Dropper *dp, float r, float s);

void lockBricks(cmap, emap, Brick *z);

void lock(not_null<a::Node*> node, Shape *shape);

void postLock(not_null<a::Node*> node, cmap, emap);

bool testFilledRow(cmap, int r);

void flashFilled(emap, FilledLines *flines, const s_vec<int>& lines);

void pauseForClearance(not_null<a::Node*> node, bool b, float delay);

bool moveDown(not_null<f::XLayer*> layer, cmap, Shape *shape);

bool shiftRight(not_null<f::XLayer*> layer, cmap, Shape *shape);

bool shiftLeft(not_null<f::XLayer*> layer, cmap, Shape *shape);

bool rotateRight(not_null<f::XLayer*> layer,cmap, Shape *shape);

bool rotateLeft(not_null<f::XLayer*> layer, cmap, Shape *shape);


NS_END(tetris)
#endif



