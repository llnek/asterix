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

#include "Logic.h"
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////
//
Logic::Logic(not_null<a::Engine*> e,
    not_null<c::Dictionary*> options)

  : f::BaseSystem(e,options) {

  SNPTR(botTimer)
}

//////////////////////////////////////////////////////////////////////////
//
void Logic::RemoveFromEngine(not_null<a::Engine*> e) {
  SNPTR(board)
}

//////////////////////////////////////////////////////////////////////////
//
void Logic::AddToEngine(not_null<a::Engine*> e) {
  BoardNode n;
  board = e->GetNodeList(n.TypeId());
}

//////////////////////////////////////////////////////////////////////////
//
bool Logic::OnUpdate(float dt) {
  auto node= board->head;
  if (MGMS()->IsRunning() && NNP(node)) {
    DoIt(node, dt);
  }
}

void Logic::DoIt(a::Node* node, float dt) {
  const ps= this.state.players,
  cp= ps[this.state.actor],
  board= node.board,
  grid= node.grid,
  bot= node.robot,
  sel= node.selection;

  //handle online play
  if (sjs.isobj(this.state.wsock)) {
    //if the mouse click is from the valid user, handle it
    if (!!cp && (this.state.pnum === cp.pnum)) {
      this.enqueue(sel.cell,cp.value,grid);
    }
  }
  else
  if (cp.category === csts.BOT) {
    // for the bot, create some small delay...
    if (!!this.botTimer) {
      if (ccsx.timerDone(this.botTimer)) {
        let bd= bot.algo.getGameBoard(),
        rc;
        bd.syncState(grid.values, this.state.players[this.state.actor].value);
        rc= bd.getFirstMove();
        if (!sjs.echt(rc)) { rc = bot.algo.eval(); }
        this.enqueue(rc,cp.value,grid);
        this.botTimer=ccsx.undoTimer(this.botTimer);
      }
    } else {
      this.botTimer = ccsx.createTimer(sh.main, 0.6);
    }
  }
  else
  if (sel.cell >= 0) {
    //possibly a valid click ? handle it
    this.enqueue(sel.cell,cp.value,grid);
  }

  sel.cell= -1;
}

/**
 * @method enqueue
 * @private
 */
enqueue(pos, value, grid) {

  if ((pos >= 0 && pos < grid.values.length) &&
      csts.CV_Z === grid.values[pos]) {

    let snd, pnum;

      sh.fire('/hud/timer/hide');

      if (sjs.isobj(this.state.wsock)) {
        this.onEnqueue(grid,this.state.actor,pos);
      } else {
        if (this.state.actor === 1) {
          snd= 'x_pick';
          pnum = 2;
        } else {
          snd= 'o_pick';
          pnum = 1;
        }
        grid.values[pos] = value;
        this.state.actor = pnum;
        sh.sfxPlay(snd);
        if (this.state.players[pnum].category === csts.HUMAN) {
          sh.fire('/hud/timer/show');
        }
      }
    }
  },
  /**
   * @method onEnqueue
   * @private
   */
  onEnqueue(grid,pnum,cell) {
    const src= {
      color: this.state.players[pnum].color,
      value: this.state.players[pnum].value,
      grid: grid.values,
      cell: cell
    },
    snd = pnum===1 ? 'x_pick' : 'o_pick',
    evt= {
      source: sjs.jsonfy(src),
      type: evts.MSG_SESSION,
      code: evts.PLAY_MOVE
    };
    this.state.wsock.send(evt);
    this.state.actor=0;
    sh.sfxPlay(snd);
  }
},{
/**
 * @memberof module:s/logic~Logic
 * @property {Number} Priority
 */
Priority : xcfg.ftypes.Logic
});

NS_END(tttoe)

