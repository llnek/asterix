ig.module('game.entities.marker')

.requires('impact.entity')

.defines(function() {

    EntityMarker = ig.Entity.extend({

        size: {

            x: 62,

            y: 62

        },

        player: undefined,

        hover: false,

        animSheet: new ig.AnimationSheet('media/markers.png', 62, 62),

        init: function(x, y, settings) {

            this.parent(x, y, settings);

            // Add image states.

            this.addAnim( 'x', 1, [ 0 ] );

            this.addAnim( 'x-hover', 1, [ 2 ] );

            this.addAnim( 'o', 1, [ 1 ] );

            this.addAnim( 'o-hover', 1, [ 3 ] );

            // No appearance to start.
            this.currentAnim = undefined;

        },

        update: function() {

            this.parent();

            if( typeof this.player === 'string' ) {

                this.currentAnim = this.anims[ this.player + ( this.hover ? '-hover' : '' ) ];

            }

        }

    });

});