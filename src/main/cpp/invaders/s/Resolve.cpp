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

#include "Resolve.h"
NS_USING(ash)
NS_BEGIN(invaders)

//////////////////////////////////////////////////////////////////////////
//
Resolve::Resolve(options)
  : state(options) {
}

//////////////////////////////////////////////////////////////////////////
//
void Resolve;:RemoveFromEngine(Engine* e) {
  aliens= nullptr;
  ships= nullptr;
  engine=nullptr;
}

//////////////////////////////////////////////////////////////////////////
//
void Resolve::AddToEngine(Engine* e) {
  aliens= e->GetNodeList(AlienMotionNode::TypeId());
  ships= e->GetNodeList(ShipMotionNode::TypeId());
  engine=e;
}

//////////////////////////////////////////////////////////////////////////
//
bool Resolve::Update(float dt) {
  auto enemies = aliens->head;
  auto ship = ships->head;

  CheckMissiles();
  CheckBombs();
  CheckAliens(enemies);
  CheckShip(ship);
}

//////////////////////////////////////////////////////////////////////////
//


  /**
   * @method checkMissiles
   * @private
   */
  checkMissiles() {
    const mss = sh.pools.Missiles,
    ht = ccsx.vrect().height;

    mss.iter( m => {
      if (m.status) {
        if (m.pos().y >= ht ||
            m.HP <= 0) {
          m.deflate();
        }
      }
    });
  },
  /**
   * @method checkBombs
   * @private
   */
  checkBombs() {
    const bbs = sh.pools.Bombs,
    bt = 0;

    bbs.iter( b => {
      if (b.status) {
        if (b.pos().y <= bt ||
            b.HP <= 0) {
          b.deflate();
        }
      }
    });
  },
  /**
   * @method checkAliens
   * @private
   */
  checkAliens(node) {
    const sqad= node.aliens;

    R.forEach( en => {
      if (en.status) {
        if (en.HP <= 0) {
          sh.fire('/game/players/earnscore', {
            score: en.value });
          en.deflate();
        }
      }
    }, sqad.aliens.pool);
  },
  /**
   * @method checkShip
   * @private
   */
  checkShip(node) {
    const ship = node.ship;

    if (ship.status &&
        ship.HP <= 0) {
      ship.deflate();
      sh.fire('/game/players/killed');
    }
  }

}, {

/**
 * @memberof module:s/resolve~Resolve
 * @property {Number} Priority
 */
Priority: xcfg.ftypes.Resolve
});


/** @alias module:s/resolve */
const xbox = /** @lends xbox# */{
  /**
   * @property {Resolve}  Resolve
   */
  Resolve : Resolve
}


sjs.merge(exports, xbox);
/*@@
return xbox;
@@*/
//////////////////////////////////////////////////////////////////////////////
//EOF

