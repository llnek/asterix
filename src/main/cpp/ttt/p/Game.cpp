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

#include "core/XConfig.h"
#include "core/CCSX.h"
#include "s/utils.h"
#include "s/GEngine.h"
#include "Game.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
BEGIN_NS_UNAMED()
class CC_DLL GLayer : public f::GameLayer {
  void showGrid();
public:

  STATIC_REIFY_LAYER(GLayer)
  MDECL_DECORATE()

  virtual void postReify();

  DECL_PTR(a::NodeList, board)
  DECL_PTR(a::NodeList, arena)
};

//////////////////////////////////////////////////////////////////////////////
void GLayer::postReify() {
  board = engine->getNodeList(BoardNode().typeId());
  arena = engine->getNodeList(ArenaNode().typeId());
  showGrid();
}

//////////////////////////////////////////////////////////////////////////////
void GLayer::decorate() {

  f::emptyQueue<sstr>( MGMS()->msgQueue() );

  centerImage("game.bg");
  incIndexZ();

  regoAtlas("game-pics");
  regoAtlas("lang-pics");

  auto ctx = (GCXX*) getSceneX()->getCtx();
  auto pnum= JS_INT(ctx->data["pnum"]);
  auto ppids = ctx->data["ppids"];
  auto p1c= CC_CSS("P1_COLOR");
  auto p2c= CC_CSS("P2_COLOR");
  sstr p1k;
  sstr p2k;
  sstr p1n;
  sstr p2n;
  J__LOOP(it, ppids) {
    auto &arr= it.value() ;
    if (JS_INT(arr[0]) == 1) {
      p1n= JS_STR(arr[1]);
      p1k= it.key();
    } else {
      p2n= JS_STR(arr[1]);
      p2k= it.key();
    }
  }

  CCLOG("seed =\n%s", ctx->data.dump(0).c_str());

  engine = mc_new_1(GEngine,pnum);

  //getHUD()->regoPlayers(p1c, p1k, p1n, p2c, p2k, p2n);
  //getHUD()->reset();

  CCLOG("init-game - ok");
}

//////////////////////////////////////////////////////////////////////////////
void GLayer::showGrid() {
  auto css= CC_GNLF(CSquares,arena, "squares");
  auto gps= mapGridPos(1);
  for (int i=0; i < gps.size(); ++i) {
    auto s= css->sqs[i];
    auto &bx= gps[i];
    assert(s->cell == i);
    s->sprite->setPosition(cx::vboxMID(bx));
    addAtlasItem( "game-pics", s->sprite);
  }
}



END_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic &t, void *m) {

}

//////////////////////////////////////////////////////////////////////////////
void Game::decorate() {
  GLayer::reify(this,2);
  play();
}




NS_END(tttoe)



