//
//  MeleeSystem.m
//  MonsterWars
//
//  Created by Ray Wenderlich on 10/27/12.
//  Copyright (c) 2012 Razeware LLC. All rights reserved.
//

#import "MeleeSystem.h"
#import "Entity.h"
#import "EntityManager.h"
#import "SimpleAudioEngine.h"

@implementation MeleeSystem

- (void)update:(float)dt {
       
    NSArray * entities = [self.entityManager getAllEntitiesPosessingComponentOfClass:[MeleeComponent class]];
    for (Entity * entity in entities) {
        
        RenderComponent * render = entity.render;
        MeleeComponent * melee = entity.melee;
        TeamComponent * team = entity.team;
        if (!render || !melee || !team) continue;
        
        BOOL aoeDamageCaused = FALSE;
        NSArray * enemies = [entity getAllEntitiesOnTeam:OPPOSITE_TEAM(team.team) posessingComponentOfClass:[RenderComponent class]];
        for (Entity * enemy in enemies) {
            
            RenderComponent * enemyRender = enemy.render;
            HealthComponent * enemyHealth = enemy.health;
            if (!enemyRender || !enemyHealth) continue;
            
            if (CGRectIntersectsRect(render.node.boundingBox, enemyRender.node.boundingBox)) {
                if (CACurrentMediaTime() - melee.lastDamageTime > melee.damageRate) {
                    
                    [[SimpleAudioEngine sharedEngine] playEffect:melee.sound];
                    
                    if (melee.aoe) {
                        aoeDamageCaused = YES;
                    } else {
                        melee.lastDamageTime = CACurrentMediaTime();
                    }
                    enemyHealth.curHp -= melee.damage;
                    if (enemyHealth.curHp < 0) {
                        enemyHealth.curHp = 0;
                    }
                    if (melee.destroySelf) {
                        [render.node removeFromParentAndCleanup:YES];
                        [self.entityManager removeEntity:entity];
                    }
                }
            }
        }
        
        if (aoeDamageCaused) {
            melee.lastDamageTime = CACurrentMediaTime();
        }
        
    }
}

@end
