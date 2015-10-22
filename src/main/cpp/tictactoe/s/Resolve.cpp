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
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
//
Resolve::Resolve(not_null<EFactory*> f,
    not_null<c:Dictionary*> d)
  : f::BaseSystem(f,d) {
}

//////////////////////////////////////////////////////////////////////////
//
void Resolve::AddToEngine(not_null<a::Engine*> e) {
  BoardNode n;
  board = e->GetNodeList(n.TypeId());
}

//////////////////////////////////////////////////////////////////////////
//
void Resolve::OnUpdate(float dt) {
  auto node= board->head;
  if (MGMS()->IsRunning() &&
      NNP(node)) {
    SyncUp(node, dt);
    DoIt(node, dt);
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Resolve::SyncUp(a::Node* node) {
  auto view= CC_GNF(GridView, node, "view");
  auto grid= CC_GNF(Grid, node, "grid");
  auto nil= CC_CSV(c::Integer, "CV_Z");
  let values= node.grid.values,
  cs= view.cells,
  z,c, offset;

  for (int i=0; i < grid->values.Size(); ++i) {
    if (grid->values[i] != nil) {
      auto c= XrefCell(i, view);
      if (NNP(c)) {
        z=cs[pos];
        if (!!z) {
          z[0].removeFromParent();
        }
        cs[pos] = [utils.drawSymbol(view, c[0], c[1], v),
                   c[0], c[1], v];
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
const c::Vec2 Resolve::XrefCell(int pos, const s::array<Box4>& map) {
  auto cells = CC_GDV(c::Integer, state, "CELLS");
  auto delta=0;

  if (pos >= 0 && pos < cells) {
    auto gg = map[pos];
    auto x = gg.left + (gg.right - gg.left  - delta) * 0.5;
    auto y = gg.top - (gg.top - gg.bottom - delta ) * 0.5;
    // the cell's center
    return c::Vec2(x,y);
  } else {
    return c::Vec2(-1, -1);
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Resolve::DoIt(a::Node* node, float dt) {
  let values= node.grid.values,
  msg,
  rc,
  res;

  if (R.find( p => {
      if (!!p) {
        rc= this.checkWin(p,values);
        if (rc) {
          return res=[p, rc];
        }
      }
    }, this.state.players)) {
    this.doWin(node, res[0], res[1]);
  }
  else
  if (this.checkDraw(values)) {
    this.doDraw(node);
  }
  else
  if (this.state.msgQ.length > 0) {
    msg = this.state.msgQ.shift();
    if ("forfeit" === msg) {
      this.doForfeit(node);
    }
  }
},
  /**
   * @method doWin
   * @private
   */
  doWin(node, winner, combo) {
    sh.fire('/hud/score/update',
            {color: winner.color,
             score: 1});
    this.doDone(node, winner, combo);
  },
  /**
   * @method doDraw
   * @private
   */
  doDraw(node) {
    this.doDone(node, null, []);
  },
  /**
   * @method doForfeit
   * @private
   */
  doForfeit(node) {
    let other = this.state.actor===1 ? 2 : this.state.actor===2 ? 1 : 0,
    tv = this.state.players[this.state.actor],
    win= this.state.players[other],
    cs = node.view.cells,
    v2= -1,
    layer= node.view.layer;

    if (!!tv) {
      v2 = tv.value;
    }

    sh.fire('/hud/score/update',
            {color: win.color,
             score: 1});

    //gray out the losing icons
    R.forEachIndexed((z, n) => {
      if (!!z && z[4] === v2) {
        z[0].removeFromParent();
        z[0] = utils.drawSymbol(node.view, z[1], z[2], z[3]+2);
      }
    }, cs);

    this.doDone(node, win, null);
  },
  /**
   * Flip all other icons except for the winning ones.
   * @method showWinningIcons
   * @private
   */
  showWinningIcons(view, combo) {
    const layer= view.layer,
    cs = view.cells;

    if (combo===null) { return; }

    R.forEachIndexed((z, n) => {
      if (! R.contains(n, combo)) { if (!!z && z[3] !== csts.CV_Z) {
        z[0].removeFromParent();
        z[0] = utils.drawSymbol(view, z[1], z[2], z[3], true);
      } }
    }, cs);
  },
  /**
   * @method doDone
   * @private
   */
  doDone(node, pobj, combo) {

    const pnum = !!pobj ? pobj.pnum : 0;

    this.showWinningIcons(node.view, combo);
    sh.fire('/hud/timer/hide');
    sh.sfxPlay('game_end');
    sh.fire('/hud/end', { winner: pnum });

    this.state.lastWinner = pnum;
    this.state.running=false;
  },
  /**
   * @method checkDraw
   * @private
   */
  checkDraw(values) {
    return ! (csts.CV_Z === R.find((v) => {
      return (v === csts.CV_Z);
    }, values));
  },
  /**
   * @method checkWin
   * @private
   */
  checkWin(actor, game) {
    //sjs.loggr.debug('checking win for ' + actor.color);
    let combo, rc= R.any( r => {
      combo=r;
      return R.all( n => {
        return actor.value === n;
      },
      R.map( i => { return game[i]; }, r));
    },
    this.state.GOALSPACE);

    return rc ? combo : null;
  }

},{
/**
 * @memberof module:s/resolve~Resolve
 * @property {Number} Priority
 */
Priority: xcfg.ftypes.Resolve
});



/** @alias module:s/resolve */
const xbox = {
  /**
   * @property {Resolve} Resolve
   */
  Resolve: Resolve
};
sjs.merge(exports, xbox);
/*@@
return xbox;
@@*/
//////////////////////////////////////////////////////////////////////////////
//EOF

