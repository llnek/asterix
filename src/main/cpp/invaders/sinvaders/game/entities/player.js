ig.module(
  'game.entities.player'
)
.requires(
  'impact.entity'
)
.defines(function(){

EntityPlayer = ig.Entity.extend({

  // The players (collision) size is a bit smaller than the animation
  // frames, so we have to move the collision box a bit (offset)
  size: {x: 12, y:15},

  maxVel: {x: 100, y: 200},
  friction: {x: 600, y: 0},

  type: ig.Entity.TYPE.A, // Player friendly group
  checkAgainst: ig.Entity.TYPE.NONE,
  collides: ig.Entity.COLLIDES.PASSIVE,
  animSheet: new ig.AnimationSheet( 'media/player/player.png', 12, 15 ),
  flip: false,
  sfxFire: new ig.Sound( 'media/sounds/fire.*',true ),
  canShoot:false,
  fireCooldown: new ig.Timer(),

  init: function( x, y, settings ) {
    this.parent( x, y, settings );
                this.addAnim( 'idle', 1, [0] );
    this.addAnim( 'canfire', 1, [1] );
    this.fireCooldown.set(1);
  },


  update: function() {
    this.parent();

    if( ig.input.state('right') ) {

      var pp = (this.pos.x+this.size.x) - ig.game.screen.x;
      if( pp < ig.system.width){
        this.vel.x = 30;
      }


    }

    if( ig.input.state('left') ) {
      var pp = this.pos.x - ig.game.screen.x;
      if( pp > 0){
        this.vel.x = -30;
      }
    }
    if(this.fireCooldown.delta()>0){
      this.canShoot = true;
      this.currentAnim = this.anims.canfire;
    }

    if(ig.input.pressed('shoot') && this.canShoot){
      ig.game.spawnEntity(EntityProjectile, this.pos.x+5, this.pos.y-3);
      this.canShoot = false;
      this.currentAnim = this.anims.idle;
      this.sfxFire.play();
      this.fireCooldown.reset();
    }
  }
});


// The grenades a player can throw are NOT in a separate file, because
// we don't need to be able to place them in Weltmeister. They are just used
// here in the code.

// Only entities that should be usable in Weltmeister need to be in their own
// file.

EntityProjectile = ig.Entity.extend({
  size: {x: 2, y: 3},
  maxVel: {x: 200, y: 200},
  // The fraction of force with which this entity bounces back in collisions

  type: ig.Entity.TYPE.NONE,
  checkAgainst: ig.Entity.TYPE.B, // Check Against B - our evil enemy group
  collides: ig.Entity.COLLIDES.PASSIVE,

  animSheet: new ig.AnimationSheet( 'media/player/projectile.png', 2, 3 ),

  bounceCounter: 0,


  init: function( x, y, settings ) {
    this.parent( x, y, settings );
    this.vel.y = -50;
    this.addAnim( 'idle', 0.2, [0] );
  },

  // This function is called when this entity overlaps anonther entity of the
  // checkAgainst group. I.e. for this entity, all entities in the B group.
  check: function( other ) {
    other.receiveDamage( 10, this );
    ig.game.playerScore += 120;
    for(i in ig.game.invaders){

      if(ig.game.invaders[i] == other){
        ig.game.invaders.splice(i,1);
      }

    }
    this.kill();
  }
});

});
