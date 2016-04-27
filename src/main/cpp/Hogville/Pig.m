//
//  Pig.m
//  Hogville
//
//  Created by Main Account on 3/1/14.
//  Copyright (c) 2014 Razeware LLC. All rights reserved.
//

#import "Pig.h"
#import "MyScene.h"

static const int POINTS_PER_SEC = 80;

@implementation Pig
{
  NSMutableArray *_wayPoints;
  CGPoint _velocity;
  SKAction *_moveAnimation;
  BOOL _hungry;
  BOOL _eating;
  BOOL _removing;
}

- (instancetype)initWithImageNamed:(NSString *)name {
  if(self = [super initWithImageNamed:name]) {
    _wayPoints = [NSMutableArray array];
    SKTexture *texture1 = [SKTexture textureWithImageNamed:@"pig_1"];
    SKTexture *texture2 = [SKTexture textureWithImageNamed:@"pig_2"];
    SKTexture *texture3 = [SKTexture textureWithImageNamed:@"pig_3"];
    _moveAnimation = [SKAction animateWithTextures:@[texture1, texture2, texture3] timePerFrame:0.1];
    self.physicsBody = [SKPhysicsBody bodyWithCircleOfRadius:self.size.width / 2.0f];
    self.physicsBody.categoryBitMask = LDPhysicsCategoryAnimal;
    self.physicsBody.contactTestBitMask = LDPhysicsCategoryAnimal | LDPhysicsCategoryFood;
    self.physicsBody.collisionBitMask = kNilOptions;
    _hungry = YES;
  }
  
  return self;
}

- (void)addPointToMove:(CGPoint)point {
  [_wayPoints addObject:[NSValue valueWithCGPoint:point]];
}

- (void)move:(NSNumber *)dt {

  if (!_eating) {
    CGPoint currentPosition = self.position;
    CGPoint newPosition;
    
    if(![self actionForKey:@"moveAction"]) {
      [self runAction:_moveAnimation withKey:@"moveAction"];
    }
    
    //1
    if([_wayPoints count] > 0) {
      CGPoint targetPoint = [[_wayPoints firstObject] CGPointValue];
      
      //2 TODO: Add movement logic here
      CGPoint offset = CGPointMake(targetPoint.x - currentPosition.x, targetPoint.y - currentPosition.y);
      CGFloat length = sqrtf(offset.x * offset.x + offset.y * offset.y);
      CGPoint direction = CGPointMake(offset.x / length, offset.y / length);
      _velocity = CGPointMake(direction.x * POINTS_PER_SEC, direction.y * POINTS_PER_SEC);
      
      //2
      newPosition = CGPointMake(currentPosition.x + _velocity.x * [dt doubleValue],
                                currentPosition.y + _velocity.y * [dt doubleValue]);
      //3
      if(CGRectContainsPoint(self.frame, targetPoint)) {
        [_wayPoints removeObjectAtIndex:0];
      }
    }
    else {
      newPosition = CGPointMake(currentPosition.x + _velocity.x * [dt doubleValue],
                                currentPosition.y + _velocity.y * [dt doubleValue]);
    }
    
    self.position = [self checkBoundaries:newPosition];
    self.zRotation = atan2f(_velocity.y, _velocity.x) + M_PI_2;
    [self checkForHome];
  }
}

- (void)moveRandom {
  //1
  [_wayPoints removeAllObjects];
 
  //2
  int width = (int)CGRectGetWidth(self.scene.frame);
  int height = (int)CGRectGetHeight(self.scene.frame);
  //3
  CGPoint randomPoint = CGPointMake(arc4random() % width,  arc4random() % height);
  [_wayPoints addObject:[NSValue valueWithCGPoint:randomPoint]];
  [_wayPoints addObject:[NSValue valueWithCGPoint:CGPointMake(randomPoint.x + 1, randomPoint.y + 1)]];
}

- (void)eat {
  //1
  if(_hungry) {
    //2
    [self removeActionForKey:@"moveAction"];
    _eating = YES;
    _hungry = NO;
 
    //3
    SKAction *blockAction = [SKAction runBlock:^{
      _eating = NO;
      [self moveRandom];
    }];
 
    [self runAction:[SKAction sequence:@[[SKAction waitForDuration:1.0], blockAction]]];
  }
}

- (CGPathRef)createPathToMove {
  //1
  CGMutablePathRef ref = CGPathCreateMutable();
    
  //2
  for(int i = 0; i < [_wayPoints count]; ++i) {
    CGPoint p = [_wayPoints[i] CGPointValue];
    p = [self.scene convertPointToView:p];
    //3
    if(i == 0) {
      CGPathMoveToPoint(ref, NULL, p.x, p.y);
    } else {
      CGPathAddLineToPoint(ref, NULL, p.x, p.y);
    }
  }
    
  return ref;
}

- (CGPoint)checkBoundaries:(CGPoint)point {
  //1
  CGPoint newVelocity = _velocity;
  CGPoint newPosition = point;
  
  //2
  CGPoint bottomLeft = CGPointZero;
  CGPoint topRight = CGPointMake(self.scene.size.width, self.scene.size.height);
  
  //3
  if (newPosition.x <= bottomLeft.x) {
    newPosition.x = bottomLeft.x;
    newVelocity.x = -newVelocity.x;
  }
  else if (newPosition.x >= topRight.x) {
    newPosition.x = topRight.x;
    newVelocity.x = -newVelocity.x;
  }
  
  if (newPosition.y <= bottomLeft.y) {
    newPosition.y = bottomLeft.y;
    newVelocity.y = -newVelocity.y;
  }
  else if (newPosition.y >= topRight.y) {
    newPosition.y = topRight.y;
    newVelocity.y = -newVelocity.y;
  }
  
  //4
  _velocity = newVelocity;
  
  return newPosition;
}

- (void)checkForHome {
  //1
  if(_hungry || _removing) {
    return;
  }
 
  //2
  SKSpriteNode *homeNode = ((MyScene *)self.scene).homeNode;
  CGRect homeRect = homeNode.frame;
 
  //3
  if(CGRectIntersectsRect(self.frame, homeRect)) {
    _removing = YES;
    [_wayPoints removeAllObjects];
    [self removeAllActions];
    //4
    [self runAction:
     [SKAction sequence:
      @[[SKAction group:
         @[[SKAction fadeAlphaTo:0.0f duration:0.5],
           [SKAction moveTo:homeNode.position duration:0.5]]],
        [SKAction removeFromParent]]]];
  }
}

- (void)clearWayPoints {
  [_wayPoints removeAllObjects];
}

@end
