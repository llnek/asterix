ig.module(
	'game.entities.yellowinvader'
)
.requires(
	'impact.entity'
)
.defines(function(){
	
EntityYellowinvader = EntityInvader.extend({
	animSheet: new ig.AnimationSheet( 'media/invaders/yellow.png', 8, 8 )
});

});