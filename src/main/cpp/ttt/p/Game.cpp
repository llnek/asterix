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

//////////////////////////////////////////////////////////////////////////////
void Game::postReify() {
  cellPicNode = engine->getNodeList(CellPicNode().typeId());
  boardNode = engine->getNodeList(BoardNode().typeId());
  showGrid();
}

//////////////////////////////////////////////////////////////////////////////
void Game::showGrid() {
  auto view= CC_GNLF(PlayView, boardNode, "view");
  auto gps= mapGridPos(1);

  for (auto node= cellPicNode->head; NNP(node); node=node->next) {
    auto cp= CC_GNF(CellPic,node,"cell");
    auto pos= cp->cell;
    cp->setPos(cx::vboxMID( gps[pos] ));
    view->layer->addAtlasItem("game-pics", sp);
  }
}



END_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
void Game::decorate() {

}




NS_END(tttoe)



