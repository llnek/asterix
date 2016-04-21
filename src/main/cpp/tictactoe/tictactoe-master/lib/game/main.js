ig.module('game.main')

.requires(

//'impact.debug.debug',

'impact.game',

'impact.font',

'game.entities.button',

'game.entities.marker',

'game.entities.grid'

)

.defines(function() {

    MyGame = ig.Game.extend({

        // Load a font
        font: new ig.Font('media/04b03.font.png'),

        glow: new ig.Image( 'media/glow-effect.png' ),

        glow1: new ig.Image( 'media/glow1.png' ),

        glow2: new ig.Image( 'media/glow2.png' ),

        glow3: new ig.Image( 'media/glow3.png' ),

        timer: new ig.Timer(),

        init: function() {

            // Bind keys.
            ig.input.bind(ig.KEY.MOUSE1, 'mouse1');

            var gridX = ( ig.system.width / 2 ) - ( EntityGrid.prototype.size.x / 2 );

            var gridY = ( ig.system.height / 2 ) - ( EntityGrid.prototype.size.y / 2 );

            // Create grid entity.
            this.grid = this.spawnEntity( EntityGrid, gridX, gridY );

            var game = this;

            // Create reset button.
            this.resetButton = this.spawnEntity(

                EntityButton,

                4,

                4,

                {

                    text: 'Reset Game',

                    action: function() {

                        game.grid.kill();

                        game.grid = game.spawnEntity( EntityGrid, gridX, gridY );

                    }

                }

            );

        },

        update: function() {
            // Update all entities and backgroundMaps
            this.parent();

        },

        draw: function() {
            // Draw all entities and backgroundMaps
            this.parent();

            /*
            // Add your own drawing code here
            var x = ig.system.width / 2,
                y = ig.system.height / 2;

            this.font.draw('It Works!', x, y, ig.Font.ALIGN.CENTER);
            */

            if( this.grid.gameover ) {

                var text = '';

                if( this.grid.winner === 'draw' ) text = 'Draw!';

                else text = this.grid.winner.toUpperCase() + ' Wins!';

                this.font.draw(

                    text,

                    ig.system.width / 2,

                    ig.system.height - this.font.height,

                    ig.Font.ALIGN.CENTER

                );

            }

            var drawGlow = function( image, multiplier ) {

                var glowX = ( ig.system.width / 2 ) - ( image.width / 2 );

                var glowY = ( ig.system.height / 2 ) - ( image.height / 2 );

                ig.system.context.globalAlpha = Math.abs( Math.sin( ig.game.timer.delta() * multiplier ) );

                image.draw( glowX, glowY );

                ig.system.context.globalAlpha = 1;

            };

            // Draw glow effect.

            drawGlow( this.glow1, 0.2 );

            drawGlow( this.glow2, 0.4 );

            drawGlow( this.glow3, 0.6 );



        }

    });


    // Start the Game with 60fps, a resolution of 320x240, scaled
    // up by a factor of 2
    ig.main('#canvas', MyGame, 60, 320 , 480, 1);

});