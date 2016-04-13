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
#include "s/GEngine.h"
#include "HUD.h"
#include "MMenu.h"
#include "Ende.h"
#include "Game.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(globes)
BEGIN_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
struct CC_DLL GLayer : public f::GameLayer {

  HUDLayer* getHUD() {
    return (HUDLayer*)getSceneX()->getLayer(3); }

  __decl_ptr(ecs::Node, _shared)

  __decl_create_layer(GLayer)
  __decl_deco_ui()
  __decl_get_iid(2)

  void createLevel(GVars*);
  void fallTile(GVars*, int,int,float);
  void drawPath(GVars*);

  virtual void onMouseMotion(const CCT_PT&);
  virtual bool onMouseStart(const CCT_PT&);
  virtual void onMouseClick(const CCT_PT&);

  virtual void onTouchMotion(c::Touch*);
  virtual bool onTouchStart(c::Touch*);
  virtual void onTouchEnd(c::Touch*);

  virtual void onInited();

  virtual ~GLayer() {}
};

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onInited() {

  _shared= _engine->getNodes("n/GVars")[0];

  auto ss= CC_GEC(GVars,_shared,"n/GVars");
  auto wz= cx::visSize();
  auto wb= cx::visBox();

  ss->arrowsLayer = c::DrawNode::create();
  ss->globezLayer = c::Layer::create();

  addItem(ss->globezLayer);
  addItem(ss->arrowsLayer);

  createLevel(ss);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseClick(const CCT_PT &loc) {
  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  ss->arrowsLayer->clear();
  ss->startColor= "";

  F__LOOP(it, ss->visited) {
    auto &vt= *it;
    auto sp= (GameTile*)ss->tiles[vt.row]->get(vt.col);
    if (ss->visited.size() < 3) {
      sp->setOpacity(255);
      sp->picked=false;
    } else {
      ss->tiles[vt.row]->set(vt.col,CC_NIL);
      ss->globezLayer->removeChild(sp);
    }
  }

  if (ss->visited.size() >= 3) {
    for (auto i = 1; i <= ss->fieldSize-1; ++i) {
      for (auto j = 0; j < ss->fieldSize; ++j) {
        auto cur=ss->tiles[i]->get(j);
        if (cur != CC_NIL) {
          auto holesBelow = 0;
          for (auto k = i-1; k >= 0; --k) {
            if (ss->tiles[k]->get(j) == CC_NIL) {
              ++holesBelow;
            }
          }
          if (holesBelow > 0) {
            cur->runAction(
                c::MoveTo::create(0.5,
                  CCT_PT(cur->getPositionX(),
                         cur->getPositionY()-holesBelow * ss->tileSize)));
             ss->tiles[i - holesBelow]->set(j,cur);
             ss->tiles[i]->set(j,CC_NIL);
          }
        }
      }
    }

    for (auto i = 0; i < ss->fieldSize; ++i) {
      auto j=0;
      for (j = ss->fieldSize-1; j>=0;  --j) {
        if (ss->tiles[j]->get(i) != CC_NIL) {
        break;
       }
      }
      auto missingGlobes = ss->fieldSize-1-j;
      if (missingGlobes > 0) {
        for (j=0; j < missingGlobes; ++j) {
          fallTile(ss,ss->fieldSize-j-1, i, missingGlobes-j);
        }
      }
    }
  }

  ss->visited.clear();
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseMotion(const CCT_PT &loc) {
  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  if (ss->startColor == "") {
  return;}

  auto pt= translatePt(ss, loc);
  auto vsz= ss->visited.size();
  auto tsz=ss->tileSize;
  auto tsz2= HTV(tsz);

  auto cur_r = (int) floor(pt.y / tsz);
  auto cur_c = (int) floor(pt.x / tsz);
  auto cy = ss->grid.bottom + (cur_r * tsz + tsz2);
  auto cx = ss->grid.left + cur_c * tsz + tsz2;
  auto dx = loc.x - cx;
  auto dy = loc.y - cy;

  if (dx*dx + dy*dy >= ss->tolerance) {
      //CCLOG("ignore!!!!, dx = %f, dy = %f", dx, dy);
  return;}

  if ( ! ss->tiles[cur_r]->get(cur_c)->picked) {
    if (abs(cur_r - ss->visited[vsz-1].row) <= 1 &&
       abs(cur_c - ss->visited[vsz-1].col) <= 1) {

      if (ss->tiles[cur_r]->get(cur_c)->tileColor == ss->startColor) {
        ss->tiles[cur_r]->get(cur_c)->setOpacity(128);
        ss->tiles[cur_r]->get(cur_c)->picked=true;
        ss->visited.push_back(f::Cell2D(cur_r,cur_c));
      }
    }
  } else {
    if (vsz >= 2 &&
        cur_r == ss->visited[vsz-2].row &&
        cur_c == ss->visited[vsz-2].col) {

      ss->tiles[ss->visited[vsz-1].row]->get(ss->visited[vsz-1].col)->setOpacity(255);
      ss->tiles[ss->visited[vsz-1].row]->get(ss->visited[vsz-1].col)->picked=false;
      ss->visited.pop_back();
    }
  }

  drawPath(ss);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::fallTile(GVars *ss, int row, int col, float height) {
  auto rtile = cx::randInt(ss->tileTypes.size());
  auto sp = GameTile::create(ss, rtile);
  auto tsz=ss->tileSize;
  auto tsz2= HTV(tsz);
  auto y0= ss->grid.bottom;
  auto x0= ss->grid.left;

  ss->globezLayer->addChild(sp,0);
  CC_POS2(sp, x0 + col*tsz+tsz2,
              (ss->fieldSize+height)*tsz);

  sp->runAction(
      c::MoveTo::create(0.5,
        CCT_PT(x0 + col*tsz+tsz2, y0 + (row*tsz+tsz2))));

  ss->tiles[row]->set(col,sp);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::drawPath(GVars *ss) {
  ss->arrowsLayer->clear();
  if (ss->visited.size() ==0) { return; }
  auto tsz=ss->tileSize;
  auto tsz2=tsz/2;
  auto y0= ss->grid.bottom;
  auto x0= ss->grid.left;
  for (auto i=1; i < ss->visited.size(); ++i) {
    ss->arrowsLayer->drawSegment(
        CCT_PT(x0 + ss->visited[i-1].col*tsz+tsz2,
               y0 + (ss->visited[i-1].row*tsz+tsz2)),
        CCT_PT(x0 + ss->visited[i].col*tsz+tsz2,
               y0 + (ss->visited[i].row*tsz+tsz2)),
        4,
        c::Color4F(c::Color4B(255, 255, 255, 255)));
  }
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onMouseStart(const CCT_PT &loc) {
  auto ss=CC_GEC(GVars,_shared,"n/GVars");

  if (!cx::pointInBox(ss->grid, loc.x, loc.y)) {
    return false;
  }

  auto pt= translatePt(ss, loc);
  auto r = (int) floor(pt.y / ss->tileSize);
  auto c = (int) floor(pt.x / ss->tileSize);
  auto sp= ss->tiles[r]->get(c);
    CCLOG("row = %d, col = %d", r, c);
  sp->setOpacity(128);
  sp->picked = true;
  ss->startColor = sp->tileColor;
  ss->visited.push_back(f::Cell2D(r,c));
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onTouchStart(c::Touch *touch) {
  return onMouseStart(touch->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchMotion(c::Touch *touch) {
  onMouseMotion(touch->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchEnd(c::Touch *touch) {
  onMouseClick(touch->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::createLevel(GVars* ss) {
  auto len= ss->fieldSize;
  auto sw= ss->tileSize;
  auto wb=cx::visBox();
  auto y= ss->grid.bottom;
  auto x0= ss->grid.left;
  auto x=x0;

  for (auto i =0; i < len; ++i) {
    auto arr= new TilePtrArray(len);
    ss->tiles.push_back(arr);
    for (auto j = 0; j < len; ++j) {
      auto r= cx::randInt(ss->tileTypes.size());
      auto sp= GameTile::create(ss,r);
      CC_POS2(sp, x+ HTV(sw), y + HTV(sw));
      arr->set(j,sp);
      ss->globezLayer->addChild(sp,0);
      x += sw;
    }
    y += sw;
    x= x0;
  }
}

//////////////////////////////////////////////////////////////////////////////
void GLayer::decoUI() {

  _engine = mc_new(GEngine);

  auto bg= c::LayerGradient::create(c::Color4B(0x00,0x22,0x22,255),
      c::Color4B(0x22,0x00,0x44,255));
  addItem(bg,-1);

  regoAtlas("game-pics");

  //cx::sfxMusic("background", true);
}

END_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic &topic, void *m) {
  auto y= (GLayer*) getGLayer();
}

//////////////////////////////////////////////////////////////////////////////
void Game::decoUI() {
  HUDLayer::reify(this, 3);
  GLayer::reify(this, 2);
  play();
}


NS_END




