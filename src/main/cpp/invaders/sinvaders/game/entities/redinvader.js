ig.module(
	'game.entities.redinvader'
)
.requires(
	'impact.entity'
)
.defines(function(){
	
EntityRedinvader = EntityInvader.extend({
	animSheet: new ig.AnimationSheet( 'media/invaders/red.png', 8, 8 )
});

});