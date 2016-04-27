//
//  Pig.h
//  Hogville
//
//  Created by Main Account on 3/1/14.
//  Copyright (c) 2014 Razeware LLC. All rights reserved.
//

#import <SpriteKit/SpriteKit.h>

@interface Pig : SKSpriteNode

- (void)addPointToMove:(CGPoint)point; 
- (void)move:(NSNumber *)dt;
- (CGPathRef)createPathToMove;
- (void)eat;
- (void)moveRandom;
- (void)clearWayPoints;

@end
