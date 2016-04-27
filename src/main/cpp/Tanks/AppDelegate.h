//
//  AppDelegate.h
//  Tanks
//
//  Created by Ray Wenderlich on 12/8/11.
//  Copyright __MyCompanyName__ 2011. All rights reserved.
//

#import <UIKit/UIKit.h>

@class RootViewController;

@interface AppDelegate : NSObject <UIApplicationDelegate> {
	UIWindow			*window_;
	RootViewController	*viewController_;
}

@property (nonatomic, strong) UIWindow *window;
@property (nonatomic, strong) RootViewController *viewController;

@end
