//
//  GameScene.h
//  PompaDroid
//
//  Created by Allen Benson G Tan on 10/19/12.
//  Copyright 2012 WhiteWidget Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "cocos2d.h"
#import "GameLayer.h"
#import "HudLayer.h"

@interface GameScene : CCScene {
    
}

@property(nonatomic,weak)GameLayer *gameLayer;
@property(nonatomic,weak)HudLayer *hudLayer;

@end
