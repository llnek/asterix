ig.module('game.entities.grid')

.requires('impact.entity')

.defines(function() {

    EntityGrid = ig.Entity.extend({

        size: {

            x: 246,

            y: 247

        },

        // Size of n by n board.
        n: 3,

        // Who's turn is it?
        turn: 'x',

        moveCount: 0,

        winner: undefined,

        gameover: false,

        // How close must the mouse be to trigger hover-effect or click?
        clickDistance: 50,

        // From left-right, top-down.
        markerOffsets: [ 15, 11, 7 ],

        image: new ig.Image( 'media/grid.png' ),

        init: function(x, y, settings) {

            this.parent(x, y, settings);

            // Calculate unit size.
            this.unit = Math.floor( this.size.x / this.n );

            // Create new table of markers.
            this.clearMarkers();

            // Spawn (blank) markers to each block.

            for( var x = 0; x < this.n; x++ ) {

                for( var y = 0; y < this.n; y++ ) {

                    var markerX = this.pos.x + ( x * this.unit ) + this.markerOffsets[ x ],
                        markerY = this.pos.y + ( y * this.unit ) + this.markerOffsets[ y ];

                    this.markers[ x ][ y ] = ig.game.spawnEntity( EntityMarker, markerX, markerY, {} );

                }

            }

        },

        getClosestBlock: function() {

            var closestBlock = -1;

            var shortestDistance = -1;

            for( var x = 0; x < this.n; x++ ) {

                for( var y = 0; y < this.n; y++ ) {

                    var centerX = this.pos.x + ( this.size.x / this.n ) * x + ( this.size.x / this.n ) / 2;

                    var centerY = this.pos.y + ( this.size.y / this.n ) * y + ( this.size.y / this.n ) / 2;

                    var a = ig.input.mouse.x - centerX;

                    var b = ig.input.mouse.y - centerY;

                    // Hypotenuse / distance
                    var distance = Math.sqrt( Math.pow( a, 2 ) + Math.pow( b, 2 ) );

                    if( closestBlock == -1 ) {

                        // First case is always the closest.

                        shortestDistance = distance;

                        closestBlock = { x: x, y: y };

                    }

                    else if( distance < shortestDistance ) {

                        shortestDistance = distance;

                        closestBlock = { x: x, y: y };

                    }

                }

            }

            return {

                pos: closestBlock,

                distance: shortestDistance

            };

        },

        // Resets the game by clearing all markers from the grid.
        clearMarkers: function() {

            moveCount = 0;

            if( typeof this.markers === 'undefined' ) {

                this.markers = new Array();

                for( var x = 0; x < this.n; x++ ) {

                    this.markers[ x ] = new Array();

                    for( var y = 0; y < this.n; y++ ) {

                        this.markers[ x ][ y ] = undefined;

                    }

                }

            } else {

                for( var x = 0; x < this.markers.length; x++ ) {

                    for( var y = 0; y < this.markers[ x ].length; y++ ) {

                        // Only reset block if it's been set.
                        if( typeof this.markers[ x ][ y ] !== 'undefined' ) {

                            this.markers[ x ][ y ].kill();

                            this.markers[ x ][ y ] = undefined;

                        }

                    }

                }

            }

        },

        // Be sure to clean up children entities also.
        kill: function() {

            for( var x = 0; x < this.markers.length; x++ ) {

                for( var y = 0; y < this.markers[ x ].length; y++ ) {

                    if( typeof this.markers[ x ][ y ] !== 'undefined' ) {

                        this.markers[ x ][ y ].kill();

                    }

                }

            }

            this.parent();

        },

        endGame: function( winner ) {

            this.winner = winner;

            this.gameover = true;

        },

        checkForWinner: function( block, player ) {

            // Check column.
            for( i = 0; i < this.n; i++ ) {

                if( typeof this.markers[ block.pos.x ][ i ] === 'undefined' ) break;

                if( this.markers[ block.pos.x ][ i ].player != this.turn ) break;

                if( i == this.n - 1 ) {

                    this.endGame( player );

                    return;

                }

            }

            // Check row.
            for( i = 0; i < this.n; i++ ) {

                if( typeof this.markers[ i ][ block.pos.y ] === 'undefined' ) break;

                if( this.markers[ i ][ block.pos.y ].player != this.turn ) break;

                if( i == this.n - 1 ) {

                    this.endGame( player );

                    return;

                }

            }

            // Check diagonal.
            if( block.pos.x == block.pos.y ) {

                for( i = 0; i < this.n; i++ ) {

                    if( typeof this.markers[ i ][ i ] === 'undefined' ) break;

                    if( this.markers[ i ][ i ].player != this.turn ) break;

                    if( i == this.n - 1) {

                        this.endGame( player );

                        return;

                    }

                }

            }

            // Check anti-diagonal.
            for( i = 0; i < this.n; i++ ) {

                if( typeof this.markers[ i ][ ( this.n - 1 ) - i ] === 'undefined' ) break;

                if( this.markers[ i ][ ( this.n - 1 ) - i ].player != this.turn ) break;

                if( i == this.n - 1 ) {

                    this.endGame( player );

                    return;

                }

            }

            // Check draw.
            if( this.moveCount == ( Math.pow( this.n, 2 ) - 1 ) ) {

                this.endGame( 'draw' );

                return;

            }

        },

        update: function() {

            this.parent();

            // Marker hover effect.

            var block = this.getClosestBlock();

            // Game still running?
            if( !this.gameover ) {

                for( var x = 0; x < this.n; x++ ) {

                    for( var y = 0; y < this.n; y++ ) {

                        var marker = this.markers[ x ][ y ];

                        // Block unclaimed?
                        if( typeof marker.player === 'undefined' ) {

                            // Is the closest within-range block.
                            if( x == block.pos.x && y == block.pos.y
                                && block.distance <= this.clickDistance ) {

                                // Do effect.
                                marker.currentAnim = marker.anims[ this.turn + '-hover' ];

                            } else {

                                marker.currentAnim = undefined;

                            }

                        }

                    }

                }

            }

            // Handle mouse click.

            if( ig.input.pressed( 'mouse1' )
              && typeof this.winner === 'undefined' ) {

                var player = this.turn;

                // Is the block within range?
                if( block.distance <= this.clickDistance ) {

                    // Is block available to be marked?
                    if( typeof this.markers[ block.pos.x ][ block.pos.y ].player === 'undefined' ) {

                        // Place marker.

                        this.moveCount++;

                        this.markers[ block.pos.x ][ block.pos.y ].player = player;

                        // Check for winner.
                        this.checkForWinner( block, player );

                        // Alternate turns.
                        this.turn = ( this.turn === 'x' ? 'o' : 'x' );

                    }

                }

            }

        },

        draw: function() {

            this.image.draw( this.pos.x, this.pos.y );

        }

    });

});