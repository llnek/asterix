//
//  GunSystem.m
//  MonsterWars
//
//  Created by Ray Wenderlich on 10/27/12.
//  Copyright (c) 2012 Razeware LLC. All rights reserved.
//

#import "GunSystem.h"
#import "Entity.h"
#import "EntityManager.h"
#import "SimpleAudioEngine.h"
#import "EntityFactory.h"

@implementation GunSystem

- (void)update:(float)dt {
    
    NSArray * entities = [self.entityManager getAllEntitiesPosessingComponentOfClass:[GunComponent class]];
    for (Entity * entity in entities) {
        
        TeamComponent * team = entity.team;
        RenderComponent * render = entity.render;
        GunComponent * gun = entity.gun;
        if (!team || !render || !gun) continue;
        
        Entity * enemy = [entity closestEntityOnTeam:OPPOSITE_TEAM(team.team)];
        if (!enemy) return;
        RenderComponent * enemyRender = enemy.render;
        if (!enemyRender) continue;
        
        float distance = ccpDistance(render.node.position, enemyRender.node.position);
        static float WIGGLE_ROOM = 5;
        if (ABS(distance) <= (gun.range + WIGGLE_ROOM) && CACurrentMediaTime() - gun.lastDamageTime > gun.damageRate) {
            
            [[SimpleAudioEngine sharedEngine] playEffect:gun.sound];
            
            gun.lastDamageTime = CACurrentMediaTime();
            
            Entity * laser = [self.entityFactory createLaserWithTeam:team.team];
            RenderComponent * laserRender = laser.render;
            MeleeComponent * laserMelee = laser.melee;
            if (!laserRender || !laserMelee) continue;
            
            laserRender.node.position = render.node.position;
            laserMelee.damage = gun.damage;
            
            CGPoint direction = ccpNormalize(ccpSub(enemyRender.node.position, render.node.position));
            static float laserPointsPerSecond = 100;
            static float laserDistance = 1000;
            
            CGPoint target = ccpMult(direction, laserDistance);
            float duration = laserDistance / laserPointsPerSecond;
            
            laserRender.node.rotation = -1 * CC_RADIANS_TO_DEGREES(ccpToAngle(direction));
            laserRender.node.zOrder = 1;
            
            [laserRender.node runAction:
             [CCSequence actions:
              [CCMoveBy actionWithDuration:duration position:target],
              [CCCallBlock actionWithBlock:^{
                 [laserRender.node removeFromParentAndCleanup:YES];
             }], nil]];
        }
        
    }
}

@end
