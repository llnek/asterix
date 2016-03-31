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

  HUDLayer* getHUD() { return (HUDLayer*)getSceneX()->getLayer(3); }

  __decl_ptr(ecs::Node, _shared)

  STATIC_REIFY_LAYER(GLayer)
  __decl_deco_ui()
  __decl_get_iid(2)

  void createLevel(GVars*);
  void fallTile(GVars*, int,int,float);
  void drawPath(GVars*);
  void addTile(GVars*,TilePtrArray*,int, int);

  virtual void onMouseMotion(const c::Vec2&);
  virtual bool onMouseStart(const c::Vec2&);
  virtual void onMouseClick(const c::Vec2&);

  virtual void onTouchMotion(c::Touch*);
  virtual bool onTouchStart(c::Touch*);
  virtual void onTouchEnd(c::Touch*);

  virtual void onInited();

  virtual ~GLayer();
};

//////////////////////////////////////////////////////////////////////////////
//
GLayer::~GLayer() {
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onInited() {

  _shared= _engine->getNodes("n/GVars")[0];

  auto ss= CC_GEC(GVars,_shared,"n/GVars");
  auto wz= cx::visRect();
  auto wb= cx::visBox();

  ss->arrowsLayer = c::DrawNode::create();
  ss->globezLayer = c::Layer::create();

  addItem(ss->globezLayer);
  addItem(ss->arrowsLayer);

  createLevel(ss);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseClick(const c::Vec2 &loc) {
    auto ss=CC_GEC(GVars,_shared,"n/GVars");
  ss->arrowsLayer->clear();
  ss->startColor= "";

  for (auto i = 0; i < ss->visited.size(); ++i){
    auto &vt= ss->visited[i];
      auto sp= (GameTile*)ss->tiles[vt.row]->get(vt.col);
    if (ss->visited.size() < 3) {
      sp->setOpacity(255);
      sp->picked=false;
    } else {
      ss->globezLayer->removeChild(sp);
      ss->tiles[vt.row]->set(vt.col,CC_NIL);
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
          if (holesBelow>0) {
            cur->runAction(
                c::MoveTo::create(0.5,
                  c::Vec2(cur->getPositionX(),
                          cur->getPositionY()-holesBelow* ss->tileSize)));
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
void GLayer::onMouseMotion(const c::Vec2 &loc) {
    auto ss=CC_GEC(GVars,_shared,"n/GVars");
  if (ss->startColor == "") {
  return;}

  auto vsz= ss->visited.size();
  auto tsz=ss->tileSize;
  auto tsz2= tsz/2;

  auto cur_r = floor(loc.y / tsz);
  auto cur_c = floor(loc.x / tsz);
  auto cx = cur_c * tsz + tsz2;
  auto cy = cur_r * tsz + tsz2;
  auto dx = loc.x - cx;
  auto dy = loc.y - cy;

  if (dx*dx + dy*dy >= ss->tolerance) {
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
  auto tsz2=tsz/2;

  ss->globezLayer->addChild(sp,0);
  sp->setPosition(col*tsz+tsz2, (ss->fieldSize+height)*tsz);
  sp->runAction(
      c::MoveTo::create(0.5,
        c::Vec2(col*tsz+tsz2, row*tsz+tsz2)));
  ss->tiles[row]->set(col,sp);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::drawPath(GVars *ss) {
  ss->arrowsLayer->clear();
  if (ss->visited.size() ==0) { return; }
  auto tsz=ss->tileSize;
  auto tsz2=tsz/2;
  for (auto i=1; i < ss->visited.size(); ++i) {
    ss->arrowsLayer->drawSegment(
        c::Vec2(ss->visited[i-1].col*tsz+tsz2,
                ss->visited[i-1].row*tsz+tsz2),
        c::Vec2(ss->visited[i].col*tsz+tsz2,
                ss->visited[i].row*tsz+tsz2),
        4,
                                 c::Color4F(c::Color4B(255, 255, 255, 255)));
  }
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onMouseStart(const c::Vec2 &loc) {
    auto ss=CC_GEC(GVars,_shared,"n/GVars");
  auto r = floor(loc.y / ss->tileSize);
  auto c = floor(loc.x / ss->tileSize);
  auto sp= ss->tiles[r]->get(c);
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
  for (auto i = 0; i < len;  ++i) {
    auto arr= new TilePtrArray(len);
    ss->tiles.push_back(arr);
    for (auto j = 0; j < len; ++j) {
      addTile(ss, arr, i, j);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::addTile(GVars *ss, TilePtrArray *arr, int row, int col) {
  auto r= cx::randInt(ss->tileTypes.size());
  auto sp= GameTile::create(ss,r);
  ss->globezLayer->addChild(sp,0);
  sp->setPosition(col*ss->tileSize+ ss->tileSize/2,
  row* ss->tileSize + ss->tileSize/2);
  arr->set(col,sp);
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




