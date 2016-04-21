ig.module(
	'game.entities.whiteinvader'
)
.requires(
	'impact.entity'
)
.defines(function(){
	
EntityWhiteinvader = EntityInvader.extend({
	animSheet: new ig.AnimationSheet( 'media/invaders/white.png', 8, 8 )
});

});