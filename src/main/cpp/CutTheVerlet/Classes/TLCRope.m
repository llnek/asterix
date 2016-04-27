//
//  TLCRope.m
//  CutTheVerlet
//
//  Created by Tammy Coron on 4/10/14.
//  Copyright (c) 2014 Tammy Coron. All rights reserved.
//

#import "TLCRope.h"

@interface TLCRope ()

@property (nonatomic, strong) NSString *name;

@property (nonatomic, strong) NSMutableArray *ropeNodes;

@property (nonatomic, strong) SKNode *attachmentNode;
@property (nonatomic, assign) CGPoint attachmentPoint;

@property (nonatomic, assign) int length;

@end

@implementation TLCRope

#pragma mark -
#pragma mark Init Method

- (instancetype)initWithLength:(int)length usingAttachmentPoint:(CGPoint)point toNode:(SKNode*)node withName:(NSString *)name withDelegate:(id<TLCRopeDelegate>)delegate;
{
    self = [super init];
    if (self) {
        self.delegate = delegate;
        
        self.name = name;
        
        self.attachmentNode = node;
        self.attachmentPoint = point;
        
        self.ropeNodes = [NSMutableArray arrayWithCapacity:length];
        
        self.length = length;
    }
    return self;
}


#pragma mark
#pragma mark Setup Physics

/* Physics code below is based on demo code provided at: https://github.com/mraty/spritekit-ropes */

- (void)addRopePhysics
{
    // keep track of the current rope part position
    CGPoint currentPosition = self.attachmentPoint;
    
    // add each of the rope parts
    for (int i = 0; i < self.length; i++) {
        SKSpriteNode *ropePart = [SKSpriteNode spriteNodeWithImageNamed:kImageNameForRopeTexture];
        ropePart.name = self.name;
        ropePart.position = currentPosition;
        ropePart.anchorPoint = CGPointMake(0.5, 0.5);
        
        [self addChild:ropePart];
        [self.ropeNodes addObject:ropePart];
        
        CGFloat offsetX = ropePart.frame.size.width * ropePart.anchorPoint.x;
        CGFloat offsetY = ropePart.frame.size.height * ropePart.anchorPoint.y;

        CGMutablePathRef path = CGPathCreateMutable();

        CGPathMoveToPoint(path, NULL, 0 - offsetX, 7 - offsetY);
        CGPathAddLineToPoint(path, NULL, 7 - offsetX, 7 - offsetY);
        CGPathAddLineToPoint(path, NULL, 7 - offsetX, 0 - offsetY);
        CGPathAddLineToPoint(path, NULL, 0 - offsetX, 0 - offsetY);

        CGPathCloseSubpath(path);

        ropePart.physicsBody = [SKPhysicsBody bodyWithPolygonFromPath:path];
        ropePart.physicsBody.allowsRotation = YES;
        ropePart.physicsBody.affectedByGravity = YES;

        ropePart.physicsBody.categoryBitMask = EntityCategoryRope;
        ropePart.physicsBody.collisionBitMask = EntityCategoryRopeAttachment;
        ropePart.physicsBody.contactTestBitMask =  EntityCategoryPrize;

        [ropePart skt_attachDebugFrameFromPath:path color:[SKColor redColor]];
        CGPathRelease(path);
        
        // set the next rope part position
        currentPosition = CGPointMake(currentPosition.x, currentPosition.y - ropePart.size.height);
        
    }
    
    [self addRopeJoints];
}

- (void)addRopeJoints
{
    // setup joint for the initial attachment point
    SKNode *nodeA = self.attachmentNode;
    SKSpriteNode *nodeB = [self.ropeNodes objectAtIndex:0];
    
    SKPhysicsJointPin *joint = [SKPhysicsJointPin jointWithBodyA: nodeA.physicsBody
                                                           bodyB: nodeB.physicsBody
                                                          anchor: self.attachmentPoint];
    
    // force the attachment point to be stiff
    joint.shouldEnableLimits = YES;
    joint.upperAngleLimit = 0;
    joint.lowerAngleLimit = 0;
    
    [self.delegate addJoint:joint];
    
    // setup joints for the rest of the rope parts
    for (int i = 1; i < self.length; i++) {
        SKSpriteNode *nodeA = [self.ropeNodes objectAtIndex:i-1];
        SKSpriteNode *nodeB = [self.ropeNodes objectAtIndex:i];
        SKPhysicsJointPin *joint = [SKPhysicsJointPin jointWithBodyA: nodeA.physicsBody
                                                               bodyB: nodeB.physicsBody
                                                              anchor: CGPointMake(CGRectGetMidX(nodeA.frame),
                                                                                  CGRectGetMinY(nodeA.frame))];
        // allow joint to rotate freely
        joint.shouldEnableLimits = NO;
        joint.upperAngleLimit = 0;
        joint.lowerAngleLimit = 0;
        
        [self.delegate addJoint:joint];
    }
}

#pragma mark -
#pragma mark Helper Methods

- (NSUInteger)getRopeLength
{
    return self.ropeNodes.count;
}

- (NSMutableArray *)getRopeNodes
{
    return self.ropeNodes;
}

@end
