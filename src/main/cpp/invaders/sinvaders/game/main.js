ig.module(
  'game.main'
)
.requires(
  'impact.game',
  'game.entities.player',
  'game.entities.invader',
  'impact.font',
  'game.entities.yellowinvader',
  'game.entities.whiteinvader',
  'game.entities.redinvader',
  'game.levels.first'

)
.defines(function(){

MyGame = ig.Game.extend({

  // Load a font
  font: new ig.Font( 'media/04b03.font.png' ),
  invaders: [],
  stepTimer: new ig.Timer(),
  fireTimer: new ig.Timer(),
  sfxStep: new ig.Sound( 'media/sounds/move.*',true ),
  step:0,
  stepy:0,
  dirx:5,
  diry:0,
  playerLives:3,
  playerScore:0,

  init: function() {
    // Initialize your game here; bind keys etc.
    ig.input.bind( ig.KEY.LEFT_ARROW, 'left' );
    ig.input.bind( ig.KEY.RIGHT_ARROW, 'right' );
    ig.input.bind( ig.KEY.SPACE, 'shoot' );

    for(var i=0;i<=8;i++){
      this.invaders.push(this.spawnEntity( EntityInvader,(i*10)+3,5 ));
    }
    for(var i=9;i<=17;i++){
      this.invaders.push(this.spawnEntity(EntityRedinvader,((i-9)*10)+3,15 ));
    }
    for(var i=18;i<=26;i++){
      this.invaders.push(this.spawnEntity( EntityYellowinvader,((i-18)*10)+3,25));
    }
    for(var i=27;i<=35;i++){
      this.invaders.push(this.spawnEntity( EntityWhiteinvader,((i-27)*10)+3,35));
    }
    this.spawnEntity( EntityPlayer,60,135);

    this.stepTimer.set(1);
    this.fireTimer.set(1.5);

    for( i in ig.Image.cache ) {
    var img = ig.Image.cache[i];
                this.pixify( img, ig.system.scale );
    }

    //this.loadLevel( LevelFirst );
  },

  update: function() {
    // Update all entities and BackgroundMaps
    this.parent();

    if(this.playerLives>0){

      if(this.stepTimer.delta()>0){
        for(i in this.invaders){
          if(this.invaders[i].pos != undefined ){
            if(this.diry == 0){
              this.invaders[i].pos.x += this.dirx;
            }else{
              this.invaders[i].pos.y += this.diry;
            }

            if(this.stepy >2 && this.stepy<5){
              this.invaders[i].currentAnim.frameTime = 0.7;
            }

            if(this.stepy >5){
              this.invaders[i].currentAnim.frameTime = 0.5;
            }

          }

        }
        this.step++;
        if(this.diry == 8){
          this.diry = 0;
          this.step = 0;
          this.stepy++;
        }
        if(this.invaders.length>0){
          this.sfxStep.play();
        }
        if(this.stepy >2 && this.stepy<5){
          this.stepTimer.set(0.6);
        }

        if(this.stepy >5){
          this.stepTimer.set(0.4);
        }
        this.stepTimer.reset();
      }

      if(this.fireTimer.delta()>0){

        chooseElement = Math.floor(Math.random()*this.invaders.length);
        if(this.invaders.length > 0){
          this.invaders[chooseElement].canFire = true;
        }

        this.fireTimer.reset();
      }
      //choose randomly entity to fire

      if(this.step < 8){
        //this.diry = 0;
      }else {
        this.step = 0;
        this.diry = 8;
        if(this.dirx == 5){
          this.dirx = -5;
        }else{
          this.dirx = 5;
        }
      }
    }else{

    }
  },

  draw: function() {
    // Draw all entities and BackgroundMaps
    this.parent();
    this.font.draw( 'Score '+this.playerScore, 3, 153 );
    this.font.draw( 'Lives '+this.playerLives, 90, 153 );
    if(this.playerLives == 0){
      this.font.draw( 'GAME OVER ', 40, 80 );
    }
    if(this.invaders.length == 0){
      this.font.draw( 'GAME FINISHED !', 30, 80 );
    }
  },

  pixify: function( img, s ) {
        var ctx = img.data.getContext('2d');
        var px = ctx.getImageData(0, 0, img.data.width, img.data.height);

        for( var y = 0; y < img.data.height; y++ ) {
            for( var x = 0; x < img.data.width; x++ ) {
                var index = (y * img.data.width + x) * 4;
                var alpha = (x % s == 0 || y % s == 0) ? 0 : 0.9;
                px.data[index + 3] = px.data[index + 3] * alpha;
            }
        }
        ctx.putImageData( px, 0, 0 );
  }
});

ig.main( '#canvas', MyGame, 60, 125, 160, 3);

});
