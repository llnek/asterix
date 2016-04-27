//
//  ShortestPathStep.h
//  CellularAutomataStarter
//
//  Created by Scott Gardner on 3/9/14.
//  Copyright (c) 2014 Kim Pedersen. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface ShortestPathStep : NSObject

@property (assign, nonatomic) CGPoint position;
@property (assign, nonatomic) NSInteger gScore;
@property (assign, nonatomic) NSInteger hScore;
@property (strong, nonatomic) ShortestPathStep *parent;

- (instancetype)initWithPosition:(CGPoint)pos;
- (NSInteger)fScore;

@end
