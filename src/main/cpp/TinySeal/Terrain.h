//
//  Terrain.h
//  TinySeal
//
//  Created by Kauserali on 06/05/13.
//  Copyright 2013 Razeware LLC. All rights reserved.
//

#import "Box2D.h"

@class HelloWorldLayer;

@interface Terrain : CCNode
@property (retain) CCSpriteBatchNode * batchNode;
@property (retain) CCSprite * stripes;
- (void) setOffsetX:(float)newOffsetX;
- (id)initWithWorld:(b2World *)world;
@end