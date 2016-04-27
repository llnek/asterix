//
//  HudLayer.h
//  PompaDroid
//
//  Created by Allen Benson G Tan on 10/19/12.
//  Copyright 2012 WhiteWidget Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "cocos2d.h"
#import "SimpleDPad.h"

@interface HudLayer : CCLayer {
    
}

@property(nonatomic,weak)SimpleDPad *dPad;

@end
