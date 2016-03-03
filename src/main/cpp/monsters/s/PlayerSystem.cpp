//
//  PlayerSystem.m
//  MonsterWars
//
//  Created by Ray Wenderlich on 10/27/12.
//  Copyright (c) 2012 Razeware LLC. All rights reserved.
//

#import "PlayerSystem.h"
#import "EntityManager.h"
#import "PlayerComponent.h"
#import "MoveComponent.h"
#import "TeamComponent.h"
#import "RenderComponent.h"
#import "HealthComponent.h"

@implementation PlayerSystem

- (void)handleMover:(Entity *)mover attacking:(BOOL)attacking {
   
    TeamComponent * moverTeam = mover.team;
    RenderComponent * moverRender = mover.render;
    MoveComponent * moverMove = mover.move;
    if (!moverTeam || !moverRender || !moverMove) return;
    
    if (attacking) {
    
        Entity * enemy = [mover closestEntityOnTeam:OPPOSITE_TEAM(moverTeam.team)];
        if (!enemy) {
            moverMove.moveTarget = moverRender.node.position;
            return;
        }
        
        RenderComponent * enemyRender = enemy.render;
        if (!enemyRender) return;
        
        moverMove.moveTarget = enemyRender.node.position;
        
        GunComponent * gun = mover.gun;
        if (gun) {
            CGPoint vector = ccpNormalize(ccpSub(moverRender.node.position, enemyRender.node.position));
            moverMove.moveTarget = ccpAdd(enemyRender.node.position, ccpMult(vector, gun.range));
        }
        
    } else {
        
        Entity * player = [mover playerForTeam:moverTeam.team];
        RenderComponent * playerRender = player.render;
        if (!playerRender) return;
        
        moverMove.moveTarget = playerRender.node.position;
    }
    
}

- (void)update:(float)dt {
    
    double time = CACurrentMediaTime();
    
    NSArray * entities = [self.entityManager getAllEntitiesPosessingComponentOfClass:[PlayerComponent class]];
    for (Entity * entity in entities) {
        
        PlayerComponent * player = entity.player;
        TeamComponent * team = entity.team;
        RenderComponent * render = entity.render;
        
        // Handle coins
        static float COIN_DROP_INTERVAL = 1.5;
        static float COINS_PER_INTERVAL = 5;
        if (time - player.lastCoinDrop > COIN_DROP_INTERVAL) {
            player.lastCoinDrop = time;
            player.coins += COINS_PER_INTERVAL;
        }
        
        // Update player image
        if (render && team) {
            if (player.attacking) {
                NSString * spriteFrameName = [NSString stringWithFormat:@"castle%d_atk.png", team.team];
                [render.node setDisplayFrame:[[CCSpriteFrameCache sharedSpriteFrameCache] spriteFrameByName:spriteFrameName]];
            } else {
                NSString * spriteFrameName = [NSString stringWithFormat:@"castle%d_def.png", team.team];
                [render.node setDisplayFrame:[[CCSpriteFrameCache sharedSpriteFrameCache] spriteFrameByName:spriteFrameName]];
            }
        }
        
        // Update monster movement orders
        if (team) {
            NSArray * movers = [entity getAllEntitiesOnTeam:team.team posessingComponentOfClass:[MoveComponent class]];
            for (Entity * mover in movers) {                
                [self handleMover:mover attacking:player.attacking];
            }
        }        
        
    }
}

@end
