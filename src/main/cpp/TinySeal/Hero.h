//
//  Hero.h
//  TinySeal
//
//  Created by Kauserali on 06/05/13.
//  Copyright 2013 Razeware LLC. All rights reserved.
//

#import "Box2D.h"

@interface Hero : CCSprite

@property (readonly) BOOL awake;

- (id)initWithWorld:(b2World *)world;
- (void)update;
- (void)wake;
- (void)dive;
- (void)limitVelocity;

- (void)nodive;
- (void)runForceAnimation;
- (void)runNormalAnimation;
@end
