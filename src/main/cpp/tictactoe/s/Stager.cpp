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

#include "core/CCSX.h"
#include "Stager.h"
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
Stager::Stager(not_null<a::Engine*> e,
    not_null<c::Dictionary*> options)

  : f::BaseSystem(e,options) {

  this->inited=false;
}

//////////////////////////////////////////////////////////////////////////////
//
Stager::~Stager() {

}

//////////////////////////////////////////////////////////////////////////////
//
void Stager::RemoveFromEngine(not_null<a::Engine*> e) {
  SNPTR(board)
}

void Stager::AddToEngine(not_null<a::Engine*> e) {
  e->AddEntity(sh.factory.reifyBoard(sh.main,
                                       this.state));
  CCLOG("adding system: Stager");
  if (! inited) {
    OnceOnly(e);
    inited=true;
  }
  BoardNode n;
  board= e->GetNodeList(n.TypeId());
}

bool Stager::OnUpdate(float dt) {
  if (cx::IsTransitioning()) { return false; }
  auto node= board->head;
  if (MGMS()->IsRunning() && NNP(node) {
    DoIt(node,dt);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Stager::ShowGrid(a::Node* node) {
  auto cs= a::NodeFld<GridView>(node, "view");
  auto mgs = MapGridPos();
  auto pos=0;
  sp;

  R.forEach( mp => {
    sp= ccsx.createSprite('z.png');
    sp.setPosition(ccsx.vboxMID(mp));
    sh.main.addAtlasItem('game-pics',sp);
    cs[pos++]=[sp, sp.getPositionX(), sp.getPositionY(), csts.CV_Z];
  }, mgs);
}

//////////////////////////////////////////////////////////////////////////////
//
void Stager::OnceOnly(a::Node* node, float dt) {

  ShowGrid(node);

  if (MGMS()->IsOnline()) {
    CCLOG("reply to server: session started ok");
    MGMS()->NetSend({
      type: evts.MSG_SESSION,
      code: evts.STARTED
    });
    state->setObject(c::Integer::create(0), "actor");
  } else {
    //randomly pick a player to start the game.
    let pnum = sjs.randSign() > 0 ? 1 : 2;
    this.state.actor=pnum;
    if (this.state.players[pnum].category === csts.HUMAN) {
      sh.fire('/hud/timer/show');
    }
    else
    if (this.state.players[pnum].category === csts.BOT) {
    }
  }

  sh.main.pkInput();
}

  /**
   * @method doit
   * @private
   */
  doit(node,dt) {

    let active = this.state.running,
    actor = this.state.actor;

    if (!active) {
      actor= this.state.lastWinner;
    }

    sh.fire('/hud/update', {
      running: active,
      pnum: actor
    });
  }

}, {
/**
 * @memberof module:s/stager~Stager
 * @property {Number} Priority
 */
Priority: xcfg.ftypes.PreUpdate
});


/** @alias module:s/stager */
const xbox = {
  /**
   * @property {Stager} Stager
   */
  Stager : Stager
};
sjs.merge(exports, xbox);
/*@@
return xbox;
@@*/
//////////////////////////////////////////////////////////////////////////////
//EOF


