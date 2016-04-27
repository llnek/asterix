//
//  GameScene.m
//  PompaDroid
//
//  Created by Allen Benson G Tan on 10/19/12.
//  Copyright 2012 WhiteWidget Inc. All rights reserved.
//

#import "GameScene.h"


@implementation GameScene

-(id)init
{
    if ((self = [super init]))
    {
        _gameLayer = [GameLayer node];
        [self addChild:_gameLayer z:0];
        
        _hudLayer = [HudLayer node];
        [self addChild:_hudLayer z:1];
        
        _hudLayer.dPad.delegate = _gameLayer;
        _gameLayer.hud = _hudLayer;
    }
    return self;
}

@end
