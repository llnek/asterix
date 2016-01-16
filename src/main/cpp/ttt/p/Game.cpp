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
  DECL_PTR(a::NodeList, board)
  DECL_PTR(a::NodeList, cells)
  DECL_PTR(a::NodeList, arena)
};

//////////////////////////////////////////////////////////////////////////////
void GLayer::postReify() {
  cells = engine->getNodeList(SquareNode().typeId());
  board = engine->getNodeList(BoardNode().typeId());
  arena = engine->getNodeList(ArenaNode().typeId());
  showGrid();
}

//////////////////////////////////////////////////////////////////////////////
void GLayer::showGrid() {
  auto view= CC_GNLF(GridView, arena, "view");
  auto gps= mapGridPos(1);
  for (auto n= cells->head; NNP(n); n=n->next) {
    auto cp= CC_GNF(CSquare,n,"square");
    auto pos= cp->cell;
    cp->setPos(cx::vboxMID( gps[pos] ));
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



