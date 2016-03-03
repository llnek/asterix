//
//  MoveSystem.m
//  MonsterWars
//
//  Created by Ray Wenderlich on 10/27/12.
//  Copyright (c) 2012 Razeware LLC. All rights reserved.
//

#import "MoveSystem.h"
#import "EntityManager.h"
#import "MoveComponent.h"
#import "RenderComponent.h"

@implementation MoveSystem

- (CGPoint)arriveEntity:(Entity *)entity withMoveComponent:(MoveComponent *)move renderComponent:(RenderComponent *)render {
    
    CGPoint vector = ccpSub(move.moveTarget, render.node.position);
    float distance = ccpLength(vector);
    
    float targetRadius = 5;
    float slowRadius = targetRadius + 25;
    static float timeToTarget = 0.1;
    
    if (distance < targetRadius) {
        return CGPointZero;
    }
    
    float targetSpeed;
    if (distance > slowRadius) {
        targetSpeed = move.maxVelocity;
    } else {
        targetSpeed = move.maxVelocity * distance / slowRadius;
    }
    
    CGPoint targetVelocity = ccpMult(ccpNormalize(vector), targetSpeed);
    
    CGPoint acceleration = ccpMult(ccpSub(targetVelocity, move.velocity), 1/timeToTarget);
    if (ccpLength(acceleration) > move.maxAcceleration) {
        acceleration = ccpMult(ccpNormalize(acceleration), move.maxAcceleration);
    }
    return acceleration;
}

- (CGPoint)separateEntity:(Entity *)entity withMoveComponent:(MoveComponent *)move renderComponent:(RenderComponent *)render teamComponent:(TeamComponent *)team {
    
    CGPoint steering = CGPointZero;
    NSArray * entities = [entity getAllEntitiesOnTeam:team.team posessingComponentOfClass:[MoveComponent class]];
    for (Entity * otherEntity in entities) {
    
        if (otherEntity.eid == entity.eid) continue;
        RenderComponent * otherRender = otherEntity.render;
        if (!otherRender) continue;
        
        CGPoint direction = ccpSub(render.node.position, otherRender.node.position);
        float distance = ccpLength(direction);
        static float SEPARATE_THRESHHOLD = 20;
        
        if (distance < SEPARATE_THRESHHOLD) {
            direction = ccpNormalize(direction);
            steering = ccpAdd(steering, ccpMult(direction, move.maxAcceleration));
        }
    }
    return steering;
}

- (void)update:(float)dt {
    
    NSArray * entities = [self.entityManager getAllEntitiesPosessingComponentOfClass:[MoveComponent class]];
    for (Entity * entity in entities) {
        
        MoveComponent * move = entity.move;
        RenderComponent * render = entity.render;
        TeamComponent * team = entity.team;
        if (!move || !render || !team) continue;
        
        CGPoint arriveComponent = [self arriveEntity:entity withMoveComponent:move renderComponent:render];
        CGPoint separateComponent = [self separateEntity:entity withMoveComponent:move renderComponent:render teamComponent:team];
        CGPoint newAcceleration = ccpAdd(arriveComponent, separateComponent);
        
        // Update current acceleration based on the above, and clamp
        move.acceleration = ccpAdd(move.acceleration, newAcceleration);
        if (ccpLength(move.acceleration) > move.maxAcceleration) {
            move.acceleration = ccpMult(ccpNormalize(move.acceleration), move.maxAcceleration);
        }
        
        // Update current velocity based on acceleration and dt, and clamp
        move.velocity = ccpAdd(move.velocity, ccpMult(move.acceleration, dt));
        if (ccpLength(move.velocity) > move.maxVelocity) {
            move.velocity = ccpMult(ccpNormalize(move.velocity), move.maxVelocity);
        }
        
        // Update position based on velocity
        CGPoint newPosition = ccpAdd(render.node.position, ccpMult(move.velocity, dt));
        CGSize winSize = [CCDirector sharedDirector].winSize;
        newPosition.x = MAX(MIN(newPosition.x, winSize.width), 0);
        newPosition.y = MAX(MIN(newPosition.y, winSize.height), 0);
        render.node.position = newPosition;
        
    }
}

@end
