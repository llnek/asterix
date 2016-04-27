//
//  GameLayer.h
//  PompaDroid
//
//  Created by Allen Benson G Tan on 10/19/12.
//  Copyright 2012 WhiteWidget Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "cocos2d.h"
#import "Hero.h"
#import "SimpleDPad.h"
#import "HudLayer.h"

@interface GameLayer : CCLayer <SimpleDPadDelegate> {
    CCTMXTiledMap *_tileMap;
    CCSpriteBatchNode *_actors;
    Hero *_hero;
}

@property(nonatomic,weak)HudLayer *hud;
@property(nonatomic,strong)CCArray *robots;

@end
