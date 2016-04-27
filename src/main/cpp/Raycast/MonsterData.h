//
//  MonsterData.h
//  Raycast
//
//  Created by Ray Wenderlich on 8/18/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface MonsterData : NSObject 

@property CGPoint eye;
@property CGPoint target;
@property BOOL canSeePlayer;
@property double lastShot;

@end
