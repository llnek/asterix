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

#include "XLives.h"
USING_NS_CC;

//////////////////////////////////////////////////////////////////////////////
//
NS_AX_BEGIN

//////////////////////////////////////////////////////////////////////////////
//
void XLives::reduce(int x) {
  for (int n=0; n < x; ++n) {
    this.hud.removeIcon(this.icons.pop());
  }
  _curLives -= x;
}

bool XLives::isDead() {
  return _curLives < 0;
}

int XLives::getLives() {
  return _curLives;
}

void XLives::reset() {
  R.forEach((z) => { this.hud.removeIcon(z); }, this.icons);
  _curLives = this.options.totalLives;
  _icons=[];
}

void XLives::resurrect() {
  reset();
  drawLives();
}

void XLives::drawLives() {
  let y= this.topLeft.y - this.lifeSize.height * 0.5,
  x= this.topLeft.x + this.lifeSize.width * 0.5,
  gap=2;

  for (let n = 0; n < this.curLives; ++n) {
    let v= new XLive(x,y,this.options);
    this.hud.addIcon(v);
    this.icons.push(v);
    if (this.options.dir> 0) {
      x += this.lifeSize.width + gap;
    } else {
      x -= this.lifeSize.width - gap;
    }
  }
}

void XLives::create() {
  const dummy = new XLive(0,0,this.options);
  this.lifeSize = { width: ccsx.getScaledWidth(dummy),
                    height: ccsx.getScaledHeight(dummy) } ;
  this.drawLives();
}

XLives::XLives(hud, x, y, options) {
  this.options = options || {};
  _topLeft= cc.p(x,y);
  _icons= [];
  _hud= hud;
  _curLives= -1;
  _reset();
  if (! sjs.echt(this.options.dir)) {
    this.options.dir= 1;
  }
}

XLives::~XLives() {
}



NS_AX_END
//////////////////////////////////////////////////////////////////////////////
//EOF
