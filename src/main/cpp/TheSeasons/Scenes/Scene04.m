//
//  Scene04.m
//  TheSeasons
//
//  Created by Tammy Coron on 9/16/13.
//  Copyright (c) 2013 Tammy Coron. All rights reserved.
//

#import "Scene04.h" // this scence should be replaced with pg04

#import "Scene00.h"
#import "Scene03.h"

@import AVFoundation;

@implementation Scene04
{
    AVAudioPlayer *_backgroundMusicPlayer;
    SKSpriteNode *_btnSound;
    BOOL _soundOff;
    SKSpriteNode *_btnLeft;
}

#pragma mark -
#pragma mark Scene Setup and Initialize

-(id)initWithSize:(CGSize)size
{
    if (self = [super initWithSize:size])
    {
        /* set up your scene here */
        
        /* set up Sound */
        
        _soundOff = [[NSUserDefaults standardUserDefaults] boolForKey:@"pref_sound"];
        [self playBackgroundMusic:@"title_bgMusic.mp3"];
        
        /* add background image */
        
        SKSpriteNode *background = [SKSpriteNode spriteNodeWithImageNamed:@"to_be_continued"];
        background.anchorPoint = CGPointZero;
        background.position = CGPointZero;
        
        [self addChild:background];
        
        /* additional Setup */
        
        [self setUpFooter];
    }
    return self;
}

- (void)willMoveFromView:(SKView *)view
{
    /* you can also stop the bg music here */
    
    // [_backgroundMusicPlayer stop];
}

#pragma mark -
#pragma mark Standard Scene Setup

- (void)setUpFooter
{
    /* add the footer */
    
    SKSpriteNode *footer = [SKSpriteNode spriteNodeWithImageNamed:@"footer"];
    footer.position = CGPointMake(self.size.width/2, 38);
    
    [self addChild:footer];
    
    /* add the left button */
    
    _btnLeft = [SKSpriteNode spriteNodeWithImageNamed:@"button_left"];
    _btnLeft.position = CGPointMake(self.size.width/2 + 400, 38);
    
    [self addChild:_btnLeft];
    
    /* add the sound button */
    
    if (_soundOff)
    {
        // NSLog(@"_soundOff");
        
        [_btnSound removeFromParent];
        
        _btnSound = [SKSpriteNode spriteNodeWithImageNamed:@"button_sound_off"];
        _btnSound.position = CGPointMake(self.size.width/2 + 330, 38);
        
        [self addChild:_btnSound];
        [_backgroundMusicPlayer stop];
    }
    else
    {
        // NSLog(@"_soundOn");
        
        [_btnSound removeFromParent];
        
        _btnSound = [SKSpriteNode spriteNodeWithImageNamed:@"button_sound_on"];
        _btnSound.position = CGPointMake(self.size.width/2 + 330, 38);
        
        [self addChild:_btnSound];
        [_backgroundMusicPlayer play];
    }
}

#pragma mark -
#pragma mark Code For Sound & Ambiance

- (void)showSoundButtonForTogglePosition:(BOOL )togglePosition
{
    // NSLog(@"togglePosition: %i", togglePosition);
    
    if (togglePosition)
    {
        _btnSound.texture = [SKTexture textureWithImageNamed:@"button_sound_on"];
        
        _soundOff = NO;
        [[NSUserDefaults standardUserDefaults] setBool:NO forKey:@"pref_sound"];
        [[NSUserDefaults standardUserDefaults] synchronize];
        
        [_backgroundMusicPlayer play];
    }
    else
    {
        _btnSound.texture = [SKTexture textureWithImageNamed:@"button_sound_off"];
        
        _soundOff = YES;
        [[NSUserDefaults standardUserDefaults] setBool:YES forKey:@"pref_sound"];
        [[NSUserDefaults standardUserDefaults] synchronize];
        
        [_backgroundMusicPlayer stop];
    }
}

- (void)playBackgroundMusic:(NSString *)filename
{
    NSError *error;
    NSURL *backgroundMusicURL = [[NSBundle mainBundle] URLForResource:filename withExtension:nil];
    _backgroundMusicPlayer = [[AVAudioPlayer alloc] initWithContentsOfURL:backgroundMusicURL error:&error];
    _backgroundMusicPlayer.numberOfLoops = -1;
    _backgroundMusicPlayer.volume = 1.0;
    [_backgroundMusicPlayer prepareToPlay];
}

#pragma mark -
#pragma mark Touch Events

-(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
  /* Called when a touch begins */
  for (UITouch *touch in touches)
  {
    CGPoint location = [touch locationInNode:self];
    
    
    if([_btnSound containsPoint:location])
    {
      // NSLog(@"xxxxxxxxxxxxxxxxxxx sound toggle");
      
      [self showSoundButtonForTogglePosition:_soundOff];
    }
    else if ([_btnLeft containsPoint:location])
    {
      // NSLog(@"<<<<<<<<<<<<<<<<<< page backward");
      
      if (![self actionForKey:@"readText"]) // do not turn page if reading
      {
        [_backgroundMusicPlayer stop];
        
        SKScene *scene = [[Scene03 alloc] initWithSize:self.size];
        SKTransition *sceneTransition = [SKTransition fadeWithColor:[UIColor darkGrayColor] duration:1];
        [self.view presentScene:scene transition:sceneTransition];
      }
    }
    else if ( location.x >= 29 && location.x <= 285 && location.y >= 6 && location.y <= 68 )
    {
      // NSLog(@">>>>>>>>>>>>>>>>> page title");
      
      if (![self actionForKey:@"readText"]) // do not turn page if reading
      {
        [_backgroundMusicPlayer stop];
        
        SKScene *scene = [[Scene00 alloc] initWithSize:self.size];
        SKTransition *sceneTransition = [SKTransition fadeWithColor:[UIColor darkGrayColor] duration:1];
        [self.view presentScene:scene transition:sceneTransition];
      }
    }
  }
}

#pragma mark -
#pragma mark Game Loop

-(void)update:(CFTimeInterval)currentTime
{
    /* Called before each frame is rendered */
}

@end
