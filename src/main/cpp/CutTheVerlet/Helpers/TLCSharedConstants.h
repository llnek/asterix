//
//  TLCSharedConstants.h
//  CutTheVerlet
//
//  Created by Tammy Coron on 4/11/14.
//  Copyright (c) 2014 Tammy Coron. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(int, Layer)
{
    LayerBackground,
    LayerForeground,
    LayerPlayer,
    LayerRope,
    LayerPrize
};

typedef NS_OPTIONS(int, EntityCategory)
{
    EntityCategoryPlayer = 1 << 0,
    EntityCategoryRopeAttachment = 1 << 1,
    EntityCategoryRope = 1 << 2,
    EntityCategoryPrize = 1 << 3,
    EntityCategoryGround = 1 << 4
};

extern NSString *const kImageNameForRopeHolder;
extern NSString *const kImageNameForRopeTexture;

extern NSString *const kImageNameForPlayerBaseImage;
extern NSString *const kImageNameForPlayerMouthOpen;
extern NSString *const kImageNameForPlayerMouthClosed;

extern NSString *const kSoundFileNameForCutAction;
extern NSString *const kSoundFileNameForSplashAction;
extern NSString *const kSoundFileNameForBiteAction;

extern NSString *const kSoundFileNameForBackgroundMusic;

extern NSString *const kImageNameForPrize;
extern NSString *const kNodeNameForPrize;

@interface TLCSharedConstants : NSObject

@end
