//
//  TLCRope.h
//  CutTheVerlet
//
//  Created by Tammy Coron on 4/10/14.
//  Copyright (c) 2014 Tammy Coron. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <SpriteKit/SpriteKit.h>

@protocol TLCRopeDelegate
- (void)addJoint:(SKPhysicsJointPin *)joint;
@end

@interface TLCRope : SKNode

@property (strong, nonatomic) id<TLCRopeDelegate> delegate;

- (instancetype)initWithLength:(int)length usingAttachmentPoint:(CGPoint)point toNode:(SKNode*)node withName:(NSString *)name withDelegate:(id<TLCRopeDelegate>)delegate;
- (void)addRopePhysics;

- (NSUInteger)getRopeLength;
- (NSMutableArray *)getRopeNodes;

@end
