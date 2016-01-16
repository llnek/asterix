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
#include "s/utils.h"
#include "Game.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
BEGIN_NS_UNAMED()
struct CC_DLL GLayer : public f::GameLayer {
  virtual void postReify();
  void showGrid();
  DECL_PTR(a::NodeList, squares)
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
void GLayer::showGrid() {
  auto css= CC_GNLF(CSquares,arena, "squares");
  auto gps= mapGridPos(1);
  F__LOOP(it,css->sqs) {
    auto cp= *it;
    auto pos= cp->cell;
    cp->sprite->setPosition(cx::vboxMID( gps[pos] ));
    cp->toggle(false);
    view->layer->
      addAtlasItem(
          "game-pics", cp->sprite);
  }
}



END_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
void Game::decorate() {

}




NS_END(tttoe)



