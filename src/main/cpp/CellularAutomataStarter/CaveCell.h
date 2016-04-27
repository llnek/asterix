//
//  CaveCell.h
//  CellularAutomataStarter
//
//  Created by Scott Gardner on 3/8/14.
//  Copyright (c) 2014 Kim Pedersen. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSInteger, CaveCellType) {
  CaveCellTypeInvalid = -1,
  CaveCellTypeWall,
  CaveCellTypeFloor,
  CaveCellTypeEntry,
  CaveCellTypeExit,
  CaveCellTypeTreasure,
  CaveCellTypeMax
};

@interface CaveCell : NSObject

@property (assign, nonatomic) CGPoint coordinate;
@property (assign, nonatomic) CaveCellType type;

- (instancetype)initWithCoordinate:(CGPoint)coordinate;

@end
