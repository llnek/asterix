//
//  TLCGameData.h
//  CutTheVerlet
//
//  Created by Tammy Coron on 4/12/14.
//  Copyright (c) 2014 Tammy Coron. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface TLCGameData : NSObject

@property (nonatomic, assign) int name;

@property (nonatomic, assign) CGPoint ropeLocation;
@property (nonatomic, assign) int ropeLength;

@property (nonatomic, assign) BOOL isPrimaryRope;

@end
