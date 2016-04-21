ig.module(
  'game.entities.invader'
)
.requires(
  'impact.entity'
)
.defines(function(){

EntityInvader = ig.Entity.extend({
  size: {x: 8, y: 8},
  maxVel: {x: 100, y: 100},
  friction: {x: 150, y: 0},
  canFire: false,
  type: ig.Entity.TYPE.B, // Evil enemy group
  checkAgainst: ig.Entity.TYPE.A, // Check against friendly
  collides: ig.Entity.COLLIDES.PASSIVE,
  health: 10,
  speed: 14,
  stepTimer: new ig.Timer(),
  flip: false,
  sfxBoom: new ig.Sound( 'media/sounds/explosion.*',true ),
  animSheet: new ig.AnimationSheet( 'media/invaders/blue.png', 8, 8 ),
  init: function( x, y, settings ) {
    this.parent( x, y, settings );
    this.addAnim( 'idle', 1.01, [0,1,2] );
  },

  update: function() {

    this.parent();
    if(this.canFire){
      ig.game.spawnEntity(EntityBomb, this.pos.x, this.pos.y);
      this.canFire = false;
    }
  },

  check: function( other ) {
    other.receiveDamage( 10, this );
  },

  receiveDamage: function(amt,who){
    ig.game.spawnEntity(EntityExplosion, this.pos.x, this.pos.y);
    this.kill();
    this.sfxBoom.play();
  }

});


EntityExplosion = ig.Entity.extend({
  size: {x: 8, y: 8},
  type: ig.Entity.TYPE.NONE,

  animSheet: new ig.AnimationSheet( 'media/invaders/explosion.png', 8, 8 ),

  init: function( x, y, settings ) {
    this.parent( x, y, settings );
    this.addAnim( 'idle', 0.1, [0,1,2,3] );
  },

  update:function(){
    this.parent();

    if(this.currentAnim.loopCount){
      this.kill();
    }
  }
});

EntityBomb = ig.Entity.extend({
  size: {x: 3, y: 5},
  maxVel: {x: 200, y: 200},
  // The fraction of force with which this entity bounces back in collisions
  sfxBoom: new ig.Sound( 'media/sounds/explosion.*',true ),
  type: ig.Entity.TYPE.NONE,
  checkAgainst: ig.Entity.TYPE.A, // Check Against B - our evil enemy group
  collides: ig.Entity.COLLIDES.PASSIVE,

  animSheet: new ig.AnimationSheet( 'media/invaders/bomb.png', 3, 5 ),

  bounceCounter: 0,


  init: function( x, y, settings ) {
    this.parent( x, y, settings );
    this.vel.y = 50;
    this.addAnim( 'idle', 0.2, [0] );
  },

  // This function is called when this entity overlaps anonther entity of the
  // checkAgainst group. I.e. for this entity, all entities in the B group.
  check: function( other ) {
    other.receiveDamage( 10, this );
    ig.game.spawnEntity(EntityExplosion, this.pos.x, this.pos.y);
    this.sfxBoom.play();
    this.kill();
    ig.game.playerLives--;
    if(ig.game.playerLives >0){
      ig.game.spawnEntity( EntityPlayer,60,135);
    }


  }
});

});
