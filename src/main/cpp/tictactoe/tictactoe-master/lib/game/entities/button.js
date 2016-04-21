ig.module('game.entities.button')

.requires('impact.entity')

.defines(function() {

    EntityButton = ig.Entity.extend({

        font: new ig.Font('media/04b03.font.png'),

        hoverfont: new ig.Font('media/04b03.font.hover.png'),

        text: 'Click Me.',

        color: 'white',

        hoverColor: 'grey',

        init: function(x, y, settings) {

            this.parent(x, y, settings);

            this.size.x = this.font.widthForString( this.text );

            this.size.y = this.font.heightForString( this.text );

        },

        // Returns true if mouse is over entity.
        hover: function() {

            var entityX = this.pos.x - this.offset.x - ig.game._rscreen.x;

            var entityY = this.pos.y - this.offset.y - ig.game._rscreen.y;

            var mouseX = ig.input.mouse.x;

            var mouseY = ig.input.mouse.y;

            // Within X bounds?
            return mouseX >= entityX
                && mouseX < entityX + this.size.x
                && mouseY >= entityY
                && mouseY < entityY + this.size.y;

        },

        draw: function() {

            this[ ( this.hover() ? 'hover' : '' ) + 'font' ].draw(

                this.text,

                this.pos.x - this.offset.x - ig.game._rscreen.x,

                this.pos.y - this.offset.y - ig.game._rscreen.y,

                ig.Font.ALIGN.LEFT

            );

            this.parent();

        },

        // What to do when clicked?
        action: function() {

            // Override this function with code.

        },

        update: function() {

            // Check if button has been clicked.
            if( ig.input.pressed( 'mouse1' ) ) {

                if( this.hover() ) {

                    this.action();

                }

            }

            this.parent();

        }

    });

});