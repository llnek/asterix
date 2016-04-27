//
//  HUDLayer.h
//  Raycast
//
//  Created by Ray Wenderlich on 8/18/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "cocos2d.h"

@interface HUDLayer : CCLayer {
    CCLabelBMFont * _statusLabel;
}

- (void)showRestartMenu:(BOOL)won;
- (void)setStatusString:(NSString *)string;

@end
